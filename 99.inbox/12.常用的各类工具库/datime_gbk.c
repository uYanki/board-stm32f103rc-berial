#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define _XOPEN_SOURCE /* glibc2 needs this */
#define __USE_XOPEN   /* from time.h */
#include <time.h>
#include <pcreposix.h>
#include "xstring.h"
#include "comn.h"
#include "xmalloc.h"

//注意时间的时区，比如抓取时间为CST,网页的最后修改时间为GMT
time_t gmt2cst(time_t gmt)
{
  return (gmt+8*3600);  // 相差8个小时
}


//功能: 把CST日期转换为秒，参数为日期，格式为: YYYY-MM-DD HH:MM:SS
time_t date2secs(const char *date)
{
  int len, result, year, mon, day, hour, min, sec;
  char *t, d[128];
  struct tm tms;
  time_t secs;

  secs = 0;
  year = 0;
  mon = 0;
  day = 0;
  hour = 0;
  min = 0;
  sec = 0;

  len = strlen(date);
  if (len >= sizeof(d)) return 0;
  strcpy(d, date);
  trim(d);
  t = strchr(d, ' ');
  if (t)
  {
    *t = 0;
    t ++;
    trim(d);
    trim(t);

    result = sscanf(t, "%d:%d:%d", &hour, &min, &sec);
    if (result == -1)
    {
      hour = 0;
      min = 0;
      sec = 0;
    }
    else
    {
      if (hour < 0 || hour > 23) return 0;
      if (min < 0 || min > 59) return 0;
      if (sec < 0 || sec > 59) return 0;
    }
  }

  result = sscanf(d, "%d-%d-%d", &year, &mon, &day);
  if (result == -1) return 0;

  if (year < 1970) return 0;
  if (mon <= 0 || mon > 12) return 0;
  if (day < 1 || day > 31) return 0;

  tms.tm_year = year - 1900;
  tms.tm_mon = mon - 1;
  tms.tm_mday = day;
  tms.tm_hour = hour;
  tms.tm_min = min;
  tms.tm_sec = sec;
  tms.tm_isdst = 0;   // 表示Daylight Saving Time, 涉及夏时制问题

  //要考虑时区的问题。当用mktime的时候，输入的参数应该是当前的时区时间，而不是GMT时间
  //mktime()函数内部和time函数一样，也是使用了UTC，会在转换前将本地时间和UTC之间的差值扣除。
  secs = mktime(&tms);
  return secs;
}


/* Check whether the result of strptime() indicates success.
   strptime() returns the pointer to how far it got to in the string.
   The processing has been successful if the string is at `GMT' or
   `+X', or at the end of the string.

   In extended regexp parlance, the function returns 1 if P matches
   "^ *(GMT|[+-][0-9]|$)", 0 otherwise.  P being NULL (which strptime
   can return) is considered a failure and 0 is returned.  */
static int check_end(const char *p)
{
  if (!p)
    return 0;
  
  while (isspace(*p))
    ++p;
  
  if (!*p
      || (p[0] == 'G' && p[1] == 'M' && p[2] == 'T')
      || ((p[0] == '+' || p[0] == '-') && isdigit(p[1])))
    return 1;
  else
    return 0;
}

/* Convert the textual specification of time in TIME_STRING to the
   number of seconds since the Epoch.

   TIME_STRING can be in any of the three formats RFC2616 allows the
   HTTP servers to emit -- RFC1123-date, RFC850-date or asctime-date,
   as well as the time format used in the Set-Cookie header.
   Timezones are ignored, and should be GMT.

   Return the computed time_t representation, or -1 if the conversion
   fails.

   This function uses strptime with various string formats for parsing
   TIME_STRING.  This results in a parser that is not as lenient in
   interpreting TIME_STRING as I would like it to be.  Being based on
   strptime, it always allows shortened months, one-digit days, etc.,
   but due to the multitude of formats in which time can be
   represented, an ideal HTTP time parser would be even more
   forgiving.  It should completely ignore things like week days and
   concentrate only on the various forms of representing years,
   months, days, hours, minutes, and seconds.  For example, it would
   be nice if it accepted ISO 8601 out of the box.

   I've investigated free and PD code for this purpose, but none was
   usable.  getdate was big and unwieldy, and had potential copyright
   issues, or so I was informed.  Dr. Marcus Hennecke's atotm(),
   distributed with phttpd, is excellent, but we cannot use it because
   it is not assigned to the FSF.  So I stuck it with strptime.  */

