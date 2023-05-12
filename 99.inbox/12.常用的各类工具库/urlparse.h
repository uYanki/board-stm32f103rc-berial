#ifndef URLPARSE_H
#define URLPARSE_H

/* Default port definitions */
#define DEFAULT_HTTP_PORT  80
#define DEFAULT_FTP_PORT   21
#define DEFAULT_HTTPS_PORT 443

/* Note: the ordering here is related to the order of elements in
   `supported_schemes' in url.c.  */

enum url_scheme {
  SCHEME_HTTP,
  SCHEME_HTTPS,
  SCHEME_FTP,
  SCHEME_INVALID
};

/*  URL:=
          [scheme://][username:password@]
          host[:port][/path]
          [;params][?query][#fragment]
          path:=dir/file
          full_path:=/path?query
          query:=name1=value1&name2=value2&... */

/* Structure containing info on a URL.  */
struct url
{
  char *url;              /* Original URL, 会在原始URL基础上对其中存在的不安全字符(包括汉字)编码 */
  enum url_scheme scheme; /* URL scheme */

  char *host;             /* Extracted hostname, 会统一为小写字母并解码 */
  int port;               /* Port number */

  /* URL components (URL-quoted 转义表示). */
  char *path;             /* 不包括根目录/,没有解码*/
  char *params;
  char *query;            /* 没有解码 */
  char *fragment;

  /* Extracted path info (unquoted 不转义表示). */
  char *dir;              /* 已解码 */
  char *file;             /* 已解码 */

  /* Username and password (unquoted 不转义表示). */
  char *user;             /* 已解码 */
  char *passwd;           /* 已解码 */
};

/* URL Parse eg.
  http://www.gekang.com  => path="" dir="" file=""
  http://www.gekang.com/ => path="" dir="" file=""
  http://www.gekang.com/dir1/dir2/ => path=dir1/dir2/ dir=dir1/dir2 file=""
  http://www.gekang.com/dir1/file1 => path=dir1/file1 dir=dir1 file=file1
*/

#ifdef __cplusplus
extern "C"
{
#endif

  void url_unescape (char *s); // 对%hh编码进行解码，不会对加号转为空格
  char *url_escape(const char *); // 仅对URL中不安全字符(包括汉字)编码
  char *reencode_escapes (const char *s); // 注意判断其返回值是否等于s，不等于s才需要free。

  struct url *url_parse(const char *, int *);
  const char *url_error(int);
  char *url_full_path(const struct url *);
  void url_set_dir(struct url *, const char *);
  void url_set_file(struct url *, const char *);
  char *url_string(const struct url *, int);
  void url_free(struct url *);

  enum url_scheme url_scheme(const char *);
  int url_has_scheme(const char *);
  int scheme_default_port(enum url_scheme);
  void scheme_disable(enum url_scheme);
  
  char *uri_merge(const char *, const char *);
  char *rewrite_shorthand_url(const char *);

#ifdef __cplusplus
}
#endif

#endif
