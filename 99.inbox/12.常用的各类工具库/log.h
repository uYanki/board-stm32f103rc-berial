/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef LOG_H
#define LOG_H

#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_WARN  3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_FATAL 5
#define MIN_LOG_LEVEL   1
#define MAX_LOG_LEVEL   5

enum {
    LLDEBUG = 1,
    LLINFO,
    LLWARN,
    LLERROR,
    LLFATAL
};

#ifdef __cplusplus
extern "C" {
#endif
int  init_log(int log_filter_level, int log_max_size, const char* log_dir, const char* log_name);
void write_log(int level, const char* msg_fmt, ...);
void write_log_r(int level, const char* msg_fmt, ...);
void flush_log(void);
void rotate_log(void);
void free_log(void);
#ifdef __cplusplus
}
#endif

#endif
