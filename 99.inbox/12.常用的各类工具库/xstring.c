/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "xmalloc.h"
#include "charset.h"

// 将'+'转成' '
void strplustospace(char* str)
{
    register int x;
    for (x = 0; str[x]; x++)
        if (str[x] == '+') str[x] = ' ';
}

// 将' '转成'+'
void strspacetoplus(char* str)
{
    register int x;
    for (x = 0; str[x]; x++)
        if (str[x] == ' ') str[x] = '+';
}

char* skipwhitespace(char* str)
{
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')
        str++;

    return str;
}

// 删除字符串首尾处的空白字符 ASCII(0,32],包括\n,\t,\r,空格...
// 注意: @str会被修改。
void trim(char* str)
{
    int   pos  = 0;
    char* dest = str;

    /* skip leading blanks */
    while (str[pos] <= ' ' && str[pos] > 0) {
        pos++;
    }

    while (str[pos]) {
        *(dest++) = str[pos];
        pos++;
    }

    *(dest--) = '\0'; /* store the null */

    /* remove trailing blanks */
    while (dest >= str && *dest <= ' ' && *dest > 0) {
        *(dest--) = '\0';
    }
}

// 去掉字符串中间的空格，只保留一个空格，@trans指明是否转换中文空格。
// 用于GBK
int trimid_gbk(const char* in, char* out, int trans)
{
    int flag;  // 为1:表示该output buffer最后一个字节是空格字符
    int i, pos, inlen;

    flag  = 1;
    pos   = 0;
    inlen = strlen(in);

    for (i = 0; i < inlen; i++) {
        /* filter whitespace char */
        if (*(in + i) < ' ' && *(in + i) >= 0)
            continue;

        /* blank  */
        if (*(in + i) == ' ') {
            if (flag == 1)
                continue;

            *(out + pos) = ' ';
            pos++;
            flag = 1;
            continue;
        }

        // 非空白ascii字符
        if (*((uint8_t*)(in + i)) < 0x80) {
            *(out + pos) = *(in + i);
            pos++;
            flag = 0;
            continue;
        } else  // double byte char
        {
            if (i + 1 < inlen)  // 防止*(src_beg+i+1)= '<'
            {
                if (trans == 1 && *(in + i) == '\xa1' && *(in + i + 1) == '\xa1') {
                    if (flag == 1) {
                        i++;
                        continue;
                    } else {
                        *(out + pos) = ' ';
                        pos++;
                        flag = 1;
                        i++;
                        continue;
                    }
                } else {
                    *(out + pos) = *(in + i);
                    pos++;
                    *(out + pos) = *(in + i + 1);
                    pos++;
                    flag = 0;
                    i++;
                    continue;
                }
            } else  // this double char only has a byte
            {
                break;
            }
        }
    }

    if (*(out + pos - 1) == ' ') {
        *(out + pos - 1) = 0;
        return (pos - 1);
    } else {
        *(out + pos) = 0;
        return pos;
    }
}

// 去掉字符串中间的空格，只保留一个空格，@trans指明是否转换中文空格。用于utf-8
int trimid_utf8(const char* in, char* out, int trans)
{
    int flag;  // 为1:表示该output buffer最后一个字节是空格字符
    int i, pos, inlen, clen;

    flag  = 1;
    pos   = 0;
    inlen = strlen(in);

    for (i = 0; i < inlen; i++) {
        if (*(in + i) >= 0 && *(in + i) < ' ') /* filter whitespace char */
            continue;

        if (*(in + i) == ' ') { /* blank  */
            if (flag == 1) continue;
            *(out + pos) = ' ';
            pos++;
            flag = 1;
            continue;
        }

        if (*((uint8_t*)(in + i)) < 0x80) {  // 非空白ascii字符
            *(out + pos) = *(in + i);
            pos++;
            flag = 0;
            continue;
        } else {  // 非ascii字符
            clen = UTF8CHLEN(*((uint8_t*)(in + i)));
            if (i + clen - 1 < inlen) {
                if (trans == 1 && *(in + i) == '\xe3' && *(in + i + 1) == '\x80' && *(in + i + 2) == '\x80') {
                    if (flag == 1) {
                        i += (clen - 1);
                        continue;
                    } else {
                        *(out + pos) = ' ';
                        pos++;
                        flag = 1;
                        i += (clen - 1);
                        continue;
                    }
                } else {
                    memcpy(out + pos, in + i, clen);
                    pos += clen;
                    flag = 0;
                    i += (clen - 1);
                    continue;
                }
            } else {
                break;
            }
        }
    }

    if (*(out + pos - 1) == ' ') {
        *(out + pos - 1) = 0;
        return (pos - 1);
    } else {
        *(out + pos) = 0;
        return pos;
    }
}

