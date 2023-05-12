/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef REGEXP_H
#define REGEXP_H

#ifdef __cplusplus
extern "C" {
#endif

int   regex_search(const char* str, const char* regex, int isfirst, int isutf8, int* sublen);
int   regex_match(const char* str, const char* regex, int isutf8);
char* regex_filter(const char* str, const char* regex, int isutf8);
char* regex_replace(const char* str, const char* regex, const char* alt, int isutf8);

#ifdef __cplusplus
}
#endif

#endif
