/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef CONFHASH_H
#define CONFHASH_H


#ifdef __cplusplus
extern "C"
{
#endif

  void *conf_hash_new(const char *confpath);
  const char *conf_hash_get(void *CONFHASH,const char *key);
  void conf_hash_del(void *CONFHASH);

#ifdef __cplusplus
}
#endif

#endif


