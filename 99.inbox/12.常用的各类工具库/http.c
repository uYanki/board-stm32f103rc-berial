#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include "http.h"
#include "xmalloc.h"

#ifndef MIN
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#endif
				      
static char * strdupdelim (const char *beg, const char *end);

/* Create a new, empty request.  At least request_set_method must be
   called before the request can be used.  */
/* 创建一个空的请求数据结构，注意：使用该请求之前，必须调用request_set_method()函数。*/
   
struct request * request_new (void)
{
  struct request *req = xnew0 (struct request);
  req->hcapacity = 8;
  req->headers = xnew_array (struct request_header, req->hcapacity);
  return req;
}

/* Set the request's method and its arguments.  METH should be a
   literal string (or it should outlive the request) because it will
   not be freed.  ARG will be freed by request_free.  */
/* 设置请求中的请求方法和请求方法的参数。
   注意：meth必须指向的是一个字符串常量或
   (it should outlive the request),因为它不会被free。*/

void request_set_method (struct request *req, const char *meth, char *arg)
{
  req->method = meth;
  req->arg = arg;
}

/* Return the method string passed with the last call to request_set_method.  */
/* 返回请求中的方法。*/
const char *request_method (const struct request *req)
{
  return req->method;
}

/* Free one header according to the release policy specified with
   request_set_header.  */
void release_header (struct request_header *hdr)
{
  switch (hdr->release_policy)
    {
    case rel_none:
      break;
    case rel_name:
      xfree (hdr->name);
      break;
    case rel_value:
      xfree (hdr->value);
      break;
    case rel_both:
      xfree (hdr->name);
      xfree (hdr->value);
      break;
    }
}

/* Set the request named NAME to VALUE.  Specifically, this means that
   a "NAME: VALUE\r\n" header line will be used in the request.  If a
   header with the same name previously existed in the request, its
   value will be replaced by this one.  A NULL value means do nothing.

   RELEASE_POLICY determines whether NAME and VALUE should be released
   (freed) with request_free.  Allowed values are:

    - rel_none     - don't free NAME or VALUE
    - rel_name     - free NAME when done
    - rel_value    - free VALUE when done
    - rel_both     - free both NAME and VALUE when done

   Setting release policy is useful when arguments come from different
   sources.  For example:

     // Don't free literal strings!
     request_set_header (req, "Pragma", "no-cache", rel_none);

     // Don't free a global variable, we'll need it later.
     request_set_header (req, "Referer", opt.referer, rel_none);

     // Value freshly allocated, free it when done.
     request_set_header (req, "Range",
                         aprintf ("bytes=%s-", number_to_static_string (hs->restval)),
			 rel_value);
*/
/*  设置请求头中的一个字段名为name的字段的值为value，
    注意：如果该字段已经存在，它的值将会被替换。
*/
void request_set_header (struct request *req, char *name, char *value, enum rp release_policy)
{
  struct request_header *hdr;
  int i;

  if (!value)
    {
      /* A NULL value is a no-op; if freeing the name is requested,
	 free it now to avoid leaks.  */
      if (release_policy == rel_name || release_policy == rel_both)
	xfree (name);
      return;
    }

  for (i = 0; i < req->hcount; i++)
    {
      hdr = &req->headers[i];
      if (0 == strcasecmp (name, hdr->name))
	{
	  /* Replace existing header. */
	  release_header (hdr);
	  hdr->name = name;
	  hdr->value = value;
	  hdr->release_policy = release_policy;
	  return;
	}
    }

  /* Install new header. */

  if (req->hcount >= req->hcapacity)
    {
      req->hcapacity <<= 1;
      req->headers = xrealloc (req->headers, req->hcapacity * sizeof (*hdr));
    }
  hdr = &req->headers[req->hcount++];
  hdr->name = name;
  hdr->value = value;
  hdr->release_policy = release_policy;
}


/* Copy the data delimited with BEG and END to alloca-allocated
   storage, and zero-terminate it.  Arguments are evaluated only once,
   in the order BEG, END, PLACE.  */
#define BOUNDED_TO_ALLOCA(beg, end, place) do {	\
    const char *BTA_beg = (beg);			\
    int BTA_len = (end) - BTA_beg;		\
    char **BTA_dest = &(place);			\
    *BTA_dest = alloca (BTA_len + 1);		\
    memcpy (*BTA_dest, BTA_beg, BTA_len);		\
    (*BTA_dest)[BTA_len] = '\0';			\
} while (0)

/* Like request_set_header, but sets the whole header line, as
   provided by the user using the `--header' option.  For example,
   request_set_user_header (req, "Foo: bar") works just like
   request_set_header (req, "Foo", "bar").  */

void request_set_user_header (struct request *req, const char *header)
{
  char *name;
  const char *p = strchr (header, ':');
  if (!p)
    return;
  BOUNDED_TO_ALLOCA (header, p, name);
  ++p;
  while (isspace (*p))
    ++p;
  request_set_header (req, xstrdup (name), (char *) p, rel_name);
}

/* Remove the header with specified name from REQ.  Returns 1 if the
   header was actually removed, 0 otherwise.  */

