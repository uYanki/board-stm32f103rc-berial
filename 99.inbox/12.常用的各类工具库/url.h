/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef URL_H
#define URL_H

#include <iconv.h>

#ifdef __cplusplus
extern "C" {
#endif

void*       url_qryparse_init(void);
int         url_qryparse(void* qrylist, const char* querystring, int unescape);
int         url_qryparse2(void* qrylist, const char* querystring, int unescape, const char delim, const char sep);
const char* url_qryparse_get(void* qrylist, const char* name);
int         url_qryparse_getvarnum(void* qrylist);
void        url_qryparse_iterator_init(void* qrylist);
const char* url_qryparse_iterator_next(void* qrylist, char** varname);
void        url_qryparse_free(void* qrylist);

int   url_isdynamic(const char* fullpath);
int   url_startwithprotocol(char* s);
int   url_filenormalize(char* file);
int   url_dirdepth(const char* path);
int   url_validipv4address(const char* str, const char* end);
char* url_transcode(char* url, int pageisutf8, iconv_t cd);
char* url_mergenormalize(char* url, char* base_url, char* ref_url, int pageisutf8, iconv_t cd, char* output_url_buf, int buf_size);

#ifdef __cplusplus
}
#endif

#endif
