/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmalloc.h"
#include "xstring.h"
#include "utils.h"
#include "comn.h"
#include "tcutil.h"
#include "charset.h"

// 测试条件是否满足
// 条件表达式: [field] op [value|$field]
// 支持的操作符:
//  int op:         > <  >= <= != ==
//  float op:       > <  >= <=
//  string op:      = ! *= *!
//  string contain: contain !contain
//  null op:        empty !empty

enum {
    OP_NO = 0,
    OP_NCT,    // !contain
    OP_CON,    // contain
    OP_NON,    // !empty
    OP_NUL,    // empty
    OP_GE,     // >=
    OP_LE,     // <=
    OP_EQ,     // ==
    OP_NE,     // !=
    OP_GT,     // >
    OP_LT,     // <
    OP_SEQNC,  // *=  no case
    OP_SNENC,  // *!  no case
    OP_SEQ,    // =
    OP_SNE     // !
};

typedef struct {
    char* n;
    int   v;
} op_t;

static op_t OPS[] = {
    {"!contain", OP_NCT  },
    {"contain",  OP_CON  },
    {"!empty",   OP_NON  },
    {"empty",    OP_NUL  },
    {">=",       OP_GE   },
    {"<=",       OP_LE   },
    {"==",       OP_EQ   },
    {"!=",       OP_NE   },
    {">",        OP_GT   },
    {"<",        OP_LT   },
    {"*=",       OP_SEQNC},
    {"*!",       OP_SNENC},
    {"=",        OP_SEQ  },
    {"!",        OP_SNE  }
};

typedef struct {
    char* field;
    int   op;
    char* value;
    int   isf;

    union {
        int    i;
        double f;
    } val;
} cond_t;

static int parse_cond(const char* cond, cond_t* c)
{
    int   i;
    char* op;

    memset(c, 0, sizeof(cond_t));

    op = NULL;
    for (i = 0; i < countof(OPS); i++) {
        if ((op = strstr(cond, OPS[i].n)))
            break;
    }

    if (!op)
        return -1;

    c->op = OPS[i].v;
    if (op == cond)
        c->field = NULL;
    else {
        c->field = xstrdupdelim(cond, op);
        trim(c->field);
    }

    op += strlen(OPS[i].n);
    if (*op) {
        c->value = xstrdup(op);
        trim(c->value);
    } else
        c->value = NULL;

    switch (c->op) {
        case OP_NUL:
        case OP_NON:
            if (c->value) goto ERR;
            break;
        case OP_GE:
        case OP_LE:
        case OP_GT:
        case OP_LT:
            if (!c->value) goto ERR;
            if (c->value[0] == '$') break;
            if (strchr(c->value, '.')) {
                c->isf   = 1;
                c->val.f = atof(c->value);
            } else {
                c->isf   = 0;
                c->val.i = atoi(c->value);
            }
            break;
        case OP_EQ:
        case OP_NE:
            if (!c->value) goto ERR;
            if (c->value[0] == '$') break;
            if (strchr(c->value, '.')) goto ERR;
            c->isf   = 0;
            c->val.i = atoi(c->value);
            break;
        case OP_SEQNC:
        case OP_SNENC:
        case OP_SEQ:
        case OP_SNE:
        case OP_CON:
        case OP_NCT:
            if (!c->value) goto ERR;
            break;
    }

    return 0;

ERR:
    xfree(c->field);
    xfree(c->value);
    return -1;
}