int request_remove_header (struct request *req, char *name)
{
	int i;
	for (i = 0; i < req->hcount; i++)
    {
		struct request_header *hdr = &req->headers[i];
		if (0 == strcasecmp (name, hdr->name))
		{
			release_header (hdr);
			/* Move the remaining headers by one. */
			if (i < req->hcount - 1)
				memmove (hdr, hdr + 1, (req->hcount - i - 1) * sizeof (*hdr));
			--req->hcount;
			return 1;
		}
    }
	return 0;
}

#define APPEND(p, str) do {			\
  int A_len = strlen (str);			\
  memcpy (p, str, A_len);			\
  p += A_len;					\
} while (0)

#define HTTP_0_9 1
#define HTTP_1_0 2
#define HTTP_1_1 3

/* Construct the request string.  */
char *request_string (const struct request *req,int http_v)
{
  char *request_string, *p;
  int i, size;

  /* Count the request size. */
  size = 0;

  /* METHOD " " ARG " " "HTTP/1.0" "\r\n" */
  size += strlen (req->method) + 1 + strlen (req->arg) + 1 + 8 + 2;

  for (i = 0; i < req->hcount; i++)
    {
      struct request_header *hdr = &req->headers[i];
      /* NAME ": " VALUE "\r\n" */
      size += strlen (hdr->name) + 2 + strlen (hdr->value) + 2;
    }

  /* "\r\n\0" */
  size += 3;

  p = request_string = xnew0_array(char, size);

  /* Generate the request. */

  APPEND (p, req->method); *p++ = ' ';
  APPEND (p, req->arg);    *p++ = ' ';
  if(http_v == HTTP_1_1)
	  memcpy (p, "HTTP/1.1\r\n", 10); 
  else
	  memcpy (p, "HTTP/1.0\r\n", 10);
  p += 10;

  for (i = 0; i < req->hcount; i++)
    {
      struct request_header *hdr = &req->headers[i];
      APPEND (p, hdr->name);
      *p++ = ':', *p++ = ' ';
      APPEND (p, hdr->value);
      *p++ = '\r', *p++ = '\n';
    }

  *p++ = '\r', *p++ = '\n', *p++ = '\0';
  assert (p - request_string == size);

#undef APPEND
 
  return request_string;
}

/* Release the resources used by REQ. */

void request_free (struct request *req)
{
  int i;
  xfree_null (req->arg);
  for (i = 0; i < req->hcount; i++)
    release_header (&req->headers[i]);
  xfree_null (req->headers);
  xfree (req);
}


/********************************************************************************/
/* Generally useful if you want to avoid arbitrary size limits but
   don't need a full dynamic array.  Assumes that BASEVAR points to a
   malloced array of TYPE objects (or possibly a NULL pointer, if
   SIZEVAR is 0), with the total size stored in SIZEVAR.  This macro
   will realloc BASEVAR as necessary so that it can hold at least
   NEEDED_SIZE objects.  The reallocing is done by doubling, which
   ensures constant amortized time per element.  */

#define DO_REALLOC(basevar, sizevar, needed_size, type)	do {		\
    long DR_needed_size = (needed_size);					\
    long DR_newsize = 0;							\
    while ((sizevar) < (DR_needed_size)) {				\
	DR_newsize = sizevar << 1;						\
	if (DR_newsize < 16)						\
	DR_newsize = 16;							\
	(sizevar) = DR_newsize;						\
    }									\
    if (DR_newsize)							\
    basevar = (type *)xrealloc (basevar, DR_newsize * sizeof (type));	\
} while (0)

/* Create a new response object from the text of the HTTP response,
   available in HEAD.  That text is automatically split into
   constituent header lines for fast retrieval using
   resp_header_*.  
*/
/* 参数head指向的是http响应头。
   如果head是有效的，该函数会根据http响应创建一个新的响应数据结构，
   并把http响应转换为响应头的多个记录(line)即设置headers。
   注意如果head指向的是'\0'则说明http响应是没有头部的http 0.9响应，这种情况下不会设置headers。*/

struct response *resp_new (const char *head)
{
  const char *hdr;
  int count, size;

  struct response *resp = xnew0 (struct response);
  resp->data = head;

  if (*head == '\0')
    {
      /* Empty head means that we're dealing with a headerless
	 (HTTP/0.9) response.  In that case, don't set HEADERS at
	 all.  */
      return resp;
    }

  /* Split HEAD into header lines, so that resp_header_* functions
     don't need to do this over and over again.  */

  size = count = 0;
  hdr = head;
  while (1)
    {
      DO_REALLOC (resp->headers, size, count + 1, const char *);
      resp->headers[count++] = hdr;

      /* Break upon encountering an empty line. */
      if (!hdr[0] || (hdr[0] == '\r' && hdr[1] == '\n') || hdr[0] == '\n')
	break;

      /* Find the end of HDR, including continuations. */
      do
	{
	  const char *end = strchr (hdr, '\n');
	  if (end)
	    hdr = end + 1;
	  else
	    hdr += strlen (hdr);
	}
      while (*hdr == ' ' || *hdr == '\t');
    }
  DO_REALLOC (resp->headers, size, count + 1, const char *);
  resp->headers[count] = NULL;

