/* This file implements several wrappers around the basic allocation
   routines.  This is done for this reasons: so that the callers
   of these functions need not check for errors, which is easy to
   forget.  If there is not enough virtual memory for running programe,
   something is seriously wrong, and programe exits with an appropriate
   error message.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Croak the fatal memory error and bail out with non-zero exit  status.  */
static void memfatal(const char *context, size_t attempted_size)
{
  fprintf(stderr, "%s: failed to allocate %d bytes,memory exhausted.", context, attempted_size);
  exit(1);
}

void *xmalloc(size_t size)
{
  void *ptr = malloc(size);
  if (!ptr)
    memfatal("malloc", size);
  return ptr;
}

void *xmalloc0(size_t size)
{
  /* Using calloc can be faster than malloc+memset because some calloc
     implementations know when they're dealing with zeroed-out memory
     from the system and can avoid unnecessary memset.  */
  void *ptr = calloc(1, size);
  if (!ptr)
    memfatal("calloc", size);
  return ptr;
}

void *xcalloc(size_t num, size_t size)
{
  void *ptr = calloc(num, size);
  if (!ptr)
    memfatal("calloc", num*size);
  return ptr;
}

void * xrealloc(void *ptr, size_t newsize)
{
  void *newptr;

  /* Not all Unixes have the feature of realloc() that calling it with
     a NULL-pointer is the same as malloc(), but it is easy to
     simulate.  */
  if (ptr)
    newptr = realloc(ptr, newsize);
  else
    newptr = malloc(newsize);

  if (!newptr)
    memfatal("realloc", newsize);

  return newptr;
}

char * xstrdup(const char *s)
{
  char *copy;

  copy = strdup(s);
  if (!copy)
    memfatal("strdup", 1 + strlen(s));

  return copy;
}

#define TOLOWER(x) ('A' <= (x) && (x) <= 'Z' ? (x) + 32 : (x))

/* Like xstrdup(), but also lowercases S.  */
char *xstrduplower(const char *s)
{
  char *copy = xstrdup(s);
  char *p = copy;
  for (; *p; p++)
    *p = TOLOWER(*p);
  return copy;
}


/* Copy the string formed by two pointers (one on the beginning, other
   on the char after the last char) to a new, malloc-ed location.
   0-terminate it.  */
char * xstrdupdelim(const char *beg, const char *end)
{
  char *res = (char *)xmalloc(end - beg + 1);
  memcpy(res, beg, end - beg);
  res[end - beg] = '\0';
  return res;
}


void xfree(void *ptr)
{
  /* xfree() must not be passed a NULL pointer.  */
  if (ptr)
  {
    free(ptr);
  }
}

void xfreenull(void **ptr)
{
  if(!ptr)
    return;
  
  if(*ptr)
  {
    free(*ptr);
    *ptr = NULL;
  }
}

