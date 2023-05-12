/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "comn.h"
#include "xmalloc.h"
#include "xstring.h"
#include "opt.h"

static void printseterror(opt_t* opt)
{
    if (opt->longname && opt->shortname > 0 && opt->shortname < NOT_SHORT_CAHR)
        printf("command option: --%s / -%c set error.\n", opt->longname, opt->shortname);
    else if (opt->longname)
        printf("command option: --%s set error.\n", opt->longname);
    else if (opt->shortname > 0 && opt->shortname < NOT_SHORT_CAHR)
        printf("command opiton: -%c set error.\n", opt->shortname);

    exit(1);
}

static void printnotset(opt_t* opt)
{
    if (opt->longname && opt->shortname > 0 && opt->shortname < NOT_SHORT_CAHR)
        printf("command option: --%s / -%c isn't set.\n", opt->longname, opt->shortname);
    else if (opt->longname)
        printf("command option: --%s isn't set.\n", opt->longname);
    else if (opt->shortname > 0 && opt->shortname < NOT_SHORT_CAHR)
        printf("command opiton: -%c isn't set.\n", opt->shortname);

    exit(1);
}

static void processarg(opt_t* opt, const char* optarg)
{
    int    j, ival, len;
    char*  pc;
    double dbl;

    switch (opt->argtype) {
        case ARG_INT:
            ival = atoi(optarg);
            if ((int)(opt->p2) && (ival < (int)(opt->p1) || ival > (int)(opt->p2)))
                printseterror(opt);

            *((int*)(opt->r)) = ival;
            opt->r            = NULL;  // 处理过的选项，其返回指针变量就设为NULL
            break;
        case ARG_FLOAT:
            dbl = atof(optarg);
            if ((double)(int)(opt->p2) && (dbl < (double)(int)(opt->p1) || dbl > (double)(int)(opt->p2)))
                printseterror(opt);

            *((double*)(opt->r)) = dbl;
            opt->r               = NULL;
            break;
        case ARG_STRING: {
            char** vals;
            int    valnum;
            int*   ivals;

            vals   = (char**)(opt->p1);
            valnum = (int)(opt->p2);
            ivals  = (int*)(opt->p3);

            if (!vals) {
                *((char**)(opt->r)) = xstrdup(optarg);
                opt->r              = NULL;
            } else {
                for (j = 0; j < valnum; ++j) {
                    if (strcasecmp(optarg, vals[j]) == 0)
                        break;
                }

                if (j == valnum)
                    printseterror(opt);

                if (ivals)
                    *((int*)(opt->r)) = ivals[j];
                else
                    *((char**)(opt->r)) = xstrdup(optarg);

                opt->r = NULL;
            }

            break;
        }
        case ARG_MULTI: {
            char*** mulval;
            char*   sepstr;
            int*    mvnum;
            char *  v, **vv;
            int     n;

            mulval = (char***)(opt->p1);
            sepstr = (char*)(opt->p2);
            mvnum  = (int*)(opt->p3);

            v      = xstrdup(optarg);
            n      = strfragnum(v, sepstr);
            vv     = (char**)xcalloc(n, sizeof(char*));
            split(v, sepstr, vv, n);

            for (j = 0; j < n; j++) {
                trim(vv[j]);
                if (*vv[j] == 0) {
                    xfree(v);
                    xfree(vv);
                    printseterror(opt);
                }
            }

            *((char**)(opt->r)) = v;
            *mulval             = vv;
            *mvnum              = n;
            opt->r              = NULL;
            break;
        }
        case ARG_DIR:
            len = strlen(optarg);
            pc  = xstrdup(optarg);
            if (pc[len - 1] == '/')
                pc[len - 1] = 0;
            *((char**)(opt->r)) = pc;
            opt->r              = NULL;
            break;
        case ARG_PATH: {
            char *home, *dir, path[1024];

            home = (char*)(opt->p1);
            dir  = (char*)(opt->p2);

            if (optarg[0] != '/') {
                path[0] = 0;
                if (home) {
                    strcpy(path, home);
                    strcat(path, "/");
                }
                if (dir) {
                    strcat(path, dir);
                    strcat(path, "/");
                }
                strcat(path, optarg);
                *((char**)(opt->r)) = xstrdup(path);
            } else {
                *((char**)(opt->r)) = xstrdup(optarg);
            }

            opt->r = NULL;
            break;
        }
    }
}

