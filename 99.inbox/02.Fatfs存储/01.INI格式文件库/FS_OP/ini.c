/*
 *	This is a simple parser for .INI files.
 *	It is based around the syntax described in the Wikipedia entry at
 *	"http://en.wikipedia.org/wiki/INI_file"
 *
 * See ini.h for more info
 *
 * This is free and unencumbered software released into the public domain.
 * http://unlicense.org/
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <setjmp.h>

#include <assert.h> /* Remember to define NDEBUG for release */

#include "ini.h"
#include "utils.h"
#include "fatfs.h"

/* Maximum number of characters expected on a line.
It is only used by the ini_putf() function.
*/
#define MAX_LINE 1024

void __aeabi_assert(const char* s1, const char* s2, int len)
{
    printf(">> error: (%s). function: %s, line %d\r\n", s1, s2, len);
}

/* Various error codes */
#define SUCCESS                1
#define FILE_CREATED           0
#define NO_SUCH_FILE           -1
#define OUT_OF_MEMORY          -2
#define MISSING_END_BRACE      -3
#define EMPTY_SECTION          -4
#define EXPECTED_EQUALS        -5
#define EXPECTED_END_OF_STRING -6
#define ER_FOPEN               -7
#define BAD_SYMBOL             -8
#define EXPECTED_PARAMETER     -9
#define EXPECTED_VALUE         -10

const char* ini_errstr(int err)
{
    switch (err) {
        case SUCCESS:
            return "Success";

        case FILE_CREATED:
            return "New INI object created";

        case NO_SUCH_FILE:
            return "Unable to open file";

        case OUT_OF_MEMORY:
            return "Out of memory";

        case MISSING_END_BRACE:
            return "Missing ']' at end of section";

        case EMPTY_SECTION:
            return "Empty [] for section";

        case EXPECTED_EQUALS:
            return "Expected an '='/':'";

        case EXPECTED_END_OF_STRING:
            return "Expected an end of string";

        case ER_FOPEN:
            return "Unable to open file";

        case BAD_SYMBOL:
            return "Bad symbol";

        case EXPECTED_PARAMETER:
            return "Expected a parameter (or section)";

        case EXPECTED_VALUE:
            return "Expected a value";
    }

    return "Unknown";
}

/** Configurable parameters *************************************************/

/*
 *	Recursively adds sections to the tree of sections
 */
static void insert_section(ini_section* r, ini_section* n)
{
    assert(r);
    assert(n);

    if (my_stricmp(r->name, n->name) < 0) {
        if (!r->left)
            r->left = n;
        else
            insert_section(r->left, n);
    } else {
        if (!r->right)
            r->right = n;
        else
            insert_section(r->right, n);
    }
}

/*
 *	Searches a tree of pairs for a specific parameter
 */
static ini_pair* find_pair(ini_pair* root, const char* name)
{
    int c;

    if (!root)
        return NULL;

    c = my_stricmp(root->param, name);

    if (c == 0)
        return root;
    else if (c < 0)
        return find_pair(root->left, name);
    else
        return find_pair(root->right, name);
}

/*
 *	Searches for a specific section
 */
static ini_section* find_section(ini_section* root, const char* name)
{
    int c;

    if (!root)
        return NULL;

    c = my_stricmp(root->name, name);

    if (c == 0)
        return root;
    else if (c < 0)
        return find_section(root->left, name);
    else
        return find_section(root->right, name);
}

/*
 *	Creates a new section, and adds it to a tree of sections
 */
static ini_section* add_section(ini_section** root, char* name)
{
    ini_section* n;

    assert(root);
    assert(name);

    n = find_section(*root, name);

    if (n) {
        free(name);
        return n;
    }

    n = malloc(sizeof *n);

    if (!n)
        return NULL;

    n->name   = name;

    n->fields = NULL;
    n->left = n->right = NULL;

    if (*root)
        insert_section(*root, n);
    else
        *root = n;

    return n;
}

/*
 *	Inserts a new pair n into a pair tree p
 */
static void insert_pair(ini_pair* p, ini_pair* n)
{
    if (my_stricmp(p->param, n->param) < 0) {
        if (!p->left)
            p->left = n;
        else
            insert_pair(p->left, n);
    } else {
        if (!p->right)
            p->right = n;
        else
            insert_pair(p->right, n);
    }
}

/*
 *	Adds a parameter-value pair to section s
 */