// Squeeze(紧缩) space characters in a string and trim it.
// 即去掉字符串中间的空格，只保留一个空格
char* strsqueeze(char* str)
{
    char* rp;
    char* wp;
    int   spc;

    if (!str) return NULL;
    rp  = str;
    wp  = str;
    spc = 1;

    while (*rp != '\0') {
        if (*rp > 0 && *rp <= ' ') {
            if (!spc) *(wp++) = *rp;
            spc = 1;
        } else {
            *(wp++) = *rp;
            spc     = 0;
        }
        rp++;
    }

    *wp = '\0';
    for (wp--; wp >= str; wp--) {
        if (*wp > 0 && *wp <= ' ') {
            *wp = '\0';
        } else {
            break;
        }
    }

    return str;
}

int strfragnum(const char* str, const char* delimiter)
{
    const char *p, *q;
    int         num = 0;
    int         len;

    len = strlen(delimiter);
    p   = str;
    while ((q = strstr(p, delimiter)) != NULL) {
        num++;
        p = q;
        p += len;
    }

    if (*p)
        num++;
    return num;
}

// 切分字符串，@str会被修改。
int split(char* str, const char* delimiter, char* fragment[], int frags)
{
    int   i, len;
    char* tmp = NULL;
    char* brk = NULL;

    if (!str || !*str || !delimiter || !*delimiter)
        return -1;

    len = strlen(delimiter);
    tmp = str;
    for (i = 0; i < frags; i++) {
        brk         = strstr(tmp, delimiter);
        fragment[i] = tmp;
        if (!brk) break;
        *brk = '\0';
        tmp  = brk + len;
    }

    if (i >= frags)
        return frags;
    else
        return i + 1;
}

int strgetfrags(const char* str, const char* delimiter, char*** frags)
{
    int   i, num;
    char *p, **pp;

    num = strfragnum(str, delimiter);
    if (!num)
        return 0;

    p  = xstrdup(str);
    pp = (char**)xcalloc(num, sizeof(char*));
    split(p, delimiter, pp, num);
    *frags = (char**)xcalloc(num, sizeof(char*));

    for (i = 0; i < num; i++) {
        trim(pp[i]);
        (*frags)[i] = xstrdup(pp[i]);
    }

    xfree(p);
    xfree(pp);
    return num;
}

char* stristr(const char* StrBase, const char* SubBase)
{
    while (*StrBase) {
        if (toupper(*StrBase) == toupper(*SubBase)) {
            const char *Str, *Sub;
            Str = StrBase + 1;
            Sub = SubBase + 1;
            while (*Sub && toupper(*Sub) == toupper(*Str)) {
                Sub++;
                Str++;
            }
            if (!*Sub) return ((char*)StrBase);
        }
        StrBase++;
    }

    return (NULL);
}

char* stristrb(const char* StrBase, int str_len, const char* SubBase, int sub_len)
{
    const char* str_end;
    const char* sub_end;

    if (!StrBase || !str_len || !SubBase || !sub_len)
        return NULL;

    str_end = StrBase + str_len;
    sub_end = SubBase + sub_len;

    while (StrBase != str_end) {
        if (toupper(*StrBase) == toupper(*SubBase)) {
            const char *Str, *Sub;
            Str = StrBase + 1;
            Sub = SubBase + 1;
            while (Sub != sub_end && toupper(*Sub) == toupper(*Str)) {
                Sub++;
                Str++;
            }
            if (Sub == sub_end) return ((char*)StrBase);
        }
        StrBase++;
    }

    return (NULL);
}

void strcharrep(char* str, char fm, char to)
{
    while (*str) {
        if (*str == fm) *str = to;
        str++;
    }
}

