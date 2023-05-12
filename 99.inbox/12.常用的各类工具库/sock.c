/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// #include "misc.h"
#include "sock.h"

// 在Windows socket下，用read()/write()总是读不到或写不出数据，但把read()/write()换成recv()/send()就正常
// recv()/send()是标准的socket函数，在任何操作系统下都可以用。

/*
  TcpListen  --- socket_listen
  TcpConnect --- socket_connect
  TcpConnect2--- socket_connect2
  TcpConnect_timeout --- socket_connecto
  TcpConnect2_timeout -- socket_connecto2
  Write      --- socket_send
  Read       --- socket_recv
  Readable   --- socket_recvable
  Writeable  --- socket_sendable
  Reado      --- socket_recvo
  Writeo     --- socket_sendo
  reado      --- socket_recvonf
  writeo     --- socket_sendonf
*/

#ifdef WIN32
#include <winsock.h>  //ws2_32.lib
#else                 // Linux
#include <netdb.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#endif

int socket_startup(void)
{
#ifdef _WIN32
    WSADATA wsad;
    WORD    wVersionReq;
    wVersionReq = MAKEWORD(1, 1);  // 清求不低于1.1版本的Winsock. 或者MAKEWORD(2,0);
    if (WSAStartup(wVersionReq, &wsad))
        return -1;

    return 0;
#else
    signal(SIGPIPE, SIG_IGN);  // 忽略SIGPIPE信号
    return 0;
#endif
}

void socket_cleanup(void)
{
#ifdef _WIN32
    WSACleanup();
#else
    signal(SIGPIPE, SIG_DFL);  // 恢复SIGPIPE信号
    return;
#endif
}

//  GetHostAddr: translate a string of host into struct sockaddr_in format
//    the string can be "192.115.123.199", also can be "xxx.com"
//    return :  -1: on failure
//               0: on okay
int socket_host2addr(char* host, int port, struct sockaddr_in* sin)
{
    bzero(sin, sizeof(struct sockaddr_in));
    // try A.B.C.D numeric IP format address first
    if ((sin->sin_addr.s_addr = inet_addr(host)) == INADDR_NONE) {
#ifndef _WIN32
        struct hostent he, *p;
        int            err, ret;
        char           buf[1024];
        ret = gethostbyname_r(host, &he, buf, sizeof(buf), &p, &err);
        if (ret < 0)
            return -1;
        memcpy(&sin->sin_addr.s_addr, he.h_addr, sizeof(sin->sin_addr.s_addr));
#else
        struct hostent* he;
        he = gethostbyname(host);  // 线程不安全的，不能用于多线程环境。
        if (!he)
            return -1;
        memcpy(&sin->sin_addr.s_addr, he->h_addr, sizeof(sin->sin_addr.s_addr));
#endif
    }

    sin->sin_family = AF_INET;
    sin->sin_port   = htons(port);

    return 0;
}

int socket_connect(char* host, int port)
{
    SOCKET             fd;
    struct sockaddr_in sin;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    bzero(&sin, sizeof(sin));
    // try A.B.C.D numeric IP format address first
    if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE) {
        // not numeric IP format, try text format
#ifndef WIN32
        struct hostent he, *p;
        int            err, ret;
        char           buf[1024];
        // Linux下使用可重入的gethostbyname_r，可以用于多线程环境。
        ret = gethostbyname_r(host, &he, buf, sizeof(buf), &p, &err);
        if (ret < 0)
            return -1;
        memcpy(&sin.sin_addr.s_addr, he.h_addr, sizeof(sin.sin_addr.s_addr));
#else
        struct hostent* he;
        he = gethostbyname(host);  // 线程不安全的，不能用于多线程环境。
        if (!he)
            return -1;
        memcpy(&sin.sin_addr.s_addr, he->h_addr, sizeof(sin.sin_addr.s_addr));
#endif
    }
    // here we assume we are always under IPv4 environment.
    // If one day shifted to other network environment, such as IPv6,
    // following code should also be under modification
    sin.sin_family = AF_INET;
    sin.sin_port   = htons(port);

    if (connect(fd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        closesocket(fd);
        return -1;
    }

    return fd;
}

