/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef GKSOCK_H
#define GKSOCK_H

#ifdef WIN32
#include <winsock.h>
#define socklen_t int
#else
#include <netdb.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef int SOCKET;

#define closesocket(_x)   close(_x)
#define ioctlsocket       ioctl
#define WSAGetLastError() errno
#define SOCKET_ERROR      -1
#define INVALID_SOCKET    -1
#ifndef INADDR_NONE
#define INADDR_NONE ((uint32_t)0xffffffff)
#endif
#define WSAETIMEDOUT   ETIMEDOUT
#define WSAEINPROGRESS EINPROGRESS
#endif

#ifdef __cplusplus
extern "C" {
#endif

int  socket_startup(void);
void socket_cleanup(void);

int socket_host2addr(char* host, int port, struct sockaddr_in* sin);
int socket_connect(char* host, int port);
int socket_connect2(struct sockaddr_in* psin);
int socket_connecto(char* host, int port, int sec, int usec);
int socket_connecto2(struct sockaddr_in* psin, int sec, int usec);
int socket_listen(int port, int queue);

int socket_recvable(SOCKET fd, int seconds);
int socket_sendable(SOCKET fd, int seconds);
int socket_recv(SOCKET fd, void* buf, int nbytes);
int socket_send(SOCKET fd, void* buf, int nbytes);
int socket_recvo(SOCKET fd, void* buf, int nbytes, int seconds);
int socket_sendo(SOCKET fd, void* buf, int nbytes, int seconds);
int socket_recvonf(SOCKET fd, void* buf, int nbytes, int seconds);
int socket_sendonf(SOCKET fd, void* buf, size_t nbytes, int seconds);
int socket_recv32int(SOCKET fd, int* pInt);
int socket_send32int(SOCKET fd, int Int);
int socket_recvline(SOCKET fd, void* buf, int maxlen);
int socket_recvlineo(SOCKET fd, void* buf, int maxlen, int secs);
int socket_recvlineq(SOCKET fd, void* vptr, int maxlen);
int socket_recv_nb(SOCKET fd, char* buf, int n);

int socket_close(SOCKET fd);
int socket_setblock(SOCKET sd);
int socket_setnoblock(SOCKET sd);

int http_get_response_headero(SOCKET sock, char* headerPtr, int buf_size, int timeout);
int http_get_response_header(SOCKET fd, char* hbuf, int maxSize, int* dataLen);

#ifdef __cplusplus
}
#endif
#endif
