/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#endif

#include "dir.h"
#include "log.h"
#include "comn.h"

#define LOG_PATH_MAX       255
#define LOG_NAME_MAX       255
#define LOG_FILE_NUM       1024                     /* 日志目录下最大日志文件数 */
#define LOG_MAX_SIZE       (1 * 1024 * 1024 * 1024) /* 1G */
#define LOG_RECORD_MAX_LEN 1536

static int  compare(const void* a, const void* b);
static void rotate(char* log_dir, char* log_name);

typedef struct
{
    int  log_filter_level;
    int  log_max_size;
    char log_dir[LOG_PATH_MAX + 1];
    char log_name[LOG_NAME_MAX + 1];
} log_conf_t;

static FILE*      log_fp;
static log_conf_t log_conf;
static int        log_size;
static char       log_path[LOG_PATH_MAX + 1];
#ifndef WIN32
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
#define THREADID() (size_t) pthread_self()
#endif
static char* weekday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};  // 支持按周统计

int init_log(int log_filter_level, int log_max_size, const char* log_dir, const char* log_name)
{
    int dir_len;

    if (!log_dir || !log_name)
        return -1;

    dir_len = strlen(log_dir);
    if (dir_len + strlen(log_name) + 5 > LOG_PATH_MAX || log_filter_level < LOG_LEVEL_DEBUG || log_filter_level > LOG_LEVEL_FATAL || log_max_size < 0 || log_max_size > LOG_MAX_SIZE)
        return -1;

    strcpy(log_conf.log_dir, log_dir);
    strcpy(log_conf.log_name, log_name);
    log_conf.log_filter_level = log_filter_level;
    if (log_max_size == 0)
        log_conf.log_max_size = LOG_MAX_SIZE;
    else
        log_conf.log_max_size = log_max_size;

    if (log_conf.log_dir[dir_len - 1] == '/') {
        log_conf.log_dir[dir_len - 1] = '\0';
        dir_len--;
    }

    strcpy(log_path, log_conf.log_dir);
    log_path[dir_len] = '/';
    dir_len++;
    strcpy(log_path + dir_len, log_conf.log_name);

    log_fp = fopen(log_path, "ab");
    if (!log_fp)
        return -1;

    fseek(log_fp, 0, SEEK_END);
    log_size = ftell(log_fp);
    fseek(log_fp, 0, SEEK_SET);

    if (log_size >= log_conf.log_max_size) {
        fclose(log_fp);
        rotate(log_conf.log_dir, log_conf.log_name);
        log_fp = fopen(log_path, "wb");
        if (log_fp == NULL)
            return -1;
        log_size = 0;
    }

    return 0;
}

void write_log(int level, const char* msg_fmt, ...)
{
    va_list    pargs;
    char       record_buf[LOG_RECORD_MAX_LEN + 1];
    char       record_buf2[LOG_RECORD_MAX_LEN + 1];
    time_t     seconds;
    struct tm* ptm;
    int        ret_len;

    if (level < LOG_LEVEL_DEBUG || level > LOG_LEVEL_FATAL || msg_fmt == NULL)
        return;

    if (level < log_conf.log_filter_level)
        return;

    time(&seconds);
    ptm = localtime(&seconds);

    switch (level) {
        case LOG_LEVEL_DEBUG: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, "DEBUG: %04d-%02d-%02d %s %02d:%02d:%02d %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, msg_fmt);
            break;
        }
        case LOG_LEVEL_INFO: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, " INFO: %04d-%02d-%02d %s %02d:%02d:%02d %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, msg_fmt);
            break;
        }
        case LOG_LEVEL_WARN: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, " WARN: %04d-%02d-%02d %s %02d:%02d:%02d %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, msg_fmt);
            break;
        }
        case LOG_LEVEL_ERROR: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, "ERROR: %04d-%02d-%02d %s %02d:%02d:%02d %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, msg_fmt);
            break;
        }

        case LOG_LEVEL_FATAL: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, "FATAL: %04d-%02d-%02d %s %02d:%02d:%02d %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, msg_fmt);
            break;
        }
        default:
            return;
    }

    if (ret_len <= 0 || ret_len > LOG_RECORD_MAX_LEN) /* log record too long */
        return;

    va_start(pargs, msg_fmt);
    ret_len = vsnprintf(record_buf2, LOG_RECORD_MAX_LEN + 1, record_buf, pargs);
    va_end(pargs);
    if (ret_len <= 0 || ret_len > LOG_RECORD_MAX_LEN) /* log record too long */
        return;

    fwrite(record_buf2, 1, ret_len, log_fp);
    if (level == LOG_LEVEL_FATAL)
        fflush(log_fp);
    log_size += ret_len;
    if (log_size >= log_conf.log_max_size) {
        fclose(log_fp);
        rotate(log_conf.log_dir, log_conf.log_name);
        log_fp = fopen(log_path, "wb");
        if (log_fp == NULL)
            log_fp = stdout;
        log_size = 0;
    }
    return;
}

