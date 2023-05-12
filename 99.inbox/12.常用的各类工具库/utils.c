/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "xmalloc.h"
#include "zlib.h"

long getfilesize(FILE* fp)
{
    long size;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

// used for text file. @return: need free.
char* loadfile(const char* path, long* filesize)
{
    FILE* fp;
    char* pstr;
    long  r;
    long  size;

    fp = fopen(path, "rb");
    if (fp == NULL)
        return NULL;

    setvbuf(fp, NULL, _IONBF, 0);

    size = getfilesize(fp);
    if (filesize)
        *filesize = size;

    if (!size) {
        fclose(fp);
        return NULL;
    }

    pstr = (char*)xcalloc(size + 1, sizeof(char));
    r    = fread(pstr, 1, size, fp);
    if (r != size) {
        fclose(fp);
        xfree(pstr);
        return NULL;
    }

    fclose(fp);
    return pstr;
}

//@return: need free.
char* loadfilep(FILE* fp, long* filesize)
{
    char* pstr;
    long  r;
    long  size;

    setvbuf(fp, NULL, _IONBF, 0);

    size = getfilesize(fp);
    if (filesize)
        *filesize = size;

    if (!size) {
        return NULL;
    }

    pstr = (char*)xcalloc(size + 1, sizeof(char));
    r    = fread(pstr, 1, size, fp);
    if (r != size) {
        xfree(pstr);
        return NULL;
    }

    return pstr;
}

// used for bin file. @return: need free.
uint8_t* loadfileb(const char* path, long* filesize)
{
    FILE*    fp;
    uint8_t* pstr;
    long     r;
    long     size;

    fp = fopen(path, "rb");
    if (fp == NULL)
        return NULL;

    setvbuf(fp, NULL, _IONBF, 0);

    size = getfilesize(fp);
    if (filesize)
        *filesize = size;

    if (!size) {
        fclose(fp);
        return NULL;
    }

    pstr = (uint8_t*)xcalloc(size, sizeof(uint8_t));
    r    = fread(pstr, 1, size, fp);
    if (r != size) {
        fclose(fp);
        xfree(pstr);
        return NULL;
    }

    fclose(fp);
    return pstr;
}

int savefile(const char* path, void* buf, int size)
{
    FILE* fp;

    fp = fopen(path, "wb");
    if (!fp)
        return -1;

    fwrite(buf, 1, size, fp);
    fclose(fp);
    return 0;
}

// not support: xx\nyy\nzz\0 = 2 lines
int getlinenum(const char* buf, const char* newline)
{
    const char *p, *q;
    int         num = 0;
    int         len;

    len = strlen(newline);
    p   = buf;
    while ((q = strstr(p, newline)) != NULL) {
        num++;
        p = q;
        p += len;
    }

    // if(*p) num++;
    return num;
}

// @function: support: xx\nyy\nzz\n, not support: xx\nyy\nzz\0,and also modify buffer.
// 注意不支持最后没有\n的行，因为这种情况下让调用程序无法确定下一行的起始位置。
char* getaline(char* buf, const char* newline, int* linelen)
{
    char* p;

    if (*buf == 0)
        return NULL;

    p = strstr(buf, newline);
    if (p == NULL) {
        return NULL;
        //*linelen = strlen(buf);
        // return buf;
    } else {
        *p       = 0;
        *linelen = p - buf;
        return buf;
    }
}

// @function: support: xx\nyy\nzz\n,not support: xx\nyy\nzz\0,and also keep original buffer.
// 注意不支持最后没有\n的行，因为这种情况下让调用程序无法确定下一行的起始位置。
const char* getalinekeep(const char* buf, const char* newline, int* linelen)
{
    char* p;

    if (*buf == 0)
        return NULL;

    p = strstr(buf, newline);
    if (p == NULL) {
        return NULL;
        //*linelen = strlen(buf);
        // return buf;
    } else {
        *linelen = p - buf;
        return buf;
    }
}

int filewriteint(const char* path, int num)
{
    FILE* fp;
    fp = fopen(path, "w");
    if (!fp)
        return -1;
    fprintf(fp, "%d", num);
    fclose(fp);
    return 0;
}

int filereadint(const char* path)
{
    FILE* fp;
    char  str[128];

    if (access(path, F_OK) == -1) {
        return 0;
    }

    fp = fopen(path, "r");
    if (!fp)
        return -1;
    if (!fgets(str, 128, fp))
        return -1;
    fclose(fp);

    return atoi(str);
}

// @return: need free.
uint8_t* compressx(const char* input, int inlen, int* outlen)
{
    Bytef* ziprec;
    uLongf zipreclen;

    if (!input || !inlen || !outlen)
        return NULL;

    zipreclen = (inlen + 12 + sizeof(int)) * 2;
    ziprec    = (Bytef*)xcalloc(1, zipreclen);

    if (compress(ziprec, &zipreclen, (Bytef*)input, inlen) != Z_OK) {
        xfree(ziprec);
        return NULL;
    }

    // 把数据的压缩前的长度保存在压缩数据的最后四个字节
    memcpy(ziprec + zipreclen, &inlen, sizeof(int));
    *outlen = (int)zipreclen + sizeof(int);
    return ziprec;
}

// @return: need free.
char* uncompressx(const char* input, int inlen, int* outlen)
{
    Bytef* unziprec;
    uLongf unzipreclen;
    int    orilen;

    if (!input || !inlen || !outlen || inlen <= 4)
        return NULL;

    // 数据压缩前的原始长度保存在压缩数据最后的4个字节
    memcpy(&orilen, input + inlen - sizeof(int), sizeof(int));
    unziprec    = (Bytef*)xcalloc(1, orilen + 1);
    unzipreclen = orilen + 1;
    if (uncompress(unziprec, &unzipreclen, (Bytef*)input, inlen - sizeof(int)) != Z_OK) {
        xfree(unziprec);
        return NULL;
    }

    *outlen               = unzipreclen;
    unziprec[unzipreclen] = 0;
    return (char*)unziprec;
}

int compressfile(const char* infilename, const char* outfilename, int chunksize)
{
    char*    inbuffer;
    int      num_read;
    uint32_t total_read;
    FILE*    infile;
    gzFile   outfile;

    if (chunksize < 4096)
        chunksize = 4096;

    infile  = fopen(infilename, "rb");
    outfile = gzopen(outfilename, "wb");
    if (!infile || !outfile)
        return -1;
    setvbuf(infile, NULL, _IONBF, 0);

    num_read   = 0;
    total_read = 0;
    inbuffer   = (char*)xcalloc(1, chunksize);
    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
        total_read += num_read;
        gzwrite(outfile, inbuffer, num_read);
    }

    xfree(inbuffer);
    fclose(infile);
    gzclose(outfile);

    return 0;
}