// 字符串替换: 支持多个替换、通配符、GBK和UTF-8编码。注意前缀相同的字符串中，长的放在前面。
// @return: need free.
char* strreplace(const char* str, int slen, char* old[], char* new[], int num, int codetype)
{
    int         i, j, len;
    const char *cur, *end;

    struct rep_mem {
        char* p;
        int   size;
        int   free;
        int   pos;
    } news;

    news.p    = (char*)xcalloc(1, slen + 1024 + 1);
    news.size = slen + 1024 + 1;
    news.free = slen + 1024;
    news.pos  = 0;

    cur       = str;
    end       = cur + slen;
    i         = 0;  // i->old[]
    j         = 0;  // j->old[][]

init:
    while (cur < end) {
        for (i = 0; i < num; i++) {
            const char* poo = cur;
            while (*cur == *(*(old + i) + j) || (cur != end && *(*(old + i) + j) == '(' && *(*(old + i) + j + 1) == '*' && *(*(old + i) + j + 2) == ')')) {
                if (*(*(old + i) + j) == '(' && *(*(old + i) + j + 1) == '*' && *(*(old + i) + j + 2) == ')') {
                    if (!*(*(old + i) + j + 3)) {
                        goto _end;
                    }
                    if (*cur == *(*(old + i) + j + 3)) {
                        if (!*(*(old + i) + j + 3 + 1)) {
                            len = strlen(*(new + i));
                            if (news.free < len) {
                                news.p = (char*)xrealloc(news.p, news.size + len + 1024);
                                news.size += (len + 1024);
                                news.free += (len + 1024);
                            }
                            memcpy(news.p + news.pos, *(new + i), len);
                            news.pos += len;
                            news.free -= len;

                            cur++;
                            j = 0;
                            goto init;
                        }
                        cur++;
                        j += 4;
                    } else {
                        cur++;
                    }

                } else {
                    if (!*(*(old + i) + j + 1)) {
                        len = strlen(*(new + i));
                        if (news.free < len) {
                            news.p = (char*)xrealloc(news.p, news.size + len + 1024);
                            news.size += (len + 1024);
                            news.free += (len + 1024);
                        }
                        memcpy(news.p + news.pos, *(new + i), len);
                        news.pos += len;
                        news.free -= len;

                        cur++;
                        j = 0;
                        goto init;
                    }
                    cur++;
                    j++;
                }
            }  // end while

            cur = poo;
            j   = 0;
        }  // end for

        if (news.free < 16) {
            news.p = (char*)xrealloc(news.p, news.size + 512);
            news.size += 512;
            news.free += 512;
        }

        if (*((uint8_t*)cur) < 0x80) {
            *(news.p + news.pos) = *cur;
            news.pos++;
            news.free--;
            ++cur;
        } else {
            int clen, cn;

            if (codetype == CODETYPE_UTF8)
                clen = UTF8CHLEN(*((uint8_t*)cur));
            else if (codetype == CODETYPE_GBK)
                clen = GBKCHLEN(*((uint8_t*)cur));
            else
                clen = 2;

            if (cur + clen <= end) {
                for (cn = 0; cn < clen; ++cn)
                    *(news.p + news.pos + cn) = *(cur + cn);

                news.pos += clen;
                news.free -= clen;
                cur += clen;
            } else
                goto _end;
        }
    }

_end:
    *(news.p + news.pos) = 0;
    return news.p;
}

// 字符串过滤: 支持多个过滤、通配符、GBK和UTF-8编码。
// @return: need free.
char* strfilter(const char* str, int slen, char* filt[], int num, int codetype)
{
    const char *cur, *end;
    char *      pn, *news;
    int         i, j;

    news = (char*)xcalloc(1, slen + 1);
    cur  = str;
    pn   = news;
    end  = cur + slen;

    i    = 0; /*i->filt[] */
    j    = 0; /*j->filt[][] */

init:
    while (cur < end) {
        for (i = 0; i < num; i++) {
            const char* poo = cur;
            while (*cur == *(*(filt + i) + j) || (cur != end && *(*(filt + i) + j) == '(' && *(*(filt + i) + j + 1) == '*' && *(*(filt + i) + j + 2) == ')')) {
                if (*(*(filt + i) + j) == '(' && *(*(filt + i) + j + 1) == '*' && *(*(filt + i) + j + 2) == ')') {
                    if (!*(*(filt + i) + j + 3)) {
                        goto _end;
                    }
                    if (*cur == *(*(filt + i) + j + 3)) {
                        if (!*(*(filt + i) + j + 3 + 1)) {
                            cur++;
                            j = 0;
                            goto init;
                        }
                        cur++;
                        j += 4;
                    } else {
                        cur++;
                    }

                } else {
                    if (!*(*(filt + i) + j + 1)) {
                        cur++;
                        j = 0;
                        goto init;
                    }
                    cur++;
                    j++;
                }
            }

            cur = poo;
            j   = 0;
        }

        if (*((uint8_t*)cur) < 0x80) {
            *pn = *cur;
            ++pn;
            ++cur;
        } else {
            int clen, cn;

            if (codetype == CODETYPE_UTF8)
                clen = UTF8CHLEN(*((uint8_t*)cur));
            else if (codetype == CODETYPE_GBK)
                clen = GBKCHLEN(*((uint8_t*)cur));
            else
                clen = 2;

            if (cur + clen <= end) {
                for (cn = 0; cn < clen; ++cn)
                    *(pn + cn) = *(cur + cn);

                pn += clen;
                cur += clen;
            } else
                goto _end;
        }
    }
_end:
    *pn = 0;
    return news;
}