#ifndef WIN32
void write_log_r(int level, const char* msg_fmt, ...)
{
    va_list    pargs;
    char       record_buf[LOG_RECORD_MAX_LEN + 1];
    char       record_buf2[LOG_RECORD_MAX_LEN + 1];
    time_t     seconds;
    struct tm* ptm;
    int        ret_len;

    if (level < LOG_LEVEL_DEBUG || level > LOG_LEVEL_FATAL || msg_fmt == NULL)
        return;

    if (level < log_conf.log_filter_level)
        return;

    time(&seconds);
    ptm = localtime(&seconds);

    switch (level) {
        case LOG_LEVEL_DEBUG: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, "DEBUG: %04d-%02d-%02d %s %02d:%02d:%02d #%p %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (char*)THREADID(), msg_fmt);
            break;
        }
        case LOG_LEVEL_INFO: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, " INFO: %04d-%02d-%02d %s %02d:%02d:%02d #%p %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (char*)THREADID(), msg_fmt);
            break;
        }
        case LOG_LEVEL_WARN: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, " WARN: %04d-%02d-%02d %s %02d:%02d:%02d #%p %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (char*)THREADID(), msg_fmt);
            break;
        }
        case LOG_LEVEL_ERROR: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, "ERROR: %04d-%02d-%02d %s %02d:%02d:%02d #%p %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (char*)THREADID(), msg_fmt);
            break;
        }

        case LOG_LEVEL_FATAL: {
            ret_len = snprintf(record_buf, LOG_RECORD_MAX_LEN + 1, "FATAL: %04d-%02d-%02d %s %02d:%02d:%02d #%p %s\n",
                               (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                               weekday[ptm->tm_wday], ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (char*)THREADID(), msg_fmt);
            break;
        }
        default:
            return;
    }

    if (ret_len <= 0 || ret_len > LOG_RECORD_MAX_LEN) /* log record too long */
        return;

    va_start(pargs, msg_fmt);
    ret_len = vsnprintf(record_buf2, LOG_RECORD_MAX_LEN + 1, record_buf, pargs);
    va_end(pargs);
    if (ret_len <= 0 || ret_len > LOG_RECORD_MAX_LEN) /* log record too long */
        return;

    pthread_mutex_lock(&log_mutex);
    fwrite(record_buf2, 1, ret_len, log_fp);
    if (level == LOG_LEVEL_FATAL)
        fflush(log_fp);
    log_size += ret_len;
    if (log_size >= log_conf.log_max_size) {
        fclose(log_fp);
        rotate(log_conf.log_dir, log_conf.log_name);
        log_fp = fopen(log_path, "wb");
        if (log_fp == NULL)
            log_fp = stdout;
        log_size = 0;
    }
    pthread_mutex_unlock(&log_mutex);
    return;
}
#else
void write_log_r(int level, const char* msg_fmt, ...)
{
    return;
}
#endif

void flush_log(void)
{
    pthread_mutex_lock(&log_mutex);
    fflush(log_fp);
    pthread_mutex_unlock(&log_mutex);
}