int socket_connect2(struct sockaddr_in* psin)
{
    SOCKET fd;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    if (connect(fd, (struct sockaddr*)psin, sizeof(struct sockaddr)) < 0) {
        closesocket(fd);
        return -1;
    }

    return fd;
}

static int connect_o(SOCKET sockfd, const struct sockaddr* serv_addr, socklen_t addrlen, int sec, int usec)
{
    int            flags, n, error;
    socklen_t      len;
    fd_set         rset, wset;
    struct timeval tval;

#ifndef _WIN32
    flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0)
        return -1;

    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#else
    uint32_t rb = 1;
    ioctlsocket(sockfd, FIONBIO, &rb);
#endif

    error = 0;
    if ((n = connect(sockfd, serv_addr, addrlen)) < 0) {
        if (errno != WSAEINPROGRESS) {
            return -1;
        }
    }

    if (n == 0)
        goto done;

    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset         = rset;
    tval.tv_sec  = sec;
    tval.tv_usec = usec;

    if ((n = select(sockfd + 1, &rset, &wset, NULL, &tval)) == 0) {
        closesocket(sockfd);
        errno = WSAETIMEDOUT;
        return -1;
    }

    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
        len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
            return -1;
        }
    } else {
        return -1;
    }

done:
#ifndef _WIN32
    fcntl(sockfd, F_SETFL, flags);
#else
    rb = 0;
    ioctlsocket(sockfd, FIONBIO, &rb);
#endif
    if (error) {
        closesocket(sockfd);
        errno = error;
        return -1;
    }
    return 0;
}

// 采用超时和非阻塞的连接方式
int socket_connecto(char* host, int port, int sec, int usec)
{
    SOCKET             fd;
    struct sockaddr_in sin;

    if (sec == 0 && usec == 0) {
        return socket_connect(host, port);
    }

    if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    bzero(&sin, sizeof(sin));
    if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE) {
#ifndef _WIN32
        struct hostent he, *p;
        int            err, ret;
        char           buf[1024];
        ret = gethostbyname_r(host, &he, buf, sizeof(buf), &p, &err);
        if (ret < 0) {
            return -1;
        }
        memcpy(&sin.sin_addr.s_addr, he.h_addr, sizeof(sin.sin_addr.s_addr));
#else
        struct hostent* he;
        he = gethostbyname(host);
        if (!he)
            return -1;
        memcpy(&sin.sin_addr.s_addr, he->h_addr, sizeof(sin.sin_addr.s_addr));
#endif
    }

    sin.sin_family = AF_INET;
    sin.sin_port   = htons(port);

    if (connect_o(fd, (struct sockaddr*)&sin, sizeof(sin), sec, usec) < 0) {
        return -1;
    }

    return fd;
}

int socket_connecto2(struct sockaddr_in* psin, int sec, int usec)
{
    SOCKET fd;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    if (connect_o(fd, (struct sockaddr*)psin, sizeof(struct sockaddr), sec, usec) < 0) {
        return -1;
    }

    return fd;
}

int socket_listen(int port, int queue)
{
    SOCKET             listenfd;
    const int          on = 1;
    struct sockaddr_in sin;

    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        return -1;

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&sin, sizeof(sin));
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port        = htons(port);
    if (bind(listenfd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        closesocket(listenfd);
        return -1;
    }

    if (listen(listenfd, queue) < 0) {
        closesocket(listenfd);
        return -1;
    }

    return listenfd;
}