// 字符串中是否包含了指定的字符串之一。支持多个字符串，UTF-8和GBK编码
int strcontain(const char* str, char* filt[], int size, int codetype)
{
    int         i   = 0;  // i->filt[]
    int         j   = 0;  // j->filt[][]
    int         len = strlen(str);
    const char* po  = str;
    const char* pe  = str + len;

    while (*po) {
        for (i = 0; i < size; i++) {
            const char* poo = po;
            while (*po == *(*(filt + i) + j) || (*po && *(*(filt + i) + j) == '(' && *(*(filt + i) + j + 1) == '*' && *(*(filt + i) + j + 2) == ')')) {
                if (*(*(filt + i) + j) == '(' && *(*(filt + i) + j + 1) == '*' && *(*(filt + i) + j + 2) == ')') {
                    if (!*(*(filt + i) + j + 3)) {
                        return i;
                    }
                    if (*po == *(*(filt + i) + j + 3)) {
                        if (!*(*(filt + i) + j + 3 + 1)) {
                            return i;
                        }
                        po++;
                        j += 4;
                    } else {
                        po++;
                    }

                } else {
                    if (!*(*(filt + i) + j + 1)) {
                        return i;
                    }
                    po++;
                    j++;
                }
            }

            po = poo;
            j  = 0;
        }

        if (*((uint8_t*)po) < 0x80) {
            ++po;
        } else {
            int clen;

            if (codetype == CODETYPE_UTF8)
                clen = UTF8CHLEN(*((uint8_t*)po));  // need translate uint8_t *
            else if (codetype == CODETYPE_GBK)
                clen = GBKCHLEN(*((uint8_t*)po));
            else
                clen = 2;

            if (po + clen <= pe)
                po += clen;
            else
                return -1;
        }
    }

    return -1;
}

// 统计字符串中汉字数。
int strcontainhanzi(char* str, int codetype)
{
    int   num, clen;
    char *p, *e;

    if (str == NULL || *str == 0)
        return 0;

    num = 0;
    p   = str;
    e   = str + strlen(str);
    while (p < e)  // 防止由于汉字的不完整导致越界
    {
        if (codetype == CODETYPE_UTF8)
            clen = UTF8CHLEN(*((uint8_t*)p));
        else if (codetype == CODETYPE_GBK)
            clen = GBKCHLEN(*((uint8_t*)p));
        else
            clen = 2;

        if (clen > 1)
            num++;

        p += clen;
    }

    return num;
}

// support input == output. @codetype: gbk,utf-8,default:gbk.
// GBK编码的低字节和ASCII字母是重复的。
void strtolower(const char* input, int size, char* output, int codetype)
{
    uint8_t* psrc     = (uint8_t*)input;
    uint8_t* pdst     = (uint8_t*)output;
    uint8_t* psrc_end = psrc + size;
    int      clen, cn;

    while (psrc < psrc_end) {
        if ((*psrc & 0x80)) {
            if (codetype == CODETYPE_GBK)
                clen = GBKCHLEN(*psrc);
            else if (codetype == CODETYPE_UTF8)
                clen = UTF8CHLEN(*psrc);
            else
                clen = 2;

            if (psrc + clen <= psrc_end) {
                for (cn = 0; cn < clen; ++cn)
                    *pdst++ = *psrc++;
            } else {
                clen = psrc_end - psrc;
                for (cn = 0; cn < clen; ++cn)
                    *pdst++ = *psrc++;
                break;
            }
        } else {
            *pdst++ = CHARTOLOWER(*psrc++);
        }
    }

    *pdst = 0;
}