static ini_pair* add_pair(ini_section* s, char* p, char* v)
{
    ini_pair* n;

    assert(s);

    n = malloc(sizeof *n);

    if (!n)
        return NULL;

    n->param = p;
    n->value = v;

    n->left = n->right = NULL;

    if (!s->fields)
        s->fields = n;
    else
        insert_pair(s->fields, n);

    return n;
}

/** Functions for memory deallocation ***************************************/

/*
 *	Free's a tree of parameter-value pairs
 */
static void free_pair(ini_pair* p)
{
    if (!p)
        return;

    free_pair(p->left);
    free_pair(p->right);

    free(p->param);
    free(p->value);
    free(p);
}

/*
 *	Free's all the memory allocated to a ini_section s
 */
static void free_section(ini_section* s)
{
    if (!s)
        return;

    free_section(s->left);
    free_section(s->right);

    free(s->name);
    free_pair(s->fields);
    free(s);
}

/*
 *	Free's all the memory allocated to a ini_file object in ini_read()
 */
void ini_free(struct ini_file* ini)
{
    if (!ini)
        return;

    free_pair(ini->globals);
    free_section(ini->sections);
    free(ini);
}

/** Parsing functions *******************************************************/

static struct ini_file* make_ini()
{
    struct ini_file* ini = malloc(sizeof *ini);

    if (!ini)
        return NULL;

    ini->globals  = NULL;
    ini->sections = NULL;
    return ini;
}

/*
 *	Reads an INI file and returns it as a ini_file object.
 *	If filename is NULL, an empty ini_file object is created and returned.
 */
struct ini_file* ini_read(const char* filename, int* err, int* line)
{
    if (line)
        *line = 0;

    if (!filename) {
        if (err)
            *err = FILE_CREATED;

        return make_ini();
    } else {
        char* text = my_readfile(filename);

        if (!text) {
            if (err)
                *err = NO_SUCH_FILE;

            return NULL;
        }

        struct ini_file* ini = ini_parse(text, err, line);

        free(text);

        return ini;
    }
}

#define T_END   0
#define T_VALUE 1

static int get_token(const char** tp, const char** tstart, const char** tend, int* line, jmp_buf err)
{
    /* *tstart points to the start of the token, while *tend points one char past the end */

    const char* t   = *tp;
    int         tok = T_END;

    assert(tp && tstart && tend);

whitespace:

    while (isspace(t[0])) {
        if (t[0] == '\n' && line)
            (*line)++;

        t++;
    }

    if (t[0] == ';' || t[0] == '#') {
        while (t[0] != '\n' && t[0] != '\0')
            t++;

        goto whitespace;
    }

    *tstart = t;
    *tend   = t;

    if (t[0]) {
        if (strchr("[]:=", t[0])) {
            tok = *t++;
        } else if (isgraph(t[0]) && !strchr("\"'[];#", t[0])) {
            while (isgraph(t[0]) && !strchr("\"'[];#", t[0])) {
                t++;
            }

            *tend = t;
            tok   = T_VALUE;
        } else if (t[0] == '\"' || t[0] == '\'') {
            char delim = t[0];

            if (t[1] == delim && t[2] == delim) {
                /* """Python style long strings""" */
                t += 3;
                *tstart = t;

                while (!(t[0] == delim && t[1] == delim && t[2] == delim)) {
                    if (t[0] == '\0') {
                        longjmp(err, EXPECTED_END_OF_STRING);
                    } else if (t[0] == '\\')
                        t++;

                    t++;
                }

                *tend = t;
                t += 3;
            } else {
                *tstart = ++t;

                while (t[0] != delim) {
                    if (t[0] == '\0' || t[0] == '\n') {
                        longjmp(err, EXPECTED_END_OF_STRING);
                    } else if (t[0] == '\\')
                        t++;

                    t++;
                }

                *tend = t++;
            }

            tok = T_VALUE;
        } else {
            /* Unrecognized token */
            longjmp(err, BAD_SYMBOL);
        }
    }

    *tp = t;
    return tok;
}

