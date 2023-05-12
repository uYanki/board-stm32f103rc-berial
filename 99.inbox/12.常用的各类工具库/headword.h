/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef  HEADWORD_H
#define  HEADWORD_H

#ifdef __cplusplus
extern "C"
{
#endif

  void *headword_init(const char *dicdir, int codetype);
  int headword_extract(void *Dic, const char *content, char *wordbuf, int bufsize, int codetype);
  void headword_free(void *Dic);


#ifdef __cplusplus
}
#endif


#endif