// support input == output. @codetype: gbk,utf-8,default:gbk.
// GBK编码的低字节和ASCII字母是重复的。
void strtoupper(const char* input, int size, char* output, int codetype)
{
    uint8_t* psrc     = (uint8_t*)input;
    uint8_t* pdst     = (uint8_t*)output;
    uint8_t* psrc_end = psrc + size;
    int      clen, cn;

    while (psrc < psrc_end) {
        if ((*psrc & 0x80)) {
            if (codetype == CODETYPE_GBK)
                clen = GBKCHLEN(*psrc);
            else if (codetype == CODETYPE_UTF8)
                clen = UTF8CHLEN(*psrc);
            else
                clen = 2;

            if (psrc + clen <= psrc_end) {
                for (cn = 0; cn < clen; ++cn)
                    *pdst++ = *psrc++;
            } else {
                clen = psrc_end - psrc;
                for (cn = 0; cn < clen; ++cn)
                    *pdst++ = *psrc++;
                break;
            }
        } else {
            *pdst++ = CHARTOUPPER(*psrc++);
        }
    }

    *pdst = 0;
}

void strtolower_ascii(char* str)
{
    char* p = str;
    while (*p) {
        if (*p >= 'A' && *p <= 'Z') {
            *p = *p - 'A' + 'a';
        }
        p++;
    }
}

void strtoupper_ascii(char* str)
{
    char* p = str;
    while (*p) {
        if (*p >= 'a' && *p <= 'z') {
            *p = *p - ('a' - 'A');  // 'a'=97 'A'=65
        }
        p++;
    }
}

// 保留前len or len + 1个字符
char* strcutforgbk(char* p, int len)
{
    int   i;
    char* r;

    if (!p) return p;

    r = p;
    for (i = 0; *p && i < len;) {
        if (*p & 0x80) {
            ++p;
            if (!*p) break;
            ++p;
            i += 2;
        } else {
            ++p;
            ++i;
        }
    }

    *p = '\0';
    return r;
}

// 截取前len or len+1个字节
char* strcutleftforgbk(char* p, int len)
{
    int   i, alen, clen;
    char* r;
    char  c;

    if (!p) return p;

    alen = strlen(p);
    r    = p;
    for (i = 0; *p && i < len;) {
        if (*p & 0x80) {
            ++p;
            if (!*p) break;
            ++p;
            i += 2;
        } else {
            ++p;
            ++i;
        }
    }

    c    = *p;
    *p   = 0;
    clen = strlen(r);
    *p   = c;
    memmove(r, p, alen - clen);
    r[alen - clen] = 0;
    return r;
}

// @rlen: max reserve length
char* strcutforutf8(char* str, int rlen)
{
    int     len, n;
    uint8_t ch;

    len = strlen(str);
    if (len <= rlen)
        return str;

    // skip 10xxxxxx,find 11xxxx
    n = rlen;
    while (n > 0) {
        ch = str[n - 1];
        if (ch < 0x80) {
            // 0xxxx
            str[n] = 0;
            return str;
        }

        // 1xxxxx
        if (ch & 0x40) {
            // 11xxx
            str[n - 1] = 0;
            return str;
        } else {
            // 10xxx
            n--;
        }
    }

    str[n] = 0;
    return str;
}

// @clen: max cut left length
char* strcutleftforutf8(char* str, int clen)
{
    int     len, n;
    uint8_t ch;

    len = strlen(str);
    if (len <= clen) {
        str[0] = 0;
        return str;
    }

    // skip 10xxxxxx,find 11xxxx
    n = clen;
    while (n > 0) {
        ch = str[n - 1];
        if (ch < 0x80) {
            // 0xxxx
            memmove(str, str + n, len - n);
            str[len - n] = 0;
            return str;
        }

        // 1xxxxx
        if (ch & 0x40) {
            // 11xxx
            n--;
            memmove(str, str + n, len - n);
            str[len - n] = 0;
            return str;
        } else {
            // 10xxx
            n--;
        }
    }

    return str;
}