int decompressfile(const char* infilename, const char* outfilename, int chunksize)
{
    char*  buffer;
    int    num_read;
    gzFile infile;
    FILE*  outfile;

    if (chunksize < 4096)
        chunksize = 4096;

    infile  = gzopen(infilename, "rb");
    outfile = fopen(outfilename, "wb");
    if (!infile || !outfile)
        return -1;
    setvbuf(outfile, NULL, _IONBF, 0);

    num_read = 0;
    buffer   = (char*)xcalloc(1, chunksize);
    while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, num_read, outfile);
    }

    xfree(buffer);
    gzclose(infile);
    fclose(outfile);
    return 0;
}

// inflate4gzip: used for http gzip content decompress,return value need free.
#define INF_CHUNK 262144  // 256k

char* inflate4gzip(char* in, int inlen, int* outlen)
{
    int      ret;
    uint32_t have;
    z_stream strm;
    uint8_t  out[INF_CHUNK];
    char*    outbuf    = NULL;
    int      totalsize = 0;

    if (!inlen) return NULL;

    /*  allocate  inflate  state  */
    strm.zalloc   = Z_NULL;
    strm.zfree    = Z_NULL;
    strm.opaque   = Z_NULL;
    strm.avail_in = 0;
    strm.next_in  = Z_NULL;
    ret           = inflateInit2(&strm, 47);
    if (ret != Z_OK)
        return NULL;

    strm.avail_in = inlen;
    strm.next_in  = (Bytef*)in;
    /*  run inflate() on input until output buffer not full  */
    do {
        strm.avail_out = INF_CHUNK;
        strm.next_out  = out;
        ret            = inflate(&strm, Z_NO_FLUSH);
        assert(ret != Z_STREAM_ERROR); /* state not clobbered  */
        switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR; /*  and  fall  through  */
            case Z_DATA_ERROR:      /* Z_DATA_ERROR: the deflate data is invalid or incomplete */
            case Z_MEM_ERROR:       /* Z_MEM_ERROR: memory could not be allocated for processing */
                (void)inflateEnd(&strm);
                if (outbuf) xfree(outbuf);
                return NULL;
        }

        have = INF_CHUNK - strm.avail_out;
        totalsize += have;
        outbuf = (char*)xrealloc(outbuf, totalsize + 1);
        memcpy(outbuf + totalsize - have, out, have);
    } while (strm.avail_out == 0);

    /*  clean  up  and  return  */
    (void)inflateEnd(&strm);
    if (ret == Z_STREAM_END) {
        *outlen         = totalsize;
        outbuf[*outlen] = 0;
        return outbuf;  // Z_OK
    } else {
        if (outbuf) xfree(outbuf);
        return NULL;  // Z_DATA_ERROR;
    }
}

