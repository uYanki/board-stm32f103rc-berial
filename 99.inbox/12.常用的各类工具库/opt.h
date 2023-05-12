/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef OPT_H
#define OPT_H

#define NOT_SHORT_CAHR 128

enum {
    NOARG  = 0,  // no argumnet option,as ARG_INT type.
    HASARG = 1
};

enum {
    ARG_HELP = 0,
    ARG_INT,
    ARG_FLOAT,
    ARG_STRING,
    ARG_MULTI,
    ARG_DIR,
    ARG_PATH
};

/*
 * 选项分为两类: 必选，可选。开发者需要知道哪些是必选的，哪些是可选的。
 * 选项取值来源只有两个: 要么由用户指定，要么采用默认值。
 * 必选选项必须有值,可选选项则可有可无。
 * isnull仅仅指的是这个选项是否一定要求用户指定取值。只用于没有默认值的必选选项。
 * defval指的是如果用户没有指定，则采用默认值。
 */

typedef struct
{
    char* longname;   // 长选项名
    int   shortname;  // 短选项名，如果不支持短选项名则设置 > 128
    int   hasarg;     // 选项后是否带有参数，没有参数的选项，则默认为带有整型参数
    int   argtype;    // 参数类型
    int   isnull;     // 指明该选项值是否必须由用户指定，用于没有默认值的必选选项。
    char* defval;     // 选项默认值，即用户没有指定该选项时默认值。
    void* p1;         // int:int min;multi:char ***mulval;string:char **vals; path: char*home;
    void* p2;         // int:int max;multi:char *sepstr;  string:int valnum;  path: char*dir;
    void* p3;         // int:NULL;   multi:int  *mvnum;   string:int *ivals;  path: NULL
    void* r;          // 选项返回值
} opt_t;

#ifdef __cplusplus
extern "C" {
#endif

void  getopts(int argc, char** argv, opt_t* opts, int optnum, void* help);
char* getprogramname(char** argv);
int   gethomepath(const char* homeenv, char* homepath, int size);

#ifdef __cplusplus
}
#endif

#endif