//////////////////////////////////////////////////////////
// 检查socket在secords时间内是否有数据读取。
// 参数：seconds为负值则表示一直阻塞到有数据读为止。
// 返回值：return -1 on error, 0 on timeout, 1 on read
int socket_recvable(SOCKET fd, int seconds)
{
    fd_set         fs;
    struct timeval tv;

    FD_ZERO(&fs);
    FD_SET(fd, &fs);
    if (seconds >= 0) {
        tv.tv_sec  = seconds;
        tv.tv_usec = 0;
        return (select(fd + 1, &fs, NULL, NULL, &tv));
    } else
        return (select(fd + 1, &fs, NULL, NULL, NULL));
}

// 检查socket在secords时间内是否可以写数据。
// 参数：seconds不能取负值。
// 返回值：return -1 on error, 0 on timeout, 1 on write
int socket_sendable(SOCKET fd, int seconds)
{
    fd_set         fs;
    struct timeval tv;

    tv.tv_sec  = seconds;
    tv.tv_usec = 0;
    FD_ZERO(&fs);
    FD_SET(fd, &fs);

    return (select(fd + 1, NULL, &fs, NULL, &tv));
}

int socket_recv(SOCKET fd, void* buf, int nbytes)
{
    int   nread, nleft = nbytes;
    char* ptr = (char*)buf;

    while (nleft > 0) {
        nread = recv(fd, ptr, nleft, 0);
        if (nread < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        } else if (nread == 0)  // 对方已关闭连接
            break;

        ptr += nread;
        nleft -= nread;
    }

    return (nbytes - nleft);
}

int socket_send(SOCKET fd, void* buf, int nbytes)
{
    size_t nwrite = 0, nleft = nbytes;
    char*  ptr = (char*)buf;

    while (nleft > 0) {
        nwrite = send(fd, ptr, nleft, 0);
        if (nwrite < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;           // error
        } else if (nwrite == 0)  // 对方已关闭连接 ???
            break;

        nleft -= nwrite;
        ptr += nwrite;
    }

    return (nbytes - nleft);
}

// 接收数据的超时版本，强制性的从socket读取nbytes字节数据。
// 返回值：错误返回-1，否则返回实际接收的数据字节数。
int socket_recvo(SOCKET fd, void* buf, int nbytes, int seconds)
{
    int            nread, nleft = nbytes;
    char*          ptr = (char*)buf;
    fd_set         fs;
    int            err;
    struct timeval tv;

    while (nleft > 0) {
        FD_ZERO(&fs);
        FD_SET(fd, &fs);
        tv.tv_sec = seconds, tv.tv_usec = 0;
        err = select(fd + 1, &fs, NULL, NULL, &tv);
        if (err <= 0) {
            return -1;
        }

        nread = recv(fd, ptr, nleft, 0);
        if (nread < 0) {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        } else if (nread == 0)
            break;
        ptr += nread;
        nleft -= nread;
    }

    return (nbytes - nleft);
}

// 接收数据的超时版本，强制性的从socket发送nbytes字节数据。
// 返回值：错误返回-1，否则返回实际发送的数据字节数。
int socket_sendo(SOCKET fd, void* buf, int nbytes, int seconds)
{
    int err;

    err = socket_sendable(fd, seconds);
    if (err < 0)
        return -1;
    else if (err == 0)
        return 0;

    return socket_send(fd, buf, nbytes);
}

// 非强制性接收数据，注意直接调用recv函数接收数据，内核tcp缓存中有多少数据就接收多少数据。
int socket_recvonf(SOCKET fd, void* buf, int nbytes, int seconds)
{
    int err;

    err = socket_recvable(fd, seconds);
    if (err < 0)
        return -1;
    else if (err == 0)
        return 0;

    return recv(fd, buf, nbytes, 0);
}

// 非强制性的发送数据，注意调用send函数发送数据，一次调用能发送多少就是多少。
int socket_sendonf(SOCKET fd, void* buf, size_t nbytes, int seconds)
{
    int err;

    err = socket_sendable(fd, seconds);
    if (err < 0)
        return -1;
    else if (err == 0)
        return 0;

    return send(fd, buf, nbytes, 0);
}

