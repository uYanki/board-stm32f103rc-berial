/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

// file...
long     getfilesize(FILE* fp);
char*    loadfile(const char* path, long* filesize);
char*    loadfilep(FILE* fp, long* filesize);
uint8_t* loadfileb(const char* path, long* filesize);
int      savefile(const char* path, void* buf, int size);
int      getlinenum(const char* buf, const char* newline);
char*    getaline(char* buf, const char* newline, int* linelen);
char*    getalinekeep(const char* buf, const char* newline, int* linelen);
int      filewriteint(const char* path, int num);
int      filereadint(const char* path);

// compress...
uint8_t* compressx(const char* input, int inlen, int* outlen);
char*    uncompressx(const char* input, int inlen, int* outlen);
int      compressfile(const char* infilename, const char* outfilename, int chunksize);
int      decompressfile(const char* infilename, const char* outfilename, int chunksize);
char*    inflate4gzip(char* in, int inlen, int* outlen);

// encode...
char* urlencode(char* source, char* dest);
char* urldecode(char* s);
int   xmlencode(const char* src, char* des);
int   htmlencode(const char* src, char* des);
int   htmlencode2(const char* src, char* des, const char* tag);
int   htmlencoden(const char* src, char* des, int desSize);
int   htmldecode(char* des);
int   htmldecoded(const char* src, char* des);
int   base64encode(const char* str, int length, char* b64store);
int   base64encode4url(const char* str, int length, char* b64store);
int   base64decode(const char* base64, char* to);
int   base64decode4url(const char* base64, char* to);
char* binencode(char* in, int inlen, char* out, char nbits);
char* bin2hex(uint8_t* bin, int binlen, char* hex);
int   hex2bin(char* hex, uint8_t* bin);
char* hexencode(char* source, char* dest);
char* hexdecode(char* s);

void format_thousands_separator(long val, char* buf, int nLen);

#ifdef __cplusplus
}
#endif

#endif
