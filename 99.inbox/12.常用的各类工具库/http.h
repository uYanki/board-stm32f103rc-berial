#ifndef HTTP_H
#define HTTP_H

enum rp {  //release policy 释放方式 ,请求头中字段的释放方式
  rel_none,
  rel_name,
  rel_value,
  rel_both
};

/* request 数据结构 */
struct request
{
  const char *method;       /* 请求方法，例如:get post  */
  char *arg;                /* 请求方法的参数  */

  struct request_header     /* 请求头中的一个字段的数据结构  */
  {   
    char *name;             /* 字段名  */
    char *value;            /* 字段值  */
    enum rp release_policy; /* 字段的释放方式  */
  }*headers;

  int hcount;               /* header count 请求头中字段的个数  */
  int hcapacity;            /* header capacity 用于存储请求头中字段的空间的个数，初始是8个。 */
};

struct response
{
  const char **headers;     /* 指向指针数组的指针 */
  const char *data;         /* The response data. */
  
  /* The array of header pointers that indicate where each header starts.
     For example, given this HTTP response:

     HTTP/1.0 200 Ok
     Description: some 
     text
     Etag: x

     The headers are located like this:
  "HTTP/1.0 200 Ok\r\nDescription: some\r\n text\r\nEtag: x\r\n\r\n"
  ^                   ^                             ^          ^
  headers[0]          headers[1]                    headers[2] headers[3]

  I.e. headers[0] points to the beginning of the response,
  headers[1] points to the end of the first header and the
  beginning of the second one, etc.  */
};

/* Some status code validation macros: */
#define H_20X(x)        (((x) >= 200) && ((x) < 300))
#define H_PARTIAL(x)    ((x) == HTTP_STATUS_PARTIAL_CONTENTS)
#define H_REDIRECTED(x) ((x) == HTTP_STATUS_MOVED_PERMANENTLY  \
                         || (x) == HTTP_STATUS_MOVED_TEMPORARILY \
                         || (x) == HTTP_STATUS_SEE_OTHER  \
                         || (x) == HTTP_STATUS_TEMPORARY_REDIRECT)

/* HTTP/1.0 status codes from RFC1945, provided for reference.  */
/* Successful 2xx.  */
#define HTTP_STATUS_OK                200
#define HTTP_STATUS_CREATED           201
#define HTTP_STATUS_ACCEPTED          202
#define HTTP_STATUS_NO_CONTENT        204
#define HTTP_STATUS_PARTIAL_CONTENTS  206

/* Redirection 3xx.  */
#define HTTP_STATUS_MULTIPLE_CHOICES  300
#define HTTP_STATUS_MOVED_PERMANENTLY 301
#define HTTP_STATUS_MOVED_TEMPORARILY 302
#define HTTP_STATUS_SEE_OTHER         303 /* from HTTP/1.1 */
#define HTTP_STATUS_NOT_MODIFIED      304
#define HTTP_STATUS_TEMPORARY_REDIRECT  307 /* from HTTP/1.1 */

/* Client error 4xx.  */
#define HTTP_STATUS_BAD_REQUEST   400
#define HTTP_STATUS_UNAUTHORIZED  401
#define HTTP_STATUS_FORBIDDEN     403
#define HTTP_STATUS_NOT_FOUND     404
#define HTTP_STATUS_RANGE_NOT_SATISFIABLE 416

/* Server errors 5xx.  */
#define HTTP_STATUS_INTERNAL        500
#define HTTP_STATUS_NOT_IMPLEMENTED 501
#define HTTP_STATUS_BAD_GATEWAY     502
#define HTTP_STATUS_UNAVAILABLE     503

#ifdef __cplusplus
extern "C"
{
#endif

struct request * request_new(void);
void request_set_method(struct request *req, const char *meth, char *arg);
const char * request_method(const struct request *req);
void release_header(struct request_header *hdr);
void request_set_header(struct request *req, char *name, char *value,enum rp release_policy);
void request_set_user_header(struct request *req, const char *header);
int request_remove_header(struct request *req, char *name);
char *request_string(const struct request *req, int http_v);
void request_free(struct request *req);

struct response *resp_new(const char *head);
int resp_header_locate(const struct response *resp, const char *name, int start,const char **begptr, const char **endptr);
int resp_header_get(const struct response *resp, const char *name,const char **begptr, const char **endptr);
int resp_header_copy(const struct response *resp, const char *name,char *buf, int bufsize);
char *resp_header_strdup(const struct response *resp, const char *name);
int resp_status(const struct response *resp, char **message);
void resp_free(struct response *resp);

#ifdef __cplusplus
}
#endif

#endif