// socket_recv32int: read a 32-bit integer from socket fd,wrapping the htonl and ntohl routine,pInt cannot be NULL
int socket_recv32int(SOCKET fd, int* pInt)
{
    int int32;
    int err;

    if (pInt == NULL)
        return -1;

    if ((err = socket_recv(fd, (char*)&int32, 4)) < 0)
        return err;

    int32 = ntohl(int32);

    *pInt = int32;

    return 4;
}

// socket_send32int
//     write a 32-bit integer into socket fd,wrap the htonl and ntohl routine
//     return <0 if err, otherwise the number of bytes written, that is 4
int socket_send32int(SOCKET fd, int Int)
{
    Int = htonl(Int);

    return socket_send(fd, &Int, 4);
}

int socket_recvline(SOCKET fd, void* buf, int maxlen)
{
    int   n;
    int   rc;
    char  c;
    char* ptr;

    ptr = (char*)buf;
    n   = 0;

    for (n = 1; n < maxlen; n++) {
    again:
        if ((rc = recv(fd, &c, 1, 0)) == 1)  // 注意每次读一个字节效率低
        {
            *ptr++ = c;
            if (c == '\n') /* newline is stored, like fgets() */
                break;
        } else if (rc == 0) {
            if (n == 1)
                return (0);
            else
                break;
        } else {
            if (errno == EINTR)
                goto again;

            return -1;
        }
    }

    *ptr = 0; /* null terminate like fgets() */
    return n;
}