// 对字母、数字、'_' '-'之外的符号进行十六进制编码，注意空格用'+'代替。
// 一般用于application/x-www-form-urlencoded表单对应URL的query string的name value部分。
char* urlencode(char* source, char* dest)
{
    static char* hex = "0123456789abcdef";
    int          len = strlen(source);
    uint8_t      ch;
    int          pos = 0;
    char*        ret = dest;

    while (pos < len) {
        ch = (uint8_t)source[pos];
        if (ch == ' ')
            *ret++ = '+';
        else if (isalnum(ch) || ch == '_' || ch == '-')  // '/'
            *ret++ = ch;
        else {
            *ret++ = '%';
            *ret++ = hex[ch / 16];
            *ret++ = hex[ch % 16];
        }
        pos++;
    }
    *ret = '\0';
    return (ret = dest);
}

/* 把字符串中的"%HH"编码转换为字符。
   注意：
     加号'+'用空格代替。
     如果%后没有两个十六进制数，则不转换。
     该函数会修改字符串S，调用该函数时注意根据需要备份原字符串。
     一般用于application/x-www-form-urlencoded表单对应URL的query string的name value部分。
*/
#define ISXDIGIT(x)             isxdigit(x)
#define TOUPPER(x)              toupper(x)

#define XDIGIT_TO_NUM(h)        ((h) < 'A' ? (h) - '0' : TOUPPER(h) - 'A' + 10)
#define X2DIGITS_TO_NUM(h1, h2) ((XDIGIT_TO_NUM(h1) << 4) + XDIGIT_TO_NUM(h2))

char* urldecode(char* s)
{
    char* t = s;
    char* h = s;
    int   x;

    for (x = 0; s[x]; x++)
        if (s[x] == '+') s[x] = ' ';

    for (; *h; h++, t++) {
        if (*h != '%') {
        copychar:
            *t = *h;
        } else {
            char c;
            if (!h[1] || !h[2] || !(ISXDIGIT(h[1]) && ISXDIGIT(h[2])))
                goto copychar;
            c = X2DIGITS_TO_NUM(h[1], h[2]);
            if (c == '\0')
                goto copychar;
            *t = c;
            h += 2;
        }
    }
    *t = '\0';

    return s;
}

// html encode & decode ... 注意：Entity是区分大小写的。
static const char* escapeChars[]    = {"gt;", "lt;", "amp;", "quot;", "#39;", "nbsp;",
                                       "#62;", "#60;", "#38;", "#34;", "apos;", "#160;",
                                       "#062;", "#060;", "#038;", "#034;", "#039;", NULL};  // IE不支持&apos;
