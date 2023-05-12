/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#define RECOGNIZER_INIT_TYPE_CREATE 1
#define RECOGNIZER_INIT_TYPE_LOAD   2

#define MAX_SEGMENT_STRING          1024
#define MAX_SEGMENT_OUTPUT          (2 * (MAX_SEGMENT_STRING))
#define WORD_TYPE_UNKNOW            0
#define WORD_TYPE_SEPARATOR         '^'

#ifdef __cplusplus
extern "C" {
#endif

void* recognizer_init(const char* word_path, const char* dic_dir, int type, int codetype, int isprint);
int   recognizer_words(void* DIC, const char* QueryStr, char* WordStr);
void  recognizer_free(void* DIC);
int   recognizer_save(const char* dir, void* DIC);
int   recognizer_load(const char* dir, void* DIC);
int   recognizer_delete(void* DIC, const char* path);
int   recognizer_append(void* DIC, const char* path, int isprint);

#ifdef __cplusplus
}
#endif

#endif
