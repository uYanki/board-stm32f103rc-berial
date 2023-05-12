/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "xmalloc.h"
#include "xstring.h"
#include "tcutil.h"
#ifdef USE_GNU_RE
#include <regex.h>  // used gnu regex, but not support utf-8.
#define REG_UTF8 0x0040
#else
#include <pcreposix.h>  // used pcre posix api, support utf-8.
#endif

// 基于正则表达式的查找
// isfirst: 是不是第一次对str进行查找。
// 如果regex以'*'开头则支持忽略字母大小写
// 返回找到的第一个匹配字符串的位置和长度，错误返回-1.
int regex_search(const char* str, const char* regex, int isfirst, int isutf8, int* sublen)
{
    regex_t    rbuf;
    regmatch_t subs[256];
    int        options = REG_EXTENDED;
    int        pos;

    assert(str && regex);
    if (*regex == '*') {
        options |= REG_ICASE;
        regex++;
    }

    if (isutf8)
        options |= REG_UTF8;  // NOT defined by POSIX; maps to PCRE_UTF8. define at pcreposix.h

    if (regex[0] == '\0' || regcomp(&rbuf, regex, options) != 0)
        return -1;

    if (regexec(&rbuf, str, 32, subs, isfirst ? 0 : REG_NOTBOL) != 0) {
        regfree(&rbuf);
        return -1;
    }

    pos = subs[0].rm_so;
    if (pos >= 0)
        *sublen = subs[0].rm_eo - subs[0].rm_so;
    regfree(&rbuf);
    return pos;
}

// 指定字符串是否有匹配正则表达式的子串，匹配成功返回1，匹配失败返回0
// 如果regex以'*'开头则支持忽略字母大小写
int regex_match(const char* str, const char* regex, int isutf8)
{
    regex_t rbuf;
    int     rv, options;

    assert(str && regex);
    options = REG_EXTENDED | REG_NOSUB;
    if (*regex == '*') {
        options |= REG_ICASE;
        regex++;
    }

    if (isutf8)
        options |= REG_UTF8;  // NOT defined by POSIX; maps to PCRE_UTF8. define at pcreposix.h

    if (regcomp(&rbuf, regex, options) != 0) return 0;
    rv = regexec(&rbuf, str, 0, NULL, 0) == 0;
    regfree(&rbuf);
    return rv;

    // return tcregexmatch(str,regex);
}

// 基于正则表达式的替换
// 如果regex以'*'开头则支持忽略字母大小写。
// 如果alt有'&'符号，则用匹配串代替，如果参数alt中有'\1'~'\9'则用子正则表达式匹配串来代替。
// 返回转换后的新字符串，如果regex是无效的返回原始串的拷贝，需要free。
char* regex_replace(const char* str, const char* regex, const char* alt, int isutf8)
{
    regex_t     rbuf;
    int         options;
    regmatch_t  subs[256];
    const char* sp;
    TCXSTR*     xstr;
    int         first;
    const char* rp;

    assert(str && regex && alt);
    options = REG_EXTENDED;
    if (*regex == '*') {
        options |= REG_ICASE;
        regex++;
    }

    if (isutf8)
        options |= REG_UTF8;  // NOT defined by POSIX; maps to PCRE_UTF8. define at pcreposix.h

    if (regex[0] == '\0' || regcomp(&rbuf, regex, options) != 0)
        return tcstrdup(str);

    if (regexec(&rbuf, str, 32, subs, 0) != 0) {
        regfree(&rbuf);
        return tcstrdup(str);
    }

    sp    = str;
    xstr  = tcxstrnew();
    first = 1;
    while (sp[0] != '\0' && regexec(&rbuf, sp, 10, subs, first ? 0 : REG_NOTBOL) == 0) {
        first = 0;
        if (subs[0].rm_so == -1) break;
        tcxstrcat(xstr, sp, subs[0].rm_so);
        for (rp = alt; *rp != '\0'; rp++) {
            if (*rp == '\\') {
                if (rp[1] >= '0' && rp[1] <= '9') {
                    int num = rp[1] - '0';
                    if (subs[num].rm_so != -1 && subs[num].rm_eo != -1)
                        tcxstrcat(xstr, sp + subs[num].rm_so, subs[num].rm_eo - subs[num].rm_so);
                    ++rp;
                } else if (rp[1] != '\0') {
                    tcxstrcat(xstr, ++rp, 1);
                }
            } else if (*rp == '&') {
                tcxstrcat(xstr, sp + subs[0].rm_so, subs[0].rm_eo - subs[0].rm_so);
            } else {
                tcxstrcat(xstr, rp, 1);
            }
        }
        sp += subs[0].rm_eo;
        if (subs[0].rm_eo < 1) break;
    }
    tcxstrcat2(xstr, sp);
    regfree(&rbuf);
    return tcxstrtomalloc(xstr);
    // return tcregexreplace(str, regex, alt);
}

// 基于正则表达式的过滤
// 如果regex以'*'开头则支持忽略字母大小写。
// 返回转换后的新字符串，如果regex是无效的返回原始串的拷贝，需要free。
char* regex_filter(const char* str, const char* regex, int isutf8)
{
    char alt[1];

    alt[0] = 0;
    return regex_replace(str, regex, alt, isutf8);
    // return tcregexreplace(str,regex,alt);
}

#ifdef TEST_REGEXP

int main()
{
    int   res, pos, sublen;
    char  cmd[1024], buf[1024], re[1024], alt[1024];
    char* r;

    printf("Please input command(Search,Match,Replace,Filter,exit)>>");
    fflush(stdout);
    while (fgets(cmd, sizeof(cmd), stdin)) {
        trim(cmd);
        if (!cmd[0]) goto next;
        if (strcasecmp(cmd, "exit") == 0) break;
    text:
        printf("text>>");
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
        trim(buf);
        if (!buf[0]) goto text;

    regex:
        printf("regex>>");
        fflush(stdout);
        fgets(re, sizeof(re), stdin);
        trim(re);
        if (!re[0]) goto regex;

    alt:
        if (strcasecmp(cmd, "replace") == 0 || strcasecmp(cmd, "r") == 0) {
            printf("alt>>");
            fflush(stdout);
            fgets(alt, sizeof(alt), stdin);
            trim(alt);
            if (!alt[0]) goto alt;
        }

        if (strcasecmp(cmd, "search") == 0 || strcasecmp(cmd, "s") == 0) goto search;
        if (strcasecmp(cmd, "match") == 0 || strcasecmp(cmd, "m") == 0) goto match;
        if (strcasecmp(cmd, "replace") == 0 || strcasecmp(cmd, "r") == 0) goto replace;
        if (strcasecmp(cmd, "filter") == 0 || strcasecmp(cmd, "f") == 0) goto filter;

    search:
        pos = regex_search(buf, re, 1, 1, &sublen);
        if (pos < 0) {
            printf("no found.\n");
        } else {
            memcpy(re, buf + pos, sublen);
            re[sublen] = 0;
            printf("%s\n", re);
        }
        goto next;
    match:
        if (regex_match(buf, re, 1))
            printf("match ok.\n");
        else
            printf("no match.\n");
        goto next;
    replace:
        r = regex_replace(buf, re, alt, 1);
        printf("Result>>%s\n", r);
        xfree(r);
        goto next;
    filter:
        r = regex_filter(buf, re, 1);
        printf("Result>>%s\n", r);
        xfree(r);
        goto next;
    next:
        printf("Please input command(Search,Match,Replace,Filter,exit)>>");
        fflush(stdout);
    }

    return 0;
}

#endif
