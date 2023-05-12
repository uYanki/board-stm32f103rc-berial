#ifndef HTML_EXTRACT_H
#define HTML_EXTRACT_H

#include <iconv.h>
#include "comn.h"

#define MAX_TITLE_LEN         512
#define MAX_TEXT_LEN          (300 * 1024)
#define MAX_ANCHOR_TEXT_LEN   (300 * 1024)
#define MAX_META_KEYWORD_LEN  1024
#define MAX_META_DESC_LEN     2048
#define MAX_URL_BUFFER        (128 * 20)

#define  SKIP_TAG             0
#define  RESERVE_TAG          1
#define  EXTRACT_TAG          2

#define IMG_OP_NONE          -1
#define IMG_OP_FILTER         0
#define IMG_OP_RESERVE        1
#define IMG_OP_REPLACE        2

#define  NO_COUNT            -1

/* content type */
#define HTML_CONTENT          0
#define IMAGE_CONTENT         1
#define OTHER_CONTENT         2

// used for extract list urls
#define FD_LISTURL_NONE       0  // 分布式抓取则不需要消重
#define FD_LISTURL_CRC32      1  // 占用内存少，不同的url可能会产生相同的crc
#define FD_LISTURL_MD5        2  // 占用内存多


/* forward declaration */
struct hash_table;
struct taginfo;
struct _spec_tag;

typedef struct
{
  buffer_t *text_buffer_p;
  const char *html_content_end_position;
  const char *last_tag_end_position;
  const struct hash_table *spec_tag_ht;
  const struct hash_table *tag_attr_ht;
  int is_reserve_indent;
  int codetype;
  int xssfilter;
}map_arg_t;

typedef const char *(*tag_handler_t)(struct taginfo *, map_arg_t*, struct _spec_tag *);

typedef struct _spec_tag
{
  char * tag_name; /* eg. title */
  char * end_str;  /* eg.</title */
  int type;
  tag_handler_t handler;
  void *handler_arg;
}spec_tag_t;

typedef struct _tag_attr
{
	char *tag_attr; /* href */
	char *tag_name; /* a | base */
  struct _tag_attr *next; 
  struct _tag_attr *tail;
}tag_attr_t;


struct _meta_text_buffer
{
  buffer_t *meta_keyword_buffer_p;
  buffer_t *meta_desc_buffer_p;
};

struct _img_arg
{
  char *base;//<base href="">
  char *referer;
  int op;
  buffer_t *url_buffer_p;
  char *image_option_dir;
  char *image_base_url;
  char *url_prefix;
  int image_subdir_num;
  int *image_subdirname_width;
  int pageisutf8;
  iconv_t cd;
};

#ifdef  __cplusplus
extern "C"
{
#endif

  void extract_text_init(spec_tag_t *spec_tag_list, int tag_list_size, struct hash_table ** spec_tag_ht_pp,
                         tag_attr_t *tag_attr_list, int attr_list_size, struct hash_table ** tag_attr_ht_pp);
  void extract_text(const char *html_content, int content_len, buffer_t *text_buffer_p,
                    const struct hash_table *spec_tag_ht, const struct hash_table *tag_attr_ht, int is_reserve_indent,int codetype,int xssfilter);
  void extract_text_free(struct hash_table *spec_tag_ht, struct hash_table * tag_attr_ht);

  const char *tag_handler_title(struct taginfo *taginfo, map_arg_t* maparg, spec_tag_t *spec_tag);
  const char *tag_handler_anchor(struct taginfo *taginfo, map_arg_t* maparg, spec_tag_t *spec_tag);
  const char *tag_handler_meta(struct taginfo *taginfo, map_arg_t* maparg, spec_tag_t *spec_tag);
  const char *tag_handler_img(struct taginfo *taginfo, map_arg_t* maparg, spec_tag_t *spec_tag);

  int extract_interesting_tag_attr(char *html_content, int content_len,
                                   char *interesting_tag, char *interesting_tag_attr,
                                   buffer_t *interesting_tag_attr_buf, int count);
  int get_content_type(const char *header);
  char * get_last_modified(const char *header, char *last_modified, int size);
  char * get_base_url(char *html_content, int content_len, buffer_t *base_url_buffer_p, int buffer_size);
  int get_next_page_url(char *html_content, int content_len, buffer_t *next_url_buffer_p, char * next_text, char *prev_text);

  void *extract_listurl_init(void *cd,int fd,int hashsize);
  const char *extract_listurl(void *maparg, const char *html_content, int content_len, const char *url, int pageisutf8);
  void extract_listurl_free(void *maparg);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */
#endif