void getopts(int argc, char** argv, opt_t* opts, int optnum, void* help)
{
    int            i, pos, opt, longnum;
    char           optstring[1024];
    struct option* longopts        = NULL;
    void (*printhelp)(char** argv) = help;

    longnum                        = 0;
    pos                            = 0;
    for (i = 0; i < optnum; i++) {
        if (opts[i].longname)
            ++longnum;

        if (opts[i].shortname && opts[i].shortname < NOT_SHORT_CAHR) {
            optstring[pos] = opts[i].shortname;
            pos++;
            if (opts[i].hasarg) {
                optstring[pos] = ':';
                pos++;
            }
        }
    }

    optstring[pos] = 0;
    if (!longnum && !optstring[0]) {
        printf("not set any option.\n");
        exit(1);
    }

    longopts = NULL;
    if (longnum) {
        longopts = (struct option*)xcalloc(longnum + 1, sizeof(struct option));
        pos      = 0;
        for (i = 0; i < optnum; i++) {
            if (!opts[i].longname)
                continue;

            longopts[pos].name    = opts[i].longname;
            longopts[pos].has_arg = opts[i].hasarg;
            longopts[pos].flag    = NULL;
            longopts[pos].val     = opts[i].shortname;
            pos++;
        }
    }

    opterr = 0; /* Don't show error message */
    while ((opt = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
        for (i = 0; i < optnum; i++) {
            if (opt == opts[i].shortname) {
                if (opts[i].argtype == ARG_HELP)
                    printhelp(argv);

                if (opts[i].hasarg == NOARG) {
                    *((int*)(opts[i].r)) = 1;
                    opts[i].r            = NULL;
                } else if (opts[i].hasarg == HASARG) {
                    processarg(opts + i, optarg);
                }

                break;
            }
        }

        if (i == optnum && printhelp)
            printhelp(argv);
    }

    for (i = 0; i < optnum; i++) {
        // r为NULL的选项是用户当前指定了的
        if (opts[i].r == NULL)
            continue;

        if (opts[i].isnull && !opts[i].defval)  // 选项可以不指定，且没有默认值
            continue;

        // 该选项用户没有指定，但选项是不能为空的，且没有设置默认值
        if (!opts[i].defval)
            printnotset(opts + i);
        else
            processarg(opts + i, opts[i].defval);
    }

    xfree(longopts);
}

char* getprogramname(char** argv)
{
    char* name;

    name = strrchr(argv[0], PATH_SEPARATOR);
    if (!name)
        name = argv[0];
    else
        ++name;

    return name;
}

int gethomepath(const char* homeenv, char* homepath, int size)
{
    char* pc;
    int   len;

    pc = getenv(homeenv);
    if (!pc)
        return 0; /* Not set home environment variable */

    len = strlen(pc);
    if (len < size) {
        strcpy(homepath, pc);
        if (homepath[len - 1] == PATH_SEPARATOR) {
            homepath[len - 1] = 0;
            --len;
        }

        return len;
    } else {
        return -1; /* environment varialbe too long */
    }
}

#ifdef TEST_OPT

void print_help(char* argv[])
{
    printf("%s: this is help infomation...\n", getprogramname(argv));
    exit(1);
}

int main(int argc, char* argv[])
{
    const char* srunmode[] = {
        "FCGI", "CMD"};
    const int irunmode[2] = {1, 2};
    int       runmode, tn;
    char*     homepath;
    void (*help)(char**) = print_help;

    opt_t opts[]         = {
        { "runmode", 'r', HASARG, ARG_STRING, 1, "FCGI", srunmode,   (void*)2, (void*)irunmode,  &runmode},
        {"homepath", 128, HASARG,    ARG_DIR, 1,   NULL,     NULL,          0,            NULL, &homepath},
        {      NULL, 't', HASARG,    ARG_INT, 1,    "1", (void*)1, (void*)100,            NULL,       &tn},
        {    "help", 'h',  NOARG,   ARG_HELP, 1,   NULL,     NULL,       NULL,            NULL,      NULL}
    };

    getopts(argc, argv, opts, 4, help);
    return 0;
}

#endif
