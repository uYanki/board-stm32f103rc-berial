/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef DIR_H
#define DIR_H

#ifdef __cplusplus
extern "C" {
#endif

void*       _opendir(const char* dir);
void        _closedir(void* vp);
const char* _readdir(void* vp);
const char* _readdirft(void* vp, char** prefixpp, int prefixnum, char** suffixpp, int suffixnum);
void        _rewinddir(void* vp);
int         mkdir_inpath(const char* path);

#ifdef __cplusplus
}
#endif
#endif