int socket_recvlineo(SOCKET fd, void* buf, int maxlen, int secs)
{
    int   n;
    int   rc;
    char  c;
    char* ptr;

    ptr = (char*)buf;
    for (n = 1; n < maxlen; n++) {
    again:
        if ((rc = socket_recvonf(fd, &c, 1, secs)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (n == 1)
                return (0);
            else
                break;
        } else {
            if ((rc == -1) && (errno == EINTR))
                goto again;
            return rc;
        }
    }
    *ptr = 0;
    return n;
}

// 一次接收一行数据，注意此函数使用了静态局部变量，不能用于多线程环境。
static int recv_line(SOCKET fd, char* ptr)
{
    static int   read_cnt = 0;
    static char* read_ptr;
    static char  read_buf[1024 + 1];

    if (read_cnt <= 0) {
    again:
        if ((read_cnt = recv(fd, read_buf, sizeof(read_buf), 0)) < 0) {
            if (errno == EINTR)
                goto again;

            return (-1);
        } else if (read_cnt == 0)
            return (0);

        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return (1);
}

// 接收一行效率比较高的版本，因为一次recv()不是一个字节，还是1024字节，因此效率高，但不能用于多线程环境。
int socket_recvlineq(SOCKET fd, void* vptr, int maxlen)
{
    int  n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ((rc = recv_line(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break; /* newline is stored, like fgets() */
        } else if (rc == 0) {
            if (n == 1)
                return (0); /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        } else
            return (-1); /* error, errno set by read() */
    }

    *ptr = 0; /* null terminate like fgets() */
    return (n);
}

// Read "n" bytes from a non-block file descriptor.
int socket_recv_nb(SOCKET fd, char* buf, int n)
{
    int   nleft;
    int   nread;
    char* ptr;

    ptr   = buf;
    nleft = n;
    while (nleft > 0) {
        if ((nread = recv(fd, ptr, nleft, 0)) < 0) {
            if (errno == EAGAIN)
                nread = 0; /* and call read() again */
            else
                return (-1);
        } else if (nread == 0)
            break; /* EOF */

        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft); /* return >= 0 */
}

//////////////////////////////////////////////////

int socket_close(SOCKET fd)
{
    return closesocket(fd);
}

// 设置socket为阻塞模式。Return value: 0:success; -1:failed.
int socket_setblock(SOCKET sd)
{
#ifdef _WIN32
    uint32_t rb = 0;
    return ioctlsocket(sd, FIONBIO, &rb);
#else
    int val = 0;
    int flags, flag_t;
    while ((flags = fcntl(sd, F_GETFL, 0)) == -1) {
        if (errno == EINTR)
            continue;
        return -1;
    }
    flag_t = flags;
    if (!(flag_t | ~O_NONBLOCK))
        return 0;
    flags &= ~O_NONBLOCK;
    while ((val = fcntl(sd, F_SETFL, flags)) == -1) {
        if (errno == EINTR)
            continue;
        return -1;
    }
    return val;
#endif
}

// 设置socket为非阻塞模式，Return value: 0:success; -1:failed.
int socket_setnoblock(SOCKET sd)
{
#ifdef _WIN32
    uint32_t rb = 1;
    return ioctlsocket(sd, FIONBIO, &rb);
#else
    int val = 0;
    int flags, flag_t;
    while ((flags = fcntl(sd, F_GETFL, 0)) == -1) {
        if (errno == EINTR)
            continue;

        return -1;
    }
    flag_t = flags;
    if (flag_t & O_NONBLOCK)
        return 0;
    flags |= O_NONBLOCK;
    while ((val = fcntl(sd, F_SETFL, flags)) == -1) {
        if (errno == EINTR)
            continue;

        return -1;
    }
    return val;
#endif
}

////////////////////////////////////////////////
// HTTP
int http_get_response_headero(SOCKET sock, char* headerPtr, int buf_size, int timeout)
{
    fd_set         rfds;
    struct timeval tv;
    int            bytesRead = 0, newlines = 0, ret, selectRet;

    int flags;

#ifdef _WIN32
    uint32_t rb = 1;
    ioctlsocket(sock, FIONBIO, &rb);
#else
    flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }

    // flags|=O_NONBLOCK;
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -1;
    }
#endif

    while (newlines != 2 && bytesRead != buf_size - 1) {
        FD_ZERO(&rfds);
        FD_SET(sock, &rfds);
        tv.tv_sec  = timeout;
        tv.tv_usec = 0;

        if (timeout >= 0)
            selectRet = select(sock + 1, &rfds, NULL, NULL, &tv);
        else /* No timeout, can block indefinately */
            selectRet = select(sock + 1, &rfds, NULL, NULL, NULL);

        if (selectRet == 0 && timeout < 0) {
            return -1;
        } else if (selectRet == -1) {
            return -1;
        }

        ret = recv(sock, headerPtr, 1, 0);
        if (ret == -1) {
            return -1;
        }

        bytesRead++;

        if (*headerPtr == '\r') /* Ignore CR */
        {
            /* Basically do nothing special, just don't set newlines to 0 */
            headerPtr++;
            continue;
        } else if (*headerPtr == '\n') /* LF is the separator */
            newlines++;
        else
            newlines = 0;

        headerPtr++;
    }

    headerPtr -= 2;
    *headerPtr = '\0';

#ifdef _WIN32
    rb = 0;
    ioctlsocket(sock, FIONBIO, &rb);
#else
    fcntl(sock, F_SETFL, flags);
#endif
    return bytesRead;
}

// 读取http响应的头部，并提取"Content-Length"的值，效率非常低下，不采用这个函数。
int http_get_response_header(SOCKET fd, char* hbuf, int maxSize, int* dataLen)
{
    int  rlen = 0;
    char line1[1024 + 1];
    int  err;

    bzero(line1, sizeof(line1));
    err = socket_recvline(fd, hbuf, maxSize - 1);
    if (err <= 0) {
        return err;
    }
    rlen = err;
    while (1) {
        err = socket_recvline(fd, line1, sizeof(line1));
        if (err <= 0) {
            return err;
        }

        if (strncasecmp(line1, "Content-Length:", 15) == 0) {
            *dataLen = atoi(line1 + 15);
            if (*dataLen < 0)
                *dataLen = 0;
        }

        if (strcmp(line1, "\r\n") == 0 || strcmp(line1, "\n") == 0) {
            return rlen;
        }
    }
}