/* [ 2 ] --> 2
     adb 45 d 67 --> 45
     int[ty] dkjd --> -1
*/
int strextractint(char* str, int len)
{
    int  i, j;
    char n[20];

    j = 0;
    for (i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            n[j] = str[i];
            j++;
            if (j >= 20)
                return -1;

            if (!isdigit(str[i + 1]))
                break;
        }
    }

    if (j != 0) {
        n[j] = 0;
        return atoi(n);
    } else
        return -1;
}

// test if b starts with a
int strstartwith(char* a, char* b)
{
    int i = 0;
    while (a[i] != 0) {
        if (a[i] != b[i]) return 0;
        i++;
    }

    return 1;
}

// test if b starts with a ignoring case,@amin is only lowercase.
int strstartwithignorecase(char* amin, char* b)
{
    int i = 0;
    while (amin[i] != 0) {
        if (amin[i] != (b[i] | 32)) return 0;
        i++;
    }
    return 1;
}

/* test if b end with a
 */
int strendwith(char* a, char* b)
{
    int la = strlen(a);
    int lb = strlen(b);
    return (la <= lb) && !strcmp(a, b + lb - la);
}

/* test if b end with a ignoring case
 * a can use min char(lowercase), '.' (a[i] = a[i] | 32)
 */
int strendwithignorecase(char* amin, char* b, int lb)
{
    int la = strlen(amin);
    if (la <= lb) {
        int i;
        int diff = lb - la;
        for (i = 0; i < la; i++) {
            if (amin[i] != (b[diff + i] | 32)) {
                return 0;
            }
        }
        return 1;
    } else {
        return 0;
    }
}

// 把字符串截短并返回字符串的汉字以及等效汉字数。@resverve指明保留的汉字数(包括等效汉字)。
// 等效汉字指的是两个英文字符等于一个汉字。实际可能会少保留半个字符。
int strtruncateforutf8(char* str, int reserve)
{
    int      clen, cnum, hznum;
    uint8_t *ps, *pe;

    if (!str || !*str)
        return 0;

    if (!reserve) {
        *str = 0;
        return 0;
    }

    ps    = (uint8_t*)str;
    pe    = ps + strlen(str);

    cnum  = 0;
    hznum = 0;
    while (ps != pe) {
        clen = UTF8CHLEN(*ps);
        if (ps + clen > pe) {
            *ps = 0;
            break;
        }
        if (clen == 1) {
            if (++cnum == 2) {
                cnum = 0;
                if (++hznum == reserve) {
                    ps[clen] = 0;
                    return hznum;
                }
            }
        } else {
            if (++hznum == reserve) {
                if (cnum == 1)
                    *ps = 0;
                else
                    *(ps + clen) = 0;
                return hznum;
            }
        }

        ps += clen;
    }

    if (cnum == 1)
        ++hznum;
    return hznum;
}

// 返回reserve汉字数的字节数
int strtrunclenforutf8(const char* str, int reserve)
{
    int      clen, cnum, hznum;
    uint8_t *ps, *pe;

    if (!str || !*str)
        return 0;

    if (!reserve) {
        //*str = 0;
        return 0;
    }

    ps    = (uint8_t*)str;
    pe    = ps + strlen(str);

    cnum  = 0;
    hznum = 0;
    while (ps != pe) {
        clen = UTF8CHLEN(*ps);
        if (ps + clen > pe) {
            return (ps - (uint8_t*)str);  //*ps = 0;
                                          // break;
        }
        if (clen == 1) {
            if (++cnum == 2) {
                cnum = 0;
                if (++hznum == reserve) {
                    // ps[clen] = 0;
                    // return hznum;
                    return (ps + clen - (uint8_t*)str);
                }
            }
        } else {
            if (++hznum == reserve) {
                if (cnum == 1)
                    return (ps - (uint8_t*)str);  //*ps = 0;
                else
                    return (ps + clen - (uint8_t*)str);  //*(ps+clen) = 0;
                                                         // return hznum;
            }
        }

        ps += clen;
    }

    return (ps - (uint8_t*)str);
    // if(cnum == 1) ++hznum;
    // return hznum;
}