time_t http_atotm(const char *time_string)
{
  /* NOTE: Solaris strptime man page claims that %n and %t match white
     space, but that's not universally available.  Instead, we simply
     use ` ' to mean "skip all WS", which works under all strptime
     implementations I've tested.  */

  static const char *time_formats[] =
  {
    "%a, %d %b %Y %T",  /* rfc1123: Thu, 29 Jan 1998 22:12:57 */
    "%A, %d-%b-%y %T",  /* rfc850:  Thursday, 29-Jan-98 22:12:57 */
    "%a %b %d %T %Y",  /* asctime: Thu Jan 29 22:12:57 1998 */
    "%a, %d-%b-%Y %T"  /* cookies: Thu, 29-Jan-1998 22:12:57 (used in Set-Cookie, defined in the Netscape cookie specification.) */
  };
  int i;

  for (i = 0; i < countof(time_formats); i++)
  {
    struct tm t;

    /* Some versions of strptime use the existing contents of struct
    tm to recalculate the date according to format.  Zero it out
    to prevent garbage from the stack influencing strptime.  */
    memset(&t,0,sizeof(t));

    /* Solaris strptime fails to recognize English month names in
    non-English locales, which we work around by not setting the
    LC_TIME category.  Another way would be to temporarily set
    locale to C before invoking strptime, but that's slow and
    messy.  GNU strptime does not have this problem because it
    recognizes English month names along with the local ones.  */

    if (check_end(strptime(time_string, time_formats[i], &t)))
      return timegm(&t);
  }

  /* All formats have failed.  */
  return -1;
}

////////////////////////////////////////////////////////////

typedef char *(*date_transfer_t)(int date_id,const char *from_date,char *to_date);
static char *date_transfer(int date_id,const char *from_date,char *to_date);
/*
  YYYY: 1990~1999 2000~2019 90~99 00~19 ==> (?:199\d|20[01]\d|9\d|[01]\d)
    MM: 01~09 1~9 10~12                 ==> (?:[1-9]|0[1-9]|1[0-2])
    DD: 01~09 1~9 10~29 30 31           ==> (?:[1-9]|0[1-9]|[12]\d|3[01])

  YYYY年MM月DD[日]:
  (?:199\d|20[01]\d|9\d|[01]\d)年(?:[1-9]|0[1-9]|1[0-2])月(?:[1-9]|0[1-9]|[12]\d|3[01])日?

  MM月DD日YYYY年:
  (?:[1-9]|0[1-9]|1[0-2])月(?:[1-9]|0[1-9]|[12]\d|3[01])日(?:199\d|20[01]\d|9\d|[01]\d)年?
  
  YYYY-MM-DD YYYY/MM/DD YYYY.MM.DD YYYY_MM_DD YYYY MM DD: 
  (?:199\d|20[01]\d|9\d|[01]\d)(-|/|\.|_| )(?:[1-9]|0[1-9]|1[0-2])\1(?:[1-9]|0[1-9]|[12]\d|3[01])

  MM-DD-YYYY MM/DD/YYYY MM.DD.YYYY MM_DD_YYYY MM DD YYYY:
  (?:[1-9]|0[1-9]|1[0-2])(-|/|\.|_| )(?:[1-9]|0[1-9]|[12]\d|3[01])\1(?:199\d|20[01]\d|9\d|[01]\d)

  MM DD,YYYY
  (?:[1-9]|0[1-9]|1[0-2]) (?:[1-9]|0[1-9]|[12]\d|3[01]),(?:199\d|20[01]\d|9\d|[01]\d)

  YYYYMMDD only support: 19990101 20121201
  (?:199\d|20[01]\d)(?:0[1-9]|1[0-2])(?:0[1-9]|[12]\d|3[01])

  MMDDYYYY only support: 01011999 12012012
  (?:0[1-9]|1[0-2])(?:0[1-9]|[12]\d|3[01])(?:199\d|20[01]\d)
*/

struct date {
  char *regexp;
  int id;
  date_transfer_t transfer;
};

