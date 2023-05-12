/*
 * A variety of utility functions.
 *
 * See utils.h for more info
 *
 * This is free and unencumbered software released into the public domain.
 * http://unlicense.org/
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "fatfs.h"

/* Case insensitive strcmp()
 */
int my_stricmp(const char* p, const char* q)
{
    for (; *p && tolower(*p) == tolower(*q); p++, q++)
        ;

    return tolower(*p) - tolower(*q);
}

/* strdup() is not ANSI C */
char* my_strdup(const char* s)
{
    char*  a;
    size_t len = strlen(s);
    a          = malloc(len + 1);

    if (!a)
        return NULL;

    memcpy(a, s, len + 1);
    return a;
}

/* converts a string to lowercase */
char* my_strlower(char* p)
{
    char* s;

    for (s = p; s[0]; s++)
        s[0] = tolower(s[0]);

    return p;
}

/* converts a string to lowercase */
char* my_strupper(char* p)
{
    char* s;

    for (s = p; s[0]; s++)
        s[0] = toupper(s[0]);

    return p;
}

char* my_strtok_r(char* str, const char* delim, char** saveptr)
{
    if (!str)
        str = *saveptr;

    if (!str[0]) {
        *saveptr = str;
        return NULL;
    }

    char* s = strpbrk(str, delim);

    if (s) {
        s[0]     = '\0';
        *saveptr = s + 1;
    } else
        for (*saveptr = str; (*saveptr)[0]; (*saveptr)++)
            ;

    return str;
}

/* Reads an entire file into a dynamically allocated memory buffer.
 * The returned buffer needs to be free()d afterwards
 */
char* my_readfile(const char* fname)
{
    FIL*     f = &USERFile;
    uint32_t len, r;
    char*    str;

    if (f_open(f, fname, FA_READ) != FR_OK) {
        return NULL;
    }

    len = f_size(f);

    str = malloc(len + 2);

    if (str == NULL) {
        return NULL;
    }

    f_read(f, str, len, &r);

    if (r != len) {
        free(str);
        return NULL;
    }

    f_close(f);
    str[len] = '\0';
    return str;

    // if (!(f = fopen(fname, "rb")))
    // 	return NULL;

    // fseek(f, 0, SEEK_END);
    // len = ftell(f);
    // rewind(f);

    // if (!(str = malloc(len + 2)))
    // 	return NULL;
    // r = fread(str, 1, len, f);

    // if (r != len)
    // {
    // 	free(str);
    // 	return NULL;
    // }

    // fclose(f);
    // str[len] = '\0';
    // return str;
}