static char* get_string(const char* tstart, const char* tend, jmp_buf err)
{
    char *      string, *s;
    const char* i;

    assert(tend > tstart);
    string = malloc(tend - tstart + 1);

    if (!string)
        longjmp(err, OUT_OF_MEMORY);

    for (i = tstart, s = string; i < tend; i++) {
        if (i[0] == '\\') {
            switch (*++i) {
                case '\\':
                case '\'':
                case '\"':
                    *s++ = i[0];
                    break;

                case 'r':
                    *s++ = '\r';
                    break;

                case 'n':
                    *s++ = '\n';
                    break;

                case 't':
                    *s++ = '\t';
                    break;

                case '0':
                    *s++ = '\0';
                    break;

                default:
                    break;
            }
        } else {
            *s++ = i[0];
        }
    }

    assert(s - string <= tend - tstart);
    s[0] = '\0';
    return string;
}

struct ini_file* ini_parse(const char* text, int* err, int* line)
{
    jmp_buf on_error;
    int     e_code;

    struct ini_file* ini     = NULL;
    ini_section*     cur_sec = NULL;

    const char *tstart, *tend;

    int t;

    if (err)
        *err = SUCCESS;

    if (line)
        *line = 1;

    ini = make_ini();

    if ((e_code = setjmp(on_error)) != 0) {
        if (err)
            *err = e_code;

        ini_free(ini);
        return NULL;
    }

    while ((t = get_token(&text, &tstart, &tend, line, on_error)) != T_END) {
        if (t == '[') {
            char* section_name;

            if (get_token(&text, &tstart, &tend, line, on_error) != T_VALUE) {
                longjmp(on_error, EMPTY_SECTION);
            }

            section_name = get_string(tstart, tend, on_error);

            cur_sec      = add_section(&ini->sections, section_name);

            if (!cur_sec)
                longjmp(on_error, OUT_OF_MEMORY);

            if (get_token(&text, &tstart, &tend, line, on_error) != ']') {
                longjmp(on_error, MISSING_END_BRACE);
            }
        } else if (t == T_VALUE) {
            char *par, *val;
            par = get_string(tstart, tend, on_error);
            t   = get_token(&text, &tstart, &tend, line, on_error);

            if (t != '=' && t != ':') {
                longjmp(on_error, EXPECTED_EQUALS);
            }

            if (get_token(&text, &tstart, &tend, line, on_error) != T_VALUE) {
                longjmp(on_error, EXPECTED_VALUE);
            }

            val = get_string(tstart, tend, on_error);

            if (cur_sec)
                add_pair(cur_sec, par, val);
            else {
                /* Add the parameter and value to the INI file's globals */
                ini_pair* pair;
                pair = malloc(sizeof *pair);

                if (!pair)
                    longjmp(on_error, OUT_OF_MEMORY);

                pair->param = par;
                pair->value = val;

                pair->left = pair->right = NULL;

                if (!ini->globals)
                    ini->globals = pair;
                else
                    insert_pair(ini->globals, pair);
            }
        } else
            longjmp(on_error, EXPECTED_PARAMETER);
    }

    return ini;
}

/** Printing functions ******************************************************/

static void string_to_file(FIL* f, const char* s)
{
    // f_putc('\"', f);
    for (; s[0]; s++) {
        switch (s[0]) {
            case '\n':
                f_puts("\\n", f);
                break;

            case '\r':
                f_puts("\\r", f);
                break;

            case '\t':
                f_puts("\\t", f);
                break;

            case '\"':
                f_puts("\\\"", f);
                break;

            case '\'':
                f_puts("\\\'", f);
                break;

            case '\\':
                f_puts("\\\\", f);
                break;

            default:
                f_putc(s[0], f);
                break;
        }
    }

    // f_putc('\"', f);
}

/*
 *	Recursively prints a tree of ini_pairs
 */
static void write_pair(ini_pair* p, FIL* f)
{
    if (!p)
        return;

    string_to_file(f, p->param);
    f_puts(" = ", f);
    string_to_file(f, p->value);
    f_putc('\n', f);

    write_pair(p->left, f);
    write_pair(p->right, f);
}

/*
 *	Recursively prints a tree of INI sections
 */
static void write_section(ini_section* s, FIL* f)
{
    if (!s)
        return;

    f_puts("\n[", f);
    string_to_file(f, s->name);
    f_puts("]\n", f);

    write_pair(s->fields, f);

    /* The akward sequence is to ensure that values are not written sorted */

    write_section(s->left, f);
    write_section(s->right, f);
}

/*
 *	Saves all the sections and parameters in an ini_file to a file.
 *	If fname is NULL, it is written to stdout.
 */