// 返回reserve汉字数的字节数,会超过指定的长度，可能会在标点符号处截断。
// around指明在截断处寻找标点符号的范围，汉字数。
int strtruncsentenceforutf8(const char* str, int reserve, int l_around, int r_around)
{
    int      rmin, rmax;
    uint8_t *lp1, *rp1, *lp2, *rp2, *mp;
    int      clen, cnum, hznum, rlen, mlen, mnum;
    uint8_t *ps, *pe;

    if (!str || !*str) return 0;
    if (!reserve) return 0;
    if (l_around <= 0 || l_around >= reserve)
        rmin = reserve;
    else
        rmin = reserve - l_around;
    if (r_around <= 0)
        rmax = reserve;
    else
        rmax = reserve + r_around;  // rmax可能会超出str

    lp1   = NULL;
    rp1   = NULL;
    lp2   = NULL;
    rp2   = NULL;
    mp    = NULL;
    ps    = (uint8_t*)str;
    pe    = ps + strlen(str);
    cnum  = 0;
    hznum = 0;
    while (ps != pe) {
        clen = UTF8CHLEN(*ps);
        if (ps + clen > pe)  // 不需要截断时，编码错误则直接返回整个字符串的长度。
            return (ps - (uint8_t*)str);

        if (hznum >= rmin && hznum < reserve && ispunct1((char*)ps, clen))
            lp1 = ps + clen;
        else if (hznum >= rmin && hznum < reserve && ispunct2((char*)ps, clen))
            lp2 = ps + clen;
        else if (!rp1 && hznum > reserve && hznum <= rmax && ispunct1((char*)ps, clen))
            rp1 = ps + clen;
        else if (!rp2 && hznum > reserve && hznum <= rmax && ispunct2((char*)ps, clen))
            rp2 = ps + clen;

        if (clen == 1) {
            if (++cnum == 2) {
                cnum = 0;
                ++hznum;
            }
        } else {
            ++hznum;
        }

        if (hznum == reserve) {
            if (ispunct1((char*)ps, clen) || ispunct2((char*)ps, clen)) {
                if (cnum == 1)
                    return (ps - (uint8_t*)str);
                else
                    return (ps + clen - (uint8_t*)str);
            }
            mp   = ps;
            mlen = clen;
            mnum = cnum;
        }

        ps += clen;

        if (hznum >= rmax || (ps >= pe && hznum >= reserve)) {
            if (rp1)
                rlen = rp1 - (uint8_t*)str;
            else if (rp2)
                rlen = rp2 - (uint8_t*)str;
            else if (lp1)
                rlen = lp1 - (uint8_t*)str;
            else if (lp2)
                rlen = lp2 - (uint8_t*)str;
            else if (mnum == 1)
                rlen = mp - (uint8_t*)str;
            else
                rlen = mp + mlen - (uint8_t*)str;

            return rlen;
        }
    }

    // 没有截断则直接返回整个str的长度。
    return (ps - (uint8_t*)str);
}

// 计算字符串的汉字以及等效汉字数。
// 等效汉字指的是两个英文字符等于一个汉字。实际可能会少保留半个字符。
int strhanzilenforutf8(const char* str)
{
    int      clen, cnum, hznum;
    uint8_t *ps, *pe;

    if (!str || !*str)
        return 0;

    ps    = (uint8_t*)str;
    pe    = ps + strlen(str);

    cnum  = 0;
    hznum = 0;
    while (ps != pe) {
        clen = UTF8CHLEN(*ps);
        if (ps + clen > pe) {
            ++hznum;
            break;
        }

        if (clen == 1) {
            if (++cnum == 2) {
                cnum = 0;
                ++hznum;
            }
        } else {
            ++hznum;
        }

        ps += clen;
    }

    if (cnum == 1)
        ++hznum;
    return hznum;
}

char* strichr(const char* str, short c)
{
    short oc;

    if (isupper(c))
        oc = tolower(c);
    else
        oc = toupper(c);

    while (*str) {
        if (*str == c || *str == oc) return ((char*)str);
        str++;
    }

    return (NULL);
}

char* strnchr(const char* str, short c, int len)
{
    register char* donestr;

    donestr = (char*)((int)str + len);

    while (str < donestr) {
        if (*str == c) return ((char*)str);
        str++;
    }

    return (NULL);
}

char* strrchrn(char* str, char c, int n)
{
    int   i, len;
    char* p;

    if (!str || n <= 0)
        return NULL;

    len = strlen(str);
    p   = str + len - 1;

    for (i = 0; i < len; i++) {
        if (*p == c) {
            n--;
            if (n == 0)
                return p;
        }
        p--;
    }

    return NULL;
}

