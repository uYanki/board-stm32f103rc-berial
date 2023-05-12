#ifndef XMALLOC_H
#define XMALLOC_H

#ifdef __cplusplus
extern "C"
{
#endif

  void *xmalloc(size_t size);
  void *xmalloc0(size_t size);
  void *xcalloc(size_t num, size_t size);
  void *xrealloc(void *ptr, size_t newsize);
  char *xstrdup(const char *s);
  char *xstrduplower(const char *s);
  char *xstrdupdelim(const char *beg, const char *end);
  void xfree(void *ptr);
  void xfreenull(void **ptr);
  
#ifdef __cplusplus
}
#endif

#define xnew(type) ((type *) xmalloc (sizeof (type)))
#define xnew0(type) ((type *) xmalloc0 (sizeof (type)))
#define xnew_array(type, len) ((type *) xmalloc ((len) * sizeof (type)))
#define xnew0_array(type, len) ((type *) xmalloc0 ((len) * sizeof (type)))
#define alloca_array(type, size) ((type *) alloca ((size) * sizeof (type)))
#define xfree_null(p) if (!(p)) ; else xfree (p)

#endif

