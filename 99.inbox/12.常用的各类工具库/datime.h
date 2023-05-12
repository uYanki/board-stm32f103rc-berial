#ifndef DATIME_H
#define DATIME_H

#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

  time_t gmt2cst(time_t gmt);
  time_t date2secs(const char *date);
  time_t http_atotm(const char *time_string);

  void *extract_date_init(void);
  char *extract_date(void *ED,const char *text,char *datebuf);
  void extract_date_free(void *ED);

#ifdef __cplusplus
}
#endif

#endif