char* strchrn(char* str, char c, int n)
{
    char* p;

    if (!str || n <= 0)
        return NULL;

    p = str;
    while (*p) {
        if (*p == c) {
            n--;
            if (n == 0)
                return p;
        }
    }

    return NULL;
}

void strins(char* to, const char* fm)
{
    int   tolen = strlen(to);
    int   fmlen = strlen(fm);
    char *newto, *oldto;

    newto = to + fmlen + tolen;
    oldto = to + tolen;
    tolen++;
    while (tolen--) *newto-- = *oldto--;

    while (fmlen--) *to++ = *fm++;
}

// Reverse a string, eg.
// strrev("Hello world!");  outputs "!dlrow olleH"
char* strrev(char* str)
{
    register char *endstr, swapper;
    char*          strbase = str;

    endstr                 = str;
    while (*endstr) endstr++;
    endstr--;

    while (endstr > str) {
        swapper   = *str;
        *str++    = *endstr;
        *endstr-- = swapper;
    }

    return (strbase);
}

// Replaces in the string str all the occurrences of the source string old with the destination string new. The parameters old and new may be of any length, and their lengths are allowed to differ. None of the three parameters may be NULL.
// The post-replacement string, or NULL if memory for the new string could not be allocated. Does not modify the original string. The memory for the returned post-replacement string may be deallocated with the standard library function free() when it is no longer required.
char* replace_str(const char* str, const char* old, const char* new)
{
    char *      ret, *r;
    const char *p, *q;
    size_t      oldlen = strlen(old);
    size_t      count, retlen, newlen = strlen(new);

    if (oldlen != newlen) {
        for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
            count++;
        /* this is undefined if p - str > PTRDIFF_MAX */
        retlen = p - str + strlen(p) + count * (newlen - oldlen);
    } else
        retlen = strlen(str);

    if ((ret = malloc(retlen + 1)) == NULL)
        return NULL;

    for (r = ret, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen) {
        /* this is undefined if q - p > PTRDIFF_MAX */
        ptrdiff_t l = q - p;
        memcpy(r, p, l);
        r += l;
        memcpy(r, new, newlen);
        r += newlen;
    }
    strcpy(r, p);

    return ret;
}

static int strlencmp(const void* a, const void* b)
{
    struct _sl {
        char* s;
        int   len;
    };
    struct _sl* aa = (struct _sl*)a;
    struct _sl* bb = (struct _sl*)b;

    if (aa->len > bb->len)
        return 1;
    else if (aa->len == bb->len)
        return 0;
    else
        return -1;
}

// 把字符串数组按照其长度降序排列。一般用于strreplace() strfilter()
void strlensortbydesc(char** ss, int num)
{
    int i;

    struct _sl {
        char* s;
        int   len;
    }* s;

    s = (struct _sl*)xcalloc(sizeof(struct _sl), num);
    for (i = 0; i < num; i++) {
        s[i].s   = ss[i];
        s[i].len = strlen(ss[i]);
    }

    qsort(s, num, sizeof(struct _sl), strlencmp);

    for (i = num; i > 0; i--) {
        ss[num - i] = s[i - 1].s;
    }
    xfree(s);
}

#ifdef TEST_XSTRING

int main()
{
    char  utf8[1024];
    char* old[] = {"xing", "hello"};
    char* new[] = {"XING", "HELLO"};
    char*  f[]  = {"?(*)"};
    char * p, *c, *s;
    char   str[1024] = "    ";
    char** frags;

    strgetfrags(str, " ", &frags);

    s = "xxx?";
    c = strfilter(s, strlen(s), f, 1, CODETYPE_UTF8);
    printf("%s\n", c);
    xfree(c);

    p = strreplace("liuxingzhi,hello,world.", strlen("liuxingzhi,hello,world."), old, new, 2, CODETYPE_UTF8);
    printf("%s\n", p);

    c = strfilter(p, strlen(p), new, 2, CODETYPE_UTF8);
    printf("%s\n", c);
    xfree(p);
    xfree(c);

    while (fgets(utf8, 1024, stdin)) {
        trim(utf8);
        if (strcasecmp("exit", utf8) == 0)
            break;
        printf("HZ: %d\n", strcontainhanzi(utf8, CODETYPE_UTF8));
        printf("%s\n", strcutforutf8(utf8, 20));
        printf("%s\n", strcutleftforutf8(utf8, 10));
    }

    return 1;
}
#endif