void rotate_log(void)
{
    pthread_mutex_lock(&log_mutex);
    fclose(log_fp);
    rotate(log_conf.log_dir, log_conf.log_name);
    log_fp = fopen(log_path, "wb");
    if (log_fp == NULL)
        log_fp = stdout;
    log_size = 0;
    pthread_mutex_unlock(&log_mutex);
}

void free_log(void)
{
    fclose(log_fp);
    return;
}

/* static void rotate(char *log_dir,char *log_name);
 *  功能：轮转日志文件。
 *  (1)修改日志文件的编号。
 *  (2)创建一个新的日志文件。
 *  返回值：
 *       OK,返回0。
 *       ERROR,返回-1。
 注意:
 (1)关闭日志文件时要刷新日志缓冲区，因为进行日志轮转时要求日志缓冲区为空，所有的日志记录都写入到日志文件中。
 (2)修改日志文件的编号，日志文件是从1开始编号，最大到1023。
 (3)创建一个新的日志文件。
 (4)设置log_size = 0
 (5)修改日志文件名编号的算法
    -- 依次读出日志目录下的每一个文件，并把每个文件名的后缀编号按读出顺序存入后缀名数组name_suffix。
    -- 对后缀名数组从小到大进行排序。
    -- 根据排序后的顺序从大到小依次重新对文件进行编号，编号规则是文件的原编号加一，这样就避免了文件重命名后文件重复。
 (6)只有在一种情况下会进行日志轮转:日志文件大小达到最大值。
 */
static void rotate(char* log_dir, char* log_name)
{
    void* DIR;
    char* filename;
    char  old_name[LOG_NAME_MAX + 5];
    char  new_name[LOG_NAME_MAX + 5];
    int   name_suffix[LOG_FILE_NUM];
    int   dirent_cnt = 0;
    int   i, path_len, len, len2;

    DIR = _opendir(log_dir);
    if (DIR == NULL)
        return;

    len = strlen(log_name);
    while ((filename = (char*)_readdir(DIR)) != NULL) {
        len2 = strlen(filename);
        if ((len2 == len) && (strncmp(filename, log_name, len) == 0)) {
            dirent_cnt++;
            if (dirent_cnt <= LOG_FILE_NUM) {
                name_suffix[dirent_cnt - 1] = 0;
            } else {
                return;
            }
            continue;
        }

        if ((len2 > len) && (strncmp(filename, log_name, len) == 0)) {
            dirent_cnt++;
            if (dirent_cnt <= LOG_FILE_NUM) {
                name_suffix[dirent_cnt - 1] = atoi(filename + len + 1); /* xxxLog.1 */
            } else {
                return;
            }
        }
    }

    _closedir(DIR);

    /* sort */
    qsort(name_suffix, dirent_cnt, sizeof(int), compare);

    /* rename */
    path_len = strlen(log_dir);
    strcpy(old_name, log_dir);
    old_name[path_len] = '/';
    strcpy(old_name + path_len + 1, log_name);
    strcpy(new_name, old_name);
    len = strlen(old_name);
    for (i = dirent_cnt - 1; i >= 0; --i) {
        if (name_suffix[i] == 0)
            old_name[len] = 0;
        else
            sprintf(old_name + len, ".%d", name_suffix[i]);

        sprintf(new_name + len, ".%d", name_suffix[i] + 1);
        if (rename(old_name, new_name) != 0) {
            return;
        }
    }
}

/* qsort */
static int compare(const void* a, const void* b)
{
    int* aa = (int*)a;
    int* bb = (int*)b;

    if (*aa > *bb)
        return 1;

    if (*aa == *bb)
        return 0;

    return -1;
}

#ifdef TEST_LOG
int main()
{
    int i;

    if (init_log(LOG_LEVEL_DEBUG, 256, ".", "test.log") < 0) {
        printf("log initialize failure.\n");
        return -1;
    }

    for (i = 0; i < 10; ++i) {
        if (i == 2)
            flush_log();

        write_log(LOG_LEVEL_DEBUG, "write log info %d.", i);
        write_log_r(LOG_LEVEL_INFO, "multi-thread write log info %d.", i);
        if (i % 100 == 0)
            rotate_log();
    }

    free_log();
}

#endif