static const int   escapeCharsLen[] = {3, 3, 4, 5, 4, 5,
                                       4, 4, 4, 4, 5, 5,
                                       5, 5, 5, 5, 5};
static const char  escapeChar[]     = "><&\"' ><&\"' ><&\"'";
static const char  xml_escapeChar[] = "><&";

// xml中不允许出现的特殊字符: < &  可以出现的字符: > ' "
int xmlencode(const char* src, char* des)
{
    const char* p = src;
    char*       q = des;
    char*       pos;
    int         id;

    while (*p) {
        if (*p <= xml_escapeChar[0]) {
            pos = strchr(xml_escapeChar, *p);
            if (pos) {
                id   = pos - xml_escapeChar;
                *q++ = '&';
                memcpy(q, escapeChars[id], escapeCharsLen[id]);
                q += escapeCharsLen[id];
                p++;
                continue;
            }
        }

        *q++ = *p++;
    }

    *q = '\0';
    return q - des;
}

// 注意不会对空格编码为&nbsp;
int htmlencode(const char* src, char* des)
{
    const char* p = src;
    char*       q = des;
    char*       pos;
    int         id;

    while (*p) {
        if (*p <= escapeChar[0] && *p != ' ') {
            pos = strchr(escapeChar, *p);
            if (pos) {
                id   = pos - escapeChar;
                *q++ = '&';
                memcpy(q, escapeChars[id], escapeCharsLen[id]);
                q += escapeCharsLen[id];
                p++;
                continue;
            }
        }
        *q++ = *p++;
    }
    *q = '\0';
    return q - des;
}

// 注意不会对空格编码为&nbsp; 指定<tag></tag>不会被encode 比如 <em> </em>
int htmlencode2(const char* src, char* des, const char* tag)
{
    const char* p = src;
    char*       q = des;
    char*       pos;
    int         id, blen, elen;
    char        btag[32], etag[32];

    blen = snprintf(btag, sizeof(btag), "<%s>", tag);
    elen = snprintf(etag, sizeof(etag), "</%s>", tag);

    while (*p) {
        if (*p <= escapeChar[0] && *p != ' ') {
            if (*p == '<' && strncmp(p, btag, blen) == 0) {
                memcpy(q, btag, blen);
                q += blen;
                p += blen;
                continue;
            }

            if (*p == '<' && strncmp(p, etag, elen) == 0) {
                memcpy(q, etag, elen);
                q += elen;
                p += elen;
                continue;
            }

            pos = strchr(escapeChar, *p);
            if (pos) {
                id   = pos - escapeChar;
                *q++ = '&';
                memcpy(q, escapeChars[id], escapeCharsLen[id]);
                q += escapeCharsLen[id];
                p++;
                continue;
            }
        }
        *q++ = *p++;
    }
    *q = '\0';
    return q - des;
}

// 注意不会对空格编码为&nbsp;
int htmlencoden(const char* src, char* des, int desSize)
{
    const char* p     = src;
    char*       q     = des;
    int         nleft = desSize - 1;
    char*       pos;
    int         id;

    while (*p) {
        if (*p <= escapeChar[0] && *p != ' ') {
            pos = strchr(escapeChar, *p);
            if (pos) {
                id = pos - escapeChar;
                if (nleft < escapeCharsLen[id] + 1)
                    return -1;
                *q++ = '&';
                memcpy(q, escapeChars[id], escapeCharsLen[id]);
                q += escapeCharsLen[id];
                nleft -= escapeCharsLen[id];
                p++;
                continue;
            }
        }
        if (nleft < 1)
            return -1;
        *q++ = *p++;
        nleft--;
    }
    *q = '\0';
    return q - des;
}

