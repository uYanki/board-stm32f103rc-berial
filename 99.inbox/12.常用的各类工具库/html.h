/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef __HTML_H
#define __HTML_H

#define MAX_HLIGHT_KW 8

#ifdef __cplusplus
extern "C" {
#endif

void*       html_filtertag_init(const char* reservetags,
                                const char* skiptags,
                                const char* reserveattrs,
                                int         maxtextsize,
                                int         xssfilter);
const char* html_filtertags(void* tags, const char* html, int htmllen, int codetype, int* sp);
void        html_filtertag_free(void* tags);

char* html_findtagclosemark(char* tag_start_mark, int len, char** tagname_beg, char** tagname_end);
int   html_filterjavascript(const char* input, int inlen, char* output, int outlen, int codetype);
int   html_getimgsrc(const char* html, const char* prefix, char* buffer, int bufsize);
char* html_modifiedimgattr(const char* html, const char* alt, const char* title);
int   html_highlight_position(char* page, int page_size, char* kw[], char* hl[], char* pos[], int kw_num, char* hl_page, int hlpage_bufsize, char* title, int title_bufsize);

#ifdef __cplusplus
}
#endif

#endif
