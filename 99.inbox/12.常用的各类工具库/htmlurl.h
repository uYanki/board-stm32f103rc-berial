#ifndef HTMLURL_H
#define HTMLURL_H

enum convert_options {
  CO_NOCONVERT = 0,		/* don't convert this URL */
  CO_CONVERT_TO_RELATIVE,	/* convert to relative, e.g. to
                                   "../../otherdir/foo.gif" */
  CO_CONVERT_TO_COMPLETE,	/* convert to absolute, e.g. to
				   "http://orighost/somedir/bar.jpg". */
  CO_NULLIFY_BASE		/* change to empty string. */
};

struct url;

/* A structure that defines the whereabouts of a URL, i.e. its
   position in an HTML document, etc.  */

struct urlpos {
  struct url *url;		/* the URL of the link, after it has been merged with the base */
  char *local_name;		/* local file to which it was saved (used by convert_links) */

  /* reserved for special links such as <base href="..."> which are
     used when converting links, but ignored when downloading.  */
  uint32_t ignore_when_downloading	:1;

  /* Information about the original link: */

  uint32_t link_relative_p	:1; /* the link was relative */
  uint32_t link_complete_p	:1; /* the link was complete (had host name) */
  uint32_t link_base_p	:1; /* the url came from <base href=...> */
  uint32_t link_inline_p	:1; /* needed to render the page */
  uint32_t link_expect_html	:1; /* expected to contain HTML */

  uint32_t link_refresh_p	:1; /* link was received from
				       <meta http-equiv=refresh content=...> */
  int refresh_timeout;		/* for reconstructing the refresh. */

  /* Conversion requirements: */
  enum convert_options convert;	/* is conversion required? */

  /* URL's position in the buffer. */
  int pos, size;

  struct urlpos *next;		/* next list element */
};

struct file_memory {
  char *content;
  long length;
  int mmap_p;
};

#ifdef  __cplusplus
extern "C" {
#endif

  struct urlpos * get_urls_html (const char *file, const char *url, int *meta_disallow_follow);
  void cleanup_html_url (void);

#ifdef  __cplusplus
}
#endif 

#endif

