/* Declarations for html-parse.c.
   Copyright (C) 1998 Free Software Foundation, Inc.
 */

#ifndef HTML_PARSE_H
#define HTML_PARSE_H

/* HTML中tag的属性 */
struct attr_pair {
  char *name;			/* attribute name,point to pool */
  char *value;			/* attribute value,point to pool */

  /* Needed for URL conversion; the places where the value begins and
     ends, including the quotes and everything. */
  const char *value_raw_beginning;
  int value_raw_size;

  /* Used internally by map_html_tags. */
  int name_pool_index, value_pool_index;
};

/* <tagname attrname="attrvalue" >....</tagname> */
/* ^                              ^              */
/* start_position                 end_postion    */

/* HTML中tag的信息*/
struct taginfo {
  char *name;			/* tag name,point to pool */
  int end_tag_p;		/* whether this is an end-tag */
  int nattrs;			/* number of attributes */
  struct attr_pair *attrs;	/* attributes */

  const char *start_position;	/* start position of tag */
  const char *end_position;	/* end position of tag */
};

struct hash_table;		/* forward declaration */

/* xinz+ */
#define START_TAG 0
#define CLOSE_TAG 1
#define COMMENT_TAG 2
#define DECLARE_TAG 2

/* Flags for map_html_tags: */
#define MHT_STRICT_COMMENTS  1  /* use strict comment interpretation */
#define MHT_TRIM_VALUES      2  /* trim attribute values, e.g. interpret <a href=" foo "> as "foo" */

#ifdef  __cplusplus
extern "C" {
#endif

void map_html_tags (const char *, int,
			    const char * (*) (struct taginfo *, void *), void *, int,
			    const struct hash_table *,
			    const struct hash_table *);
int decode_entity (const char **ptr, const char *end);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* HTML_PARSE_H */