// 条件满足返回1，不满足返回0，错误返回-1
int testif(void* recmap, const char* field, const char* cond)
{
    const char *fv, *pv;
    cond_t      c;
    double      f;
    int         i, testres;
    char*       cs[1];

    if (!recmap || !cond || !*cond)
        return -1;

    if (parse_cond(cond, &c) < 0)
        return -1;

    if (c.field)
        fv = tcmapget2(recmap, c.field);
    else
        fv = field;

    if (c.value && c.value[0] == '$') {
        pv = tcmapget2(recmap, c.value + 1);
        if (!pv) goto ERRFREE;
        xfree(c.value);
        c.value = xstrdup(pv);
        if (c.op == OP_GE || c.op == OP_LE || c.op == OP_GT || c.op == OP_LT) {
            if (strchr(c.value, '.')) {
                c.isf   = 1;
                c.val.f = atof(c.value);
            } else {
                c.isf   = 0;
                c.val.i = atoi(c.value);
            }
        } else if (c.op == OP_EQ || c.op == OP_NE) {
            if (strchr(c.value, '.')) goto ERRFREE;
            c.isf   = 0;
            c.val.i = atoi(c.value);
        }
    }

    testres = 0;
    switch (c.op) {
        case OP_NUL:
            if (!fv || !fv[0])
                testres = 1;
            break;
        case OP_NON:
            if (fv && fv[0])
                testres = 1;
            break;
        case OP_GE:
            if (!fv || !fv[0]) break;
            if (strchr(fv, '.') || c.isf) {
                f = atof(fv);
                if (!c.isf)
                    c.val.f = atof(c.value);
                if (f >= c.val.f)
                    testres = 1;
            } else {
                i = atoi(fv);
                if (i >= c.val.i)
                    testres = 1;
            }
            break;
        case OP_LE:
            if (!fv || !fv[0]) break;
            if (strchr(fv, '.') || c.isf) {
                f = atof(fv);
                if (!c.isf)
                    c.val.f = atof(c.value);
                if (f <= c.val.f)
                    testres = 1;
            } else {
                i = atoi(fv);
                if (i <= c.val.i)
                    testres = 1;
            }
            break;
        case OP_EQ:
            if (!fv || !fv[0]) break;
            i = atoi(fv);
            if (i == c.val.i)
                testres = 1;
            break;
        case OP_NE:
            if (!fv || !fv[0]) break;
            i = atoi(fv);
            if (i != c.val.i)
                testres = 1;
            break;
        case OP_GT:
            if (!fv || !fv[0]) break;
            if (strchr(fv, '.') || c.isf) {
                f = atof(fv);
                if (!c.isf)
                    c.val.f = atof(c.value);
                if (f > c.val.f)
                    testres = 1;
            } else {
                i = atoi(fv);
                if (i > c.val.i)
                    testres = 1;
            }
            break;
        case OP_LT:
            if (!fv || !fv[0]) break;
            if (strchr(fv, '.') || c.isf) {
                f = atof(fv);
                if (!c.isf)
                    c.val.f = atof(c.value);
                if (f < c.val.f)
                    testres = 1;
            } else {
                i = atoi(fv);
                if (i < c.val.i)
                    testres = 1;
            }
            break;
        case OP_SEQNC:
            if (!fv || !fv[0]) break;
            if (strcasecmp(fv, c.value) == 0)
                testres = 1;
            break;
        case OP_SNENC:
            if (!fv || !fv[0] || strcasecmp(fv, c.value) != 0)
                testres = 1;
            break;
        case OP_SEQ:
            if (!fv || !fv[0]) break;
            if (strcmp(fv, c.value) == 0)
                testres = 1;
            break;
        case OP_SNE:
            if (!fv || !fv[0] || strcmp(fv, c.value) != 0)
                testres = 1;
            break;
        case OP_CON:
            if (!fv || !fv[0]) break;
            cs[0] = c.value;
            if (strcontain(fv, cs, 1, CODETYPE_UTF8) >= 0)
                testres = 1;
            break;
        case OP_NCT:
            cs[0] = c.value;
            if (!fv || !fv[0] || strcontain(fv, cs, 1, CODETYPE_UTF8) < 0)
                testres = 1;
            break;
    }

    xfree(c.field);
    xfree(c.value);
    return testres;

ERRFREE:
    xfree(c.field);
    xfree(c.value);
    return -1;
}

#ifdef TEST_IF

int main()
{
    TCMAP* recmap;
    recmap = tcmapnew();

    if (testif(recmap, "5", ">= 3"))
        printf("test true\n");
    else
        printf("test false\n");

    tcmapdel(recmap);
    return 0;
}

#endif