// 注意会对&nbsp; &#160;解码
int htmldecode(char* des)
{
    char* begin = des;
    char* p     = strchr(begin, '&');
    char* q;
    int   i;

    // no need to decoding
    if (p == NULL)
        return -1;

    q = p;
    while (p != NULL) {
        char ch = '\0';

        for (i = 0; escapeChars[i] != NULL; i++) {
            if (!strncmp(p + 1, escapeChars[i], escapeCharsLen[i])) {
                ch = escapeChar[i];
                break;
            }
        }

        if (ch) {
            *q++  = ch;
            begin = p + escapeCharsLen[i] + 1;
        } else {
            *q++  = *p;
            begin = p + 1;
        }

        p = strchr(begin, '&');
        if (p == NULL)
            break;
        memcpy(q, begin, p - begin);
        q += p - begin;
    }

    while (*begin)
        *q++ = *begin++;
    *q = '\0';
    return q - des;
}

// 注意会对&nbsp; &#160;解码
int htmldecoded(const char* src, char* des)
{
    const char* begin = src;
    const char* p     = strchr(begin, '&');
    char*       q     = des;
    char        ch;
    int         i;

    while (p != NULL) {
        memcpy(q, begin, p - begin);
        q += p - begin;

        ch = '\0';
        for (i = 0; escapeChars[i] != NULL; i++) {
            if (!strncmp(p + 1, escapeChars[i], escapeCharsLen[i])) {
                ch = escapeChar[i];
                break;
            }
        }

        if (ch) {
            *q++  = ch;
            begin = p + escapeCharsLen[i] + 1;
        } else {
            *q++  = *p;
            begin = p + 1;
        }

        p = strchr(begin, '&');
    }

    while (*begin)
        *q++ = *begin++;
    *q = '\0';
    return q - des;
}

/* Encode the string STR of length LENGTH to base64 format and place it
   to B64STORE.  The output will be \0-terminated, and must point to a
   writable buffer of at least 1+BASE64_LENGTH(length) bytes.  It
   returns the length of the resulting base64 data, not counting the
   terminating zero.

   This implementation will not emit newlines after 76 characters of
   base64 data.  */