static const struct date DATES[] = {
  {"(?:199\\d|20[01]\\d|9\\d|[01]\\d)年(?:[1-9]|0[1-9]|1[0-2])月(?:[1-9]|0[1-9]|[12]\\d|3[01])日?",0,date_transfer},
  {"(?:[1-9]|0[1-9]|1[0-2])月(?:[1-9]|0[1-9]|[12]\\d|3[01])日(?:199\\d|20[01]\\d|9\\d|[01]\\d)年?", 1,date_transfer},
  {"(?:199\\d|20[01]\\d|9\\d|[01]\\d)(-|/|\\.|_| )(?:[1-9]|0[1-9]|1[0-2])\1(?:[1-9]|0[1-9]|[12]\\d|3[01])",2,date_transfer},
  {"(?:[1-9]|0[1-9]|1[0-2])(-|/|\\.|_| )(?:[1-9]|0[1-9]|[12]\\d|3[01])\1(?:199\\d|20[01]\\d|9\\d|[01]\\d)",3,date_transfer},
  {"(?:[1-9]|0[1-9]|1[0-2]) (?:[1-9]|0[1-9]|[12]\\d|3[01]),(?:199\\d|20[01]\\d|9\\d|[01]\\d)",4,date_transfer},
  {"(?:199\\d|20[01]\\d)(?:0[1-9]|1[0-2])(?:0[1-9]|[12]\\d|3[01])",5,date_transfer},
  {"(?:0[1-9]|1[0-2])(?:0[1-9]|[12]\\d|3[01])(?:199\\d|20[01]\\d)",6,date_transfer}
};

#define TO_YYYY(b,e,l,n,to_date)  do { \
  l = e - b; \
  if(l == 2 && *b == '9') { \
    memcpy(to_date+n,"19",2); \
    n += 2; \
  } else if(l == 2) { \
    memcpy(to_date+n,"20",2); \
    n += 2; \
  } \
  memcpy(to_date+n,b,l); \
  n += l; \
  to_date[n++] = '-'; \
} while(0)

#define TO_M(b,e,l,n,to_date)  do { \
  l = e - b; \
  if(l == 2 && *b == '0') { \
   ++b; \
   --l; \
  } \
  memcpy(to_date+n,b,l); \
  n += l; \
  to_date[n++] = '-'; \
} while(0)

#define TO_D(b,e,l,n,to_date) do { \
  l = e - b; \
  if(l == 2 && *b == '0') { \
    ++b; \
    --l; \
  } \
  memcpy(to_date+n,b,l); \
  n += l; \
  to_date[n++] = '\0'; \
} while(0)

// 转换为标准形式为: YYYY-MM-DD YYYY-M-D
static char *date_transfer(int date_id,const char *from_date,char *to_date)
{
  int l,n;
  const char *b,*e;

  if(!from_date || !*from_date || !to_date)
    return NULL;

  n = 0;
  switch(date_id) {
    case 0: // YYYY年MM月DD[日]
      b = from_date;
      e = strstr(b,"年"); if(!e) return NULL;
      TO_YYYY(b,e,l,n,to_date);
      
      b = e+strlen("年");
      e = strstr(b,"月"); if(!e) return NULL;
      TO_M(b,e,l,n,to_date);
      
      b = e+strlen("月");
      e = strstr(b,"日"); if(!e) e = from_date+strlen(from_date);
      TO_D(b,e,l,n,to_date);
      break;
    case 1: // MM月DD日YYYY[年]
      b = strstr(from_date,"日"); if(!b) return NULL;
      b += strlen("日");
      e = strstr(b,"年"); if(!e) e = from_date+strlen(from_date);
      TO_YYYY(b,e,l,n,to_date);

      b = from_date;
      e = strstr(b,"月"); if(!e) return NULL;
      TO_M(b,e,l,n,to_date);

      b = e+strlen("月");
      e = strstr(b,"日"); if(!e) return NULL;
      TO_D(b,e,l,n,to_date);
      break;   
    case 2: // YYYY-MM-DD YYYY/MM/DD YYYY.MM.DD YYYY_MM_DD YYYY MM DD YYYYMMDD
      b = from_date;
      e = strpbrk(b,"-/._ "); if(!e) return NULL;
      TO_YYYY(b,e,l,n,to_date);

      b = ++e;
      e = strpbrk(b,"-/._ "); if(!e) return NULL;
      TO_M(b,e,l,n,to_date);

      b = ++e;
      e = from_date+strlen(from_date);
      TO_D(b,e,l,n,to_date);
      break;
    case 3: // MM-DD-YYYY MM/DD/YYYY MM.DD.YYYY MM_DD_YYYY MM DD YYYY MMDDYYYY
      b = strpbrk(from_date,"-/._ "); if(!b) return NULL;
      b = strpbrk(++b,"-/._ "); if(!b) return NULL;
      ++b;
      e = from_date+strlen(from_date);
      TO_YYYY(b,e,l,n,to_date);

      b = from_date;
      e = strpbrk(b,"-/._ "); if(!e) return NULL;
      TO_M(b,e,l,n,to_date);

      b = ++e;
      e = strpbrk(b,"-/._ "); if(!e) return NULL;
      TO_D(b,e,l,n,to_date);
      break; 
    case 4: // MM DD,YYYY
      b = strchr(from_date,','); if(!b) return NULL;
      ++b;
      e = from_date+strlen(from_date);
      TO_YYYY(b,e,l,n,to_date);

      b = from_date;
      e = strchr(b,' '); if(!e) return NULL;
      TO_M(b,e,l,n,to_date);

      b = ++e;
      e = strchr(b,','); if(!e) return NULL;
      TO_D(b,e,l,n,to_date);
      break;
    case 5: // YYYYMMDD
      b = from_date;
      e = b+4;
      TO_YYYY(b,e,l,n,to_date);

      b = e;
      e = b+2;
      TO_M(b,e,l,n,to_date);

      b = e;
      e = b+2;
      TO_D(b,e,l,n,to_date);
      break;
    case 6: // MMDDYYYY
      b = from_date+4;
      e = b+4;
      TO_YYYY(b,e,l,n,to_date);

      b = from_date;
      e = b+2;
      TO_M(b,e,l,n,to_date);

      b = e;
      e = b+2;
      TO_D(b,e,l,n,to_date);
      break;
    default:
      return NULL;
  }

  return to_date;
}