  return resp;
}

/* Locate the header named NAME in the response data, starting with
   position START.  This allows the code to loop through the response
   data, filtering for response of a given name.  Returns the
   found position, or -1 for failure.  The code that uses this
   function typically looks like this:

     for (pos = 0; (pos = resp_header_locate (...)) != -1; pos++)
       ... do something with header ...

   If you only care about one header, use resp_header_get instead of
   this function.  */

int resp_header_locate (const struct response *resp, const char *name, int start,
		    const char **begptr, const char **endptr)
{
  int i;
  const char **headers = resp->headers;
  int name_len;

  if (!headers || !headers[1])
    return -1;

  name_len = strlen (name);
  if (start > 0)
    i = start;
  else
    i = 1;

  for (; headers[i + 1]; i++)
    {
      const char *b = headers[i];
      const char *e = headers[i + 1];
      if (e - b > name_len
	  && b[name_len] == ':'
	  && 0 == strncasecmp (b, name, name_len))
	{
	  b += name_len + 1;
	  while (b < e && isspace (*b))
	    ++b;
	  while (b < e && isspace (e[-1]))
	    --e;
	  *begptr = b;
	  *endptr = e;
	  return i;
	}
    }
  return -1;
}

/* Find and retrieve the header named NAME in the request data.  If
   found, set *BEGPTR to its starting, and *ENDPTR to its ending
   position, and return 1.  Otherwise return 0.

   This function is used as a building block for resp_header_copy
   and resp_header_strdup.  */

int resp_header_get (const struct response *resp, const char *name,
		 const char **begptr, const char **endptr)
{
  int pos = resp_header_locate (resp, name, 0, begptr, endptr);
  return pos != -1;
}

/* Copy the response header named NAME to buffer BUF, no longer than
   BUFSIZE (BUFSIZE includes the terminating 0).  If the header
   exists, 1 is returned, otherwise 0.  If there should be no limit on
   the size of the header, use resp_header_strdup instead.

   If BUFSIZE is 0, no data is copied, but the boolean indication of
   whether the header is present is still returned.  */

int resp_header_copy (const struct response *resp, const char *name,
		  char *buf, int bufsize)
{
  const char *b, *e;
  if (!resp_header_get (resp, name, &b, &e))
    return 0;
  if (bufsize)
    {
      int len = MIN (e - b, bufsize - 1);
      memcpy (buf, b, len);
      buf[len] = '\0';
    }
  return 1;
}

/* Return the value of header named NAME in RESP, allocated with
   malloc.  If such a header does not exist in RESP, return NULL.  */

char * resp_header_strdup (const struct response *resp, const char *name)
{
  const char *b, *e;
  if (!resp_header_get (resp, name, &b, &e))
    return NULL;
  return strdupdelim (b, e);
}

/* Parse the HTTP status line, which is of format:

   HTTP-Version SP Status-Code SP Reason-Phrase

   The function returns the status-code, or -1 if the status line
   appears malformed.  The pointer to "reason-phrase" message is
   returned in *MESSAGE.  */

int resp_status (const struct response *resp, char **message)
{
  int status;
  const char *p, *end;

  if (!resp->headers)
    {
      /* For a HTTP/0.9 response, assume status 200. */
      if (message)
	*message = xstrdup ("No headers, assuming HTTP/0.9");
      return 200;
    }

  p = resp->headers[0];
  end = resp->headers[1];

  if (!end)
    return -1;

  /* "HTTP" */
  if (end - p < 4 || 0 != strncmp (p, "HTTP", 4))
    return -1;
  p += 4;

  /* Match the HTTP version.  This is optional because Gnutella
     servers have been reported to not specify HTTP version.  */
  if (p < end && *p == '/')
    {
      ++p;
      while (p < end && isdigit (*p))
	++p;
      if (p < end && *p == '.')
	++p; 
      while (p < end && isdigit (*p))
	++p;
    }

  while (p < end && isspace (*p))
    ++p;
  if (end - p < 3 || !isdigit (p[0]) || !isdigit (p[1]) || !isdigit (p[2]))
    return -1;

  status = 100 * (p[0] - '0') + 10 * (p[1] - '0') + (p[2] - '0');
  p += 3;

  if (message)
    {
      while (p < end && isspace (*p))
	++p;
      while (p < end && isspace (end[-1]))
	--end;
      *message = strdupdelim (p, end);
    }

  return status;
}

/* Release the resources used by RESP.  */

void resp_free (struct response *resp)
{
  xfree_null (resp->headers);
  xfree (resp);
}

/* Copy the string formed by two pointers (one on the beginning, other
   on the char after the last char) to a new, malloc-ed location.
   0-terminate it.  */
/* 类似于strdup,不同是指定了字符串的结束位置，而不是以\0作为结束符。*/
static char * strdupdelim (const char *beg, const char *end)
{
    char *res = (char *)xmalloc (end - beg + 1);
    memcpy (res, beg, end - beg);
    res[end - beg] = '\0';
    return res;
}
