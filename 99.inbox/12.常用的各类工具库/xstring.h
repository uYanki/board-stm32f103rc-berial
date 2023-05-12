/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef XSTRING_H
#define XSTRING_H

#ifdef __cplusplus
extern "C" {
#endif
void  strplustospace(char* str);
void  strspacetoplus(char* str);
char* skipwhitespace(char* str);
void  trim(char* str);
int   trimid(const char* in, char* out, int trans);
int   trimid_utf8(const char* in, char* out, int trans);
char* strsqueeze(char* str);
int   strfragnum(const char* str, const char* delimiter);
int   split(char* str, const char* delimiter, char* fragment[], int frags);
int   strgetfrags(const char* str, const char* delimiter, char*** frags);

char* stristr(const char* StrBase, const char* SubBase);
char* stristrb(const char* StrBase, int str_len, const char* SubBase, int sub_len);
void  strcharrep(char* str, char fm, char to);
char* strreplace(const char* str, int slen, char* old[], char* new[], int num, int codetype);
char* strfilter(const char* str, int slen, char* filt[], int num, int codetype);
int   strcontain(const char* str, char* filt[], int size, int codetype);
int   strcontainhanzi(char* str, int codetype);

void strtolower(const char* input, int size, char* output, int codetype);
void strtolower_ascii(char* str);
void strtoupper(const char* input, int size, char* output, int codetype);
void strtoupper_ascii(char* str);

char* strcutforgbk(char* p, int len);
char* strcutleftforgbk(char* p, int len);
char* strcutforutf8(char* str, int rlen);
char* strcutleftforutf8(char* str, int clen);

int strextractint(char* str, int len);
int strstartwith(char* a, char* b);
int strstartwithignorecase(char* amin, char* b);
int strendwith(char* a, char* b);
int strendwithignorecase(char* amin, char* b, int lb);

int strtruncateforutf8(char* str, int reserve);
int strtrunclenforutf8(const char* str, int reserve);
int strtruncsentenceforutf8(const char* str, int reserve, int l_around, int r_around);
int strhanzilenforutf8(const char* str);

char* strichr(const char* str, short c);
char* strnchr(const char* str, short c, int len);
char* strrchrn(char* str, char c, int n);
char* strchrn(char* str, char c, int n);
void  strins(char* to, const char* fm);
char* strrev(char* str);

char* replace_str(const char* str, const char* old, const char* new);
void  strlensortbydesc(char** ss, int num);

#ifdef __cplusplus
}
#endif

#endif
