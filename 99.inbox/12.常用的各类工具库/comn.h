/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef COMN_H
#define COMN_H

// types
#ifndef _STDINT_H
typedef signed char         int8_t;
typedef uint8_t       uint8_t;
typedef short               int16_t;
typedef uint16_t      uint16_t;
typedef int                 int32_t;
typedef uint32_t            uint32_t;
typedef long long           int64_t;
typedef unsigned long  uint64_t;
#endif
typedef int                 bool_t;
typedef int                 sock_t;

// constants
enum {
  GK_FALSE = 0,
  GK_TRUE = 1,
  GK_FAILURE = -1,
  GK_SUCCESS = 0,
  GK_ERROR = -1,
  GK_OK = 0,
  GK_INEXIST = 0,
  GK_EXIST = 1
};

// macros
#define COUNTOF(array)        (sizeof (array) / sizeof ((array)[0]))
#define countof(array)        (sizeof (array) / sizeof ((array)[0]))
#define TOLOWER(x)            ('A' <= (x) && (x) <= 'Z' ? (x) + 32 : (x))
#define XZERO(x)              memset (&(x), '\0', sizeof (x))
#define ISSPACE(x)            isspace(x)
#define MAX(a,b)              (((a) >= (b)) ? (a) : (b))
#define MIN(a,b)              (((a) <= (b)) ? (a) : (b))
#define ISGBK(ch)             ((uint8_t)ch & 0x80)
#define DIRISEXIST(dir)       (access(dir,F_OK) == 0)
#define FILEISEXIST(file)     (access(file,F_OK) == 0)
#define strcasebeginwith(a,b) (strncasecmp(b,a,strlen(a)) == 0)
#define strcaseequal(s1,s2)   (strcasecmp(s1,s2) == 0)

// ports
#ifdef WIN32
#define PATH_SEPARATOR      '\\'
#define PATH_SEPARATOR_STR  "\\"
#define strcasecmp          stricmp
#define strncasecmp         strnicmp
#define snprintf            _snprintf
#define vsnprintf           _vsnprintf
#define bzero(x, y)         memset((x), 0, (y))
#define index               strchr
#define rindex              strrchr
#else
#define PATH_SEPARATOR      '/'
#define PATH_SEPARATOR_STR  "/"
#endif

// limits
#define MAX_URL_LEN     1024
#define MAX_LINE_LEN    1024
#define MAX_PATH_LEN    255
#define MAX_NAME_LEN    255
#define BUFSIZE_1M      (1*1024*1024)
#define BUFSIZE_4M      (4*1024*1024)

// buffer
typedef struct {
  char *p;
  int size;
  int free;
  int pos;
  int reserve1;
  int reserve2;
  int reserve3;
}buffer_t;

#define INIT_BUFFER(BUFFER,SIZE) {\
  BUFFER.p = (char *)xcalloc(1,(SIZE)+1);\
  BUFFER.size = (SIZE);\
  BUFFER.free = (SIZE);\
  BUFFER.pos = 0;\
  BUFFER.reserve1 = 0;\
  BUFFER.reserve2 = 0;\
  BUFFER.reserve3 = 0;\
}

#define CLEAR_BUFFER(BUFFER,SIZE) {\
  BUFFER.free = (SIZE);\
  BUFFER.pos = 0;\
  BUFFER.reserve1 = 0;\
  BUFFER.reserve2 = 0;\
  BUFFER.reserve3 = 0;\
}

#define RESIZE_BUFFER(BUFFER,OLD_SIZE,RE_SIZE) {\
  BUFFER.p = (char *)xrealloc(BUFFER.p,(OLD_SIZE) + (RE_SIZE) +1);\
  BUFFER.size = (OLD_SIZE) + (RE_SIZE);\
  BUFFER.free += (RE_SIZE);\
}

#define RESUME_BUFFER(BUFFER,SIZE) {\
  if(BUFFER.size != (SIZE)) {\
    BUFFER.p = (char *)xrealloc(BUFFER.p,(SIZE)+1);\
    BUFFER.size = (SIZE);\
  }\
  BUFFER.free = (SIZE);\
  BUFFER.pos = 0;\
  BUFFER.reserve1 = 0;\
  BUFFER.reserve2 = 0;\
  BUFFER.reserve3 = 0;\
}