int base64encode(const char* str, int length, char* b64store)
{
    /* Conversion table.  */
    static char tbl[64] =
        {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', '+', '/'};
    int            i;
    const uint8_t* s = (const uint8_t*)str;
    char*          p = b64store;

    /* Transform the 3x8 bits to 4x6 bits, as required by base64.  */
    for (i = 0; i < length; i += 3) {
        *p++ = tbl[s[0] >> 2];
        *p++ = tbl[((s[0] & 3) << 4) + (s[1] >> 4)];
        *p++ = tbl[((s[1] & 0xf) << 2) + (s[2] >> 6)];
        *p++ = tbl[s[2] & 0x3f];
        s += 3;
    }

    /* Pad the result if necessary...  */
    if (i == length + 1)
        *(p - 1) = '=';
    else if (i == length + 2)
        *(p - 1) = *(p - 2) = '=';

    /* ...and zero-terminate it.  */
    *p = '\0';

    return p - b64store;
}

// +/= --> *!.
int base64encode4url(const char* str, int length, char* b64store)
{
    /* Conversion table.  */
    static char tbl[64] =
        {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', '*', '!'  //'+' '/'
        };
    int            i;
    const uint8_t* s = (const uint8_t*)str;
    char*          p = b64store;

    /* Transform the 3x8 bits to 4x6 bits, as required by base64.  */
    for (i = 0; i < length; i += 3) {
        *p++ = tbl[s[0] >> 2];
        *p++ = tbl[((s[0] & 3) << 4) + (s[1] >> 4)];
        *p++ = tbl[((s[1] & 0xf) << 2) + (s[2] >> 6)];
        *p++ = tbl[s[2] & 0x3f];
        s += 3;
    }

    /* Pad the result if necessary...  */
    if (i == length + 1)
        *(p - 1) = '.';
    else if (i == length + 2)
        *(p - 1) = *(p - 2) = '.';

    /* ...and zero-terminate it.  */
    *p = '\0';

    return p - b64store;
}

#define IS_ASCII(c)  (((c)&0x80) == 0)
#define IS_BASE64(c) ((IS_ASCII(c) && base64_char_to_value[c] >= 0) || c == '=')

/* Get next character from the string, except that non-base64
   characters are ignored, as mandated by rfc2045.  */
#define NEXT_BASE64_CHAR(c, p) \
    do {                       \
        c = *p++;              \
    } while (c != '\0' && !IS_BASE64(c))

/* Decode data from BASE64 (assumed to be encoded as base64) into
   memory pointed to by TO.  TO should be large enough to accomodate
   the decoded data, which is guaranteed to be less than
   strlen(base64).

   Since TO is assumed to contain binary data, it is not
   NUL-terminated.  The function returns the length of the data
   written to TO.  -1 is returned in case of error caused by malformed
   base64 input.  */

int base64decode(const char* base64, char* to)
{
    /* Table of base64 values for first 128 characters.  Note that this
       assumes ASCII (but so does Wget in other places).  */
    static short base64_char_to_value[128] =
        {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /*   0-  9 */
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /*  10- 19 */
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /*  20- 29 */
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /*  30- 39 */
            -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, /*  40- 49 */
            54, 55, 56, 57, 58, 59, 60, 61, -1, -1, /*  50- 59 */
            -1, -1, -1, -1, -1, 0, 1, 2, 3, 4,      /*  60- 69 */
            5, 6, 7, 8, 9, 10, 11, 12, 13, 14,      /*  70- 79 */
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, /*  80- 89 */
            25, -1, -1, -1, -1, -1, -1, 26, 27, 28, /*  90- 99 */
            29, 30, 31, 32, 33, 34, 35, 36, 37, 38, /* 100-109 */
            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, /* 110-119 */
            49, 50, 51, -1, -1, -1, -1, -1          /* 120-127 */
        };

    const char* p = base64;
    char*       q = to;

    while (1) {
        uint8_t  c;
        uint32_t value;

        /* Process first byte of a quadruplet.  */
        NEXT_BASE64_CHAR(c, p);
        if (!c)
            break;
        if (c == '=')
            return -1; /* illegal '=' while decoding base64 */
        value = base64_char_to_value[c] << 18;

        /* Process scond byte of a quadruplet.  */
        NEXT_BASE64_CHAR(c, p);
        if (!c)
            return -1; /* premature EOF while decoding base64 */
        if (c == '=')
            return -1; /* illegal `=' while decoding base64 */
        value |= base64_char_to_value[c] << 12;
        *q++ = value >> 16;

        /* Process third byte of a quadruplet.  */
        NEXT_BASE64_CHAR(c, p);
        if (!c)
            return -1; /* premature EOF while decoding base64 */

        if (c == '=') {
            NEXT_BASE64_CHAR(c, p);
            if (!c)
                return -1; /* premature EOF while decoding base64 */
            if (c != '=')
                return -1; /* padding `=' expected but not found */
            continue;
        }

        value |= base64_char_to_value[c] << 6;
        *q++ = 0xff & value >> 8;

        /* Process fourth byte of a quadruplet.  */
        NEXT_BASE64_CHAR(c, p);
        if (!c)
            return -1; /* premature EOF while decoding base64 */
        if (c == '=')
            continue;

        value |= base64_char_to_value[c];
        *q++ = 0xff & value;
    }

    return q - to;
}

#define IS_BASE64_URL(c) ((IS_ASCII(c) && base64_char_to_value[c] >= 0) || c == '.')

/* Get next character from the string, except that non-base64
   characters are ignored, as mandated by rfc2045.  */
#define NEXT_BASE64_CHAR_URL(c, p) \
    do {                           \
        c = *p++;                  \
    } while (c != '\0' && !IS_BASE64_URL(c))

int base64decode4url(const char* base64, char* to)
{
    /* Table of base64 values for first 128 characters.  Note that this
       assumes ASCII (but so does Wget in other places).  */
    static short base64_char_to_value[128] =
        {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /*   0-  9 */
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /*  10- 19 */
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /*  20- 29 */
            -1, -1, -1, 63, -1, -1, -1, -1, -1, -1, /*  30- 39 */
            -1, -1, 62, -1, -1, -1, -1, -1, 52, 53, /*  40- 49 */
            54, 55, 56, 57, 58, 59, 60, 61, -1, -1, /*  50- 59 */
            -1, -1, -1, -1, -1, 0, 1, 2, 3, 4,      /*  60- 69 */
            5, 6, 7, 8, 9, 10, 11, 12, 13, 14,      /*  70- 79 */
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, /*  80- 89 */
            25, -1, -1, -1, -1, -1, -1, 26, 27, 28, /*  90- 99 */
            29, 30, 31, 32, 33, 34, 35, 36, 37, 38, /* 100-109 */
            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, /* 110-119 */
            49, 50, 51, -1, -1, -1, -1, -1          /* 120-127 */
        };

    const char* p = base64;
    char*       q = to;

    while (1) {
        uint8_t  c;
        uint32_t value;

        /* Process first byte of a quadruplet.  */
        NEXT_BASE64_CHAR_URL(c, p);
        if (!c)
            break;
        if (c == '.')
            return -1; /* illegal '=' while decoding base64 */
        value = base64_char_to_value[c] << 18;

        /* Process scond byte of a quadruplet.  */
        NEXT_BASE64_CHAR_URL(c, p);
        if (!c)
            return -1; /* premature EOF while decoding base64 */
        if (c == '.')
            return -1; /* illegal `=' while decoding base64 */
        value |= base64_char_to_value[c] << 12;
        *q++ = value >> 16;

        /* Process third byte of a quadruplet.  */
        NEXT_BASE64_CHAR_URL(c, p);
        if (!c)
            return -1; /* premature EOF while decoding base64 */

        if (c == '.') {
            NEXT_BASE64_CHAR_URL(c, p);
            if (!c)
                return -1; /* premature EOF while decoding base64 */
            if (c != '.')
                return -1; /* padding `=' expected but not found */
            continue;
        }

        value |= base64_char_to_value[c] << 6;
        *q++ = 0xff & value >> 8;

        /* Process fourth byte of a quadruplet.  */
        NEXT_BASE64_CHAR_URL(c, p);
        if (!c)
            return -1; /* premature EOF while decoding base64 */
        if (c == '.')
            continue;

        value |= base64_char_to_value[c];
        *q++ = 0xff & value;
    }

    return q - to;
}

/*
 * Note that we cannot use the BASE64 alphabet here, because
 * it contains "/" and "+": both are unacceptable for simple inclusion
 * into URLs.
 */

static char hexconvtab[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ*!";

// nbits: 指明多少位对应一个字符，nbits取值为4,5,6
char* binencode(char* in, int inlen, char* out, char nbits)
{
    uint8_t *p, *q;
    uint16_t w;
    int      mask;
    int      have;

    if (nbits != 4 && nbits != 5 && nbits != 6)
        return NULL;

    p = (uint8_t*)in;
    q = (uint8_t*)in + inlen;

    w    = 0;
    have = 0;
    mask = (1 << nbits) - 1;

    while (1) {
        if (have < nbits) {
            if (p < q) {
                w |= *p++ << have;
                have += 8;
            } else {
                /* consumed everything? */
                if (have == 0) break;
                /* No? We need a final round */
                have = nbits;
            }
        }

        /* consume nbits */
        *out++ = hexconvtab[w & mask];
        w >>= nbits;
        have -= nbits;
    }

    *out = '\0';
    return out;
}

/* Convert a number in the [0, 16) range to
   the ASCII representation of the corresponding hexadecimal digit.
   `+ 0' is there so you can't accidentally use it as an lvalue.  */
#define XNUM_TO_DIGIT(x) ("0123456789ABCDEF"[x] + 0)
#define XNUM_TO_digit(x) ("0123456789abcdef"[x] + 0)

char* bin2hex(uint8_t* bin, int binlen, char* hex)
{
    int i;

    for (i = 0; i < binlen; i++) {
        *hex = XNUM_TO_digit(bin[i] >> 4);
        hex++;
        *hex = XNUM_TO_digit(bin[i] & 0xf);
        hex++;
    }

    *hex = '\0';
    return hex;
}

int hex2bin(char* hex, uint8_t* bin)
{
    int i;

    i = 0;
    while (*hex) {
        if (!hex[1] || !(ISXDIGIT(hex[0]) && ISXDIGIT(hex[1])))
            return 0;

        bin[i] = X2DIGITS_TO_NUM(hex[0], hex[1]);
        hex += 2;
        i++;
    }

    return i;
}

// 对字母、数字、'_' '-'之外的符号进行十六进制编码。
char* hexencode(char* source, char* dest)
{
    char*   hex = "0123456789abcdef";
    int     len = strlen(source);
    uint8_t ch;
    int     pos = 0;
    char*   ret = dest;

    while (pos < len) {
        ch = (uint8_t)source[pos];
        if (isalnum(ch) || ch == '_' || ch == '-')
            *ret++ = ch;
        else {
            *ret++ = '%';
            *ret++ = hex[ch / 16];
            *ret++ = hex[ch % 16];
        }
        pos++;
    }
    *ret = '\0';
    return (ret = dest);
}

// 对字符串中%hh编码进行解码。
char* hexdecode(char* s)
{
    char* t = s;
    char* h = s;

    for (; *h; h++, t++) {
        if (*h != '%') {
        copychar:
            *t = *h;
        } else {
            char c;
            if (!h[1] || !h[2] || !(ISXDIGIT(h[1]) && ISXDIGIT(h[2])))
                goto copychar;
            c = X2DIGITS_TO_NUM(h[1], h[2]);
            if (c == '\0')
                goto copychar;
            *t = c;
            h += 2;
        }
    }
    *t = '\0';

    return s;
}

void format_thousands_separator(long val, char* buf, int nLen)
{
    char* firstdig;
    long  m, n = 0;
    char* p = &buf[nLen - 1];
    *p      = '\0';

    // from back to front
    do {
        m    = val % 10;
        val  = val / 10;
        *--p = '0' + (m < 0 ? -m : m);

        if (!val && m < 0)
            *--p = '-';

        if (val && !(++n % 3))
            *--p = ',';

    } while (val);

    firstdig = buf;
    do {
        *firstdig++ = *p++;
    } while (*p);

    *firstdig = '\0';
}

#ifdef TEST_UTILS

int main()
{
    uint8_t *uc, ub[1024];
    char *   p, *o;
    int      len, inlen;
    char     buf[1024];
    char     r[1024];

    urlencode("/url-en_code ", buf);
    printf("%s\n", buf);
    urldecode(buf);
    printf("%s\n", buf);

    p = loadfile("test.inf", (long*)&inlen);
    if (!p) {
        printf("load test.inf failure.\n");
        return -1;
    }
    o = inflate4gzip(p, inlen, &len);
    savefile("test.inf.ok", o, len);
    xfree(p);
    xfree(o);

    inlen = strlen("liuxingzhi,hello,world.");
    uc    = compressx("liuxingzhi,hello,world.", inlen, &len);
    printf("ratio: %d/%d\n", inlen, len);
    bin2hex(uc, len, buf);
    printf("%s\n", buf);
    hex2bin(buf, ub);
    p = uncompressx(ub, len, &len);
    printf("%s\n", p);
    xfree(uc);
    xfree(p);

    format_thousands_separator(101, r, 1024);
    printf("%s\n", r);
    format_thousands_separator(1124, r, 1024);
    printf("%s\n", r);
    format_thousands_separator(99100, r, 1024);
    printf("%s\n", r);
    format_thousands_separator(-888100, r, 1024);
    printf("%s\n", r);
    format_thousands_separator(-12345100, r, 1024);
    printf("%s\n", r);
    format_thousands_separator(-1324234320, r, 1024);
    printf("%s\n", r);

    return 0;
}

#endif