struct extract_date {
  int renum;
  regex_t *res;
};

void *extract_date_init(void)
{
  int i,j,options;
  struct extract_date *ed;

  ed = (struct extract_date *)xcalloc(1,sizeof(struct extract_date));
  ed->renum = countof(DATES);
  ed->res = (regex_t *)xcalloc(ed->renum,sizeof(regex_t));

  options = REG_EXTENDED | REG_UTF8; 
  for(i=0;i<ed->renum;i++) {
    if(regcomp(ed->res+i, DATES[i].regexp, options) != 0)
      goto EXIT;
  }

  return ed;

EXIT:
  for(j=0;j<i;j++)
    regfree(ed->res+j);
  xfree(ed->res);
  xfree(ed);
  return NULL;
}

void extract_date_free(void *ED)
{
  int i;
  struct extract_date *ed = (struct extract_date *)ED;

  for(i=0;i<ed->renum;i++)
    regfree(ed->res+i);
  xfree(ed->res);
  xfree(ed);
}

char *extract_date(void *ED,const char *text,char *datebuf)
{
  int i,sublen;
  regmatch_t subs[32]; 
  char frombuf[32];
  struct extract_date *ed = (struct extract_date *)ED;

  if(!text || !*text || !datebuf)
    return NULL;

  datebuf[0] = 0;
  for(i=0;i<ed->renum;i++) {
    if(regexec(ed->res+i, text, 32, subs, 0) == 0) {
      if(subs[0].rm_so < 0) continue;
      sublen = subs[0].rm_eo - subs[0].rm_so;
      if(sublen >= sizeof(frombuf)) continue;
      memcpy(frombuf,text+subs[0].rm_so,sublen);
      frombuf[sublen] = 0;
      if(DATES[i].transfer)
        DATES[i].transfer(DATES[i].id,frombuf,datebuf);
      else
        strcpy(datebuf,frombuf);
      break;
    }
  }

  if(datebuf[0]) 
    return datebuf;
  else
    return NULL;
}

#ifdef TEST_DATIME

int main()
{
  void *ed;
  char buf[1024],*r,datebuf[32];
  
  printf("%ld\n",date2secs("2011-04-05 10:33:28"));
  printf("%ld\n",date2secs("2011-4-5 10:33:28"));
  printf("%ld\n",http_atotm("Fri, 15 Apr 2011 2:33:28 GMT"));

  ed = extract_date_init();
  if(!ed) return 0;

  while(fgets(buf,1024,stdin))
  {
    trim(buf);
    if(strcasecmp("exit",buf) == 0)
      break;
    r = extract_date(ed,buf,datebuf);
    if(r)
      printf("DATE: %s\n",datebuf);
    else
      printf("No DATE\n");
  }
  
  extract_date_free(ed);
  return 0;
}

#endif