int ini_write(struct ini_file* ini, const char* fname)
{
    FIL* f = &USERFile;

    if (fname) {
        if (f_open(f, fname, FA_WRITE) != FR_OK) {
            return ER_FOPEN;
        }
    } else {
        return EXPECTED_PARAMETER;
    }

    write_pair(ini->globals, f);
    write_section(ini->sections, f);

    f_close(f);

    // if (fname)
    // {
    // 	f = fopen(fname, "w");
    // 	if (!f)
    // 		return ER_FOPEN;
    // }
    // else
    // 	f = stdout;

    // write_pair(ini->globals, f);
    // write_section(ini->sections, f);

    // if (fname)
    // 	fclose(f);

    return SUCCESS;
}

/****************************************************************************/

int ini_has_section(struct ini_file* ini, const char* sec)
{
    return find_section(ini->sections, sec) != NULL;
}

/*
 *	Finds a specific parameter-value pair in the configuration file
 */
static ini_pair* find_param(const struct ini_file* ini,
                            const char*            sec,
                            const char*            par)
{
    ini_section* s;
    ini_pair*    p;

    if (!ini)
        return NULL;

    if (sec) {
        s = find_section(ini->sections, sec);

        if (!s)
            return NULL;

        p = s->fields;
    } else
        p = ini->globals;

    if (!p)
        return NULL;

    return find_pair(p, par);
}

/*
 *	Retrieves a parameter 'par' from a section 'sec' within the ini_file 'ini'
 *	and returns its value.
 *	If 'sec' is NULL, the global parameters ini 'ini' are searched.
 *	If the value is not found, 'def' is returned.
 *	It returns a string. Functions like atoi(), atof(), strtol() and even
 *	sscanf() can be used to convert it to the relevant type.
 */
const char* ini_get(struct ini_file* ini,
                    const char*      sec,
                    const char*      par,
                    const char*      def)
{
    ini_pair* p;

    p = find_param(ini, sec, par);

    if (!p) {
        if (def)
            ini_put(ini, sec, par, def);

        return def;
    }

    return p->value;
}

/*
 *	Sets a parameter 'par' in section 'sec's value to 'val', replacing the
 *	current value if it already exists, or creates the section if it does not
 *	exist
 */
int ini_put(struct ini_file* ini, const char* sec, const char* par, const char* val)
{
    ini_section* s;
    ini_pair *   p, **pp;

    if (!ini || !val)
        return 0;

    p = find_param(ini, sec, par);

    if (p) {
        /* Replace the existing value */
        char* t  = p->value;
        p->value = my_strdup(val);

        if (!p->value) {
            p->value = t;
            return 0;
        }

        free(t);
        return 1;
    }

    if (sec) {
        s = find_section(ini->sections, sec);

        if (!s) {
            /* Create a new section */
            s = malloc(sizeof *s);

            if (!s)
                return 0;

            s->name = my_strdup(sec);

            if (!s->name) {
                free(s);
                return 0;
            }

            s->fields = NULL;
            s->left = s->right = NULL;

            if (ini->sections)
                insert_section(ini->sections, s);
            else
                ini->sections = s;
        }

        pp = &s->fields;
    } else
        pp = &ini->globals;

    p = malloc(sizeof *p);

    if (!p)
        return 0;

    p->param = my_strdup(par);
    p->value = my_strdup(val);

    if (!(p->param) || !(p->value)) {
        free(p);
        return 0;
    }

    p->left = p->right = NULL;

    if (!*pp)
        *pp = p;
    else
        insert_pair(*pp, p);

    return 1;
}

/*
 *	ini_putf() takes a printf() style format string and uses vsnprintf() to
 *	pass a value to ini_put(). This function is intended for placing
 *	data types that are not strings into the ini_file
 *
 *	The other parameters are the same as those of ini_put().
 */
int ini_putf(struct ini_file* ini,
             const char*      sec,
             const char*      par,
             const char*      fmt,
             ...)
{
    char    buffer[MAX_LINE];
    va_list arg;

    va_start(arg, fmt);

#ifdef _MSC_VER /* Microsoft Visual C++? */
    /* VC++ messes around with _'s before vsnprintf(): */
#define vsnprintf _vsnprintf
#endif

#if 1
    vsnprintf(buffer, MAX_LINE, fmt, arg);
#else
    vsprintf(buffer, fmt, arg);
    assert(strlen(buffer) < MAX_LINE);
#endif
    va_end(arg);

    return ini_put(ini, sec, par, buffer);
}
