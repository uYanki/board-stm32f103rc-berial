/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef CHARSET_H
#define CHARSET_H

enum {
    CODETYPE_UNKNOW = 0,
    CODETYPE_UTF8   = 1,
    CODETYPE_GBK    = 2,
    CODETYPE_ASCII,
    CODETYPE_GB2312,
    CODETYPE_GB18030,
    CODETYPE_BIG5,
    CODETYPE_UNICODE
};

extern uint8_t utf8charlen[256];
extern uint8_t gbkcharlen[256];
extern uint8_t lowercharmap[256];
extern uint8_t uppercharmap[256];

// 注意c为char则需要转换为uint8_t
#define UTF8CHLEN(c)   utf8charlen[(c)]
#define GBKCHLEN(c)    gbkcharlen[(c)]
#define CHARTOLOWER(c) lowercharmap[(c)]
#define CHARTOUPPER(c) uppercharmap[(c)]

#ifdef __cplusplus
extern "C" {
#endif

void gbkqj2bj(const char* szIn, char* szOut);
void gbkqj2bjext(const char* szIn, char* szOut);
void utf8space2ascii(const char* in, char* out);
void utf8qj2bj(const char* in, char* out);
void utf8qj2bjext(const char* in, char* out);
void utf8qj2bjext2(const char* in, char* out);
void utf8qj2bjtolower(const char* in, char* out);

int ispunct1(const char* ch, int clen);
int ispunct2(const char* ch, int clen);

void* utf8fan2jian_init(const char* unihan);
void  utf8fan2jian(void* map, const char* fan, char* jian);
void  utf8fan2jian_free(void* map);

#ifdef __cplusplus
}
#endif

#endif