//注意: 这是一个复合语句，注意在 if while for 等语句中用{}包含起来
//增加appstrlen变量的目的是防止STR_LEN=strlen(),因为这种情况下要调用9次。
#define APPEND_BUFFER(BUFFER,STR,STR_LEN) {\
  int appstrlen = (int)(STR_LEN); \
  if(BUFFER.free <= appstrlen) {\
    BUFFER.p = (char *)xrealloc(BUFFER.p,BUFFER.size + appstrlen +1);\
    BUFFER.size = BUFFER.size + appstrlen;\
    BUFFER.free += appstrlen;\
  }\
  memcpy(BUFFER.p + BUFFER.pos,STR,appstrlen);\
  *(BUFFER.p+BUFFER.pos+appstrlen) = 0;\
  BUFFER.pos += appstrlen;\
  BUFFER.free -= appstrlen;\
}

// 拷贝字符串包括\0到buffer,如果空间不足则设置RETURN=NULL,否则RETURN=字符串指针
#define STRCPY_BUFFER(BUFFER,STR,RETURN) do {\
  int cpystrlen = strlen(STR); \
  if(BUFFER.free <= cpystrlen) {\
    RETURN = NULL;\
  } else { \
    RETURN = BUFFER.p + BUFFER.pos;\
    memcpy(BUFFER.p + BUFFER.pos,STR,cpystrlen);\
    *(BUFFER.p+BUFFER.pos+cpystrlen) = 0;\
    BUFFER.pos += (cpystrlen+1);\
    BUFFER.free -= (cpystrlen+1);\
  } \
} while(0)

#define STRCAT_BUFFER(BUFFER,STR,RETURN) do {\
    int catstrlen = strlen(STR); \
    if(BUFFER.pos && *(BUFFER.p+BUFFER.pos-1) == '\0') { \
      BUFFER.pos--; \
      BUFFER.free++; \
    } \
    if(BUFFER.free <= catstrlen) {\
      RETURN = NULL;\
    } else { \
      RETURN = BUFFER.p + BUFFER.pos;\
      memcpy(BUFFER.p + BUFFER.pos,STR,catstrlen);\
      *(BUFFER.p+BUFFER.pos+catstrlen) = 0;\
      BUFFER.pos += (catstrlen+1);\
      BUFFER.free -= (catstrlen+1);\
    } \
  } while(0)

// 拷贝字符串包括\0到buffer,如果空间不足则设置RETURN=NULL,否则RETURN=字符串指针
#define STRCPY2_BUFFER(BUFFER,STR,LEN,RETURN) do {\
      int cpystrlen = (LEN); \
      if(BUFFER.free <= cpystrlen) {\
        RETURN = NULL;\
      } else { \
        RETURN = BUFFER.p + BUFFER.pos;\
        memcpy(BUFFER.p + BUFFER.pos,STR,cpystrlen);\
        *(BUFFER.p+BUFFER.pos+cpystrlen) = 0;\
        BUFFER.pos += (cpystrlen+1);\
        BUFFER.free -= (cpystrlen+1);\
      } \
    } while(0)
    
#define STRCAT2_BUFFER(BUFFER,STR,LEN,RETURN) do {\
        int catstrlen = (LEN); \
        if(BUFFER.pos && *(BUFFER.p+BUFFER.pos-1) == '\0') { \
          BUFFER.pos--; \
          BUFFER.free++; \
        } \
        if(BUFFER.free <= catstrlen) {\
          RETURN = NULL;\
        } else { \
          RETURN = BUFFER.p + BUFFER.pos;\
          memcpy(BUFFER.p + BUFFER.pos,STR,catstrlen);\
          *(BUFFER.p+BUFFER.pos+catstrlen) = 0;\
          BUFFER.pos += (catstrlen+1);\
          BUFFER.free -= (catstrlen+1);\
        } \
      } while(0)
      

#define FREE_BUFFER(BUFFER) xfree(BUFFER.p);

#endif
