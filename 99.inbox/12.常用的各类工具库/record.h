/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef RECORD_H
#define RECORD_H

#define RECORD_BEG_FLAG "<_record_>\r\n"
#define RECORD_END_FLAG "</_record_>\r\n\r\n"

enum {
    EEOD = 0,  // eod: end of dir
    ENODIR,    // no such directory
    ENOFILE,   // no such file
    ERENAME,   // rename error
    EREMOVE    // remove error
};

#define FIELD_NONE    0
#define FIELD_ZIP     'z'  // 压缩(z)
#define FIELD_UNZIP   'x'  // 解压缩(x)
#define FIELD_CRYPT   'c'  // 加密(c)
#define FIELD_DECRYPT 'd'  // 解密(d)

#ifdef __cplusplus
extern "C" {
#endif

// 从文件中读取记录
void*       record_fromfile_init(const char* path, const char* recbeg, const char* recend, int bufsize, int codetype);
const char* record_fromfile_get(void* datafile, int* record_len);
void        record_fromfile_free(void* datafile);

// 从目录读取文件
void*       record_fromdir_init(const char* path, const char* prefix, const char* suffix, const char* delim, int isremove, const char* rename, const char* recbeg, const char* recend, int bufsize, int codetype);
void        record_fromdir_bof(void* datadir, void* bof, void* arg);
void        record_fromdir_eof(void* datadir, void* eof, void* arg);
void        record_fromdir_rewind(void* datadir);
void*       record_fromdir_reopen(void* datadir);
const char* record_fromdir_get(void* datadir, int* reclen);
const char* record_fromdir_curfile(void* datadir);
int         record_fromdir_filenum(void* datadir);
int         record_fromdir_recnum(void* datadir);
int         record_fromdir_filerecnum(void* datadir);
int         record_fromdir_errno(void* datadir);
const char* record_fromdir_errmsg(void* datadir);
void        record_fromdir_free(void* datadir);

// 对记录进行解析
void* record_parse_init(const char* fieldnames, int codetype, const char* pwdfile, int pwdmax);
int   record_parse(void* rec_mode, const char* record, int record_len);
char* record_parse_getbyname(void* rec_mode, const char* fieldname, int* fieldlen, int* free);
char* record_parse_getbyid(void* rec_mode, int fieldid, int* fieldlen, char** fname, int* free);
int   record_parse_getfieldnum(void* rec_mode);
void  record_parse_free(void* rec_mode);

#ifdef __cplusplus
}
#endif

#endif
