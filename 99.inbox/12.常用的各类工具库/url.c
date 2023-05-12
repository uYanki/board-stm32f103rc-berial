/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iconv.h>
#include "xmalloc.h"
#include "xstring.h"
#include "utils.h"
#include "urlparse.h"
#include "comn.h"

typedef struct qryvar {
    char*          name;
    char*          value;
    struct qryvar* next;
} qryvar_t;

typedef struct qrylist {
    qryvar_t* head;
    qryvar_t* tail;
    int       varnum;
    qryvar_t* curvar;  // 用于遍历
} qrylist_t;

void* url_qryparse_init(void)
{
    qrylist_t* ql;

    ql = (qrylist_t*)xcalloc(1, sizeof(qrylist_t));

    return ql;
}

static void qrylist_addvar(qrylist_t* list, qryvar_t* var)
{
    if (list->tail) {
        list->tail->next = var;
        var->next        = NULL;
        list->tail       = var;
    } else {
        list->tail = var;
        list->head = var;
        var->next  = NULL;
    }

    list->varnum++;
}

int url_qryparse(void* qrylist, const char* querystring, int unescape)
{
    qrylist_t* list = (qrylist_t*)qrylist;
    qryvar_t*  var;
    char *     s, *q, *np, *vp;

    if (!querystring || !querystring[0])
        return 0;

    q  = xstrdup(querystring);
    s  = q;
    np = s;
    while (*np) {
        /* find name/value pairs */
        for (; *s && *s != '&'; s++)
            ;

        /* chop them up */
        if (*s == '&') {
            *s = '\0';
            s++;
        }

        if ((vp = strchr(np, '=')) != NULL) {
            *vp = '\0';
            vp++;
            if (unescape) {
                urldecode(np);
                urldecode(vp);
            }
            trim(np);
            trim(vp);
            if (np[0] && vp[0]) {
                var        = (qryvar_t*)xcalloc(1, sizeof(qryvar_t));
                var->name  = xstrdup(np);
                var->value = xstrdup(vp);
                qrylist_addvar(list, var);
            }
        }

        np = s;
    }

    xfree(q);
    return 0;
}

// delim = '=' sep = '&'
int url_qryparse2(void* qrylist, const char* querystring, int unescape, const char delim, const char sep)
{
    qrylist_t* list = (qrylist_t*)qrylist;
    qryvar_t*  var;
    char *     s, *q, *np, *vp;

    if (!querystring || !querystring[0])
        return 0;

    q  = xstrdup(querystring);
    s  = q;
    np = s;
    while (*np) {
        /* find name/value pairs */
        for (; *s && *s != sep; s++)
            ;

        /* chop them up */
        if (*s == sep) {
            *s = '\0';
            s++;
        }

        if ((vp = strchr(np, delim)) != NULL) {
            *vp = '\0';
            vp++;
            if (unescape) {
                urldecode(np);
                urldecode(vp);
            }
            trim(np);
            trim(vp);
            if (np[0] && vp[0]) {
                var        = (qryvar_t*)xcalloc(1, sizeof(qryvar_t));
                var->name  = xstrdup(np);
                var->value = xstrdup(vp);
                qrylist_addvar(list, var);
            }
        }

        np = s;
    }

    xfree(q);
    return 0;
}

const char* url_qryparse_get(void* qrylist, const char* name)
{
    qrylist_t* list = (qrylist_t*)qrylist;
    qryvar_t*  var;

    if (!name || !name[0])
        return NULL;

    var = list->head;
    while (var) {
        if (var->name && strcasecmp(name, var->name) == 0)
            return var->value;
        var = var->next;
    }

    return NULL;
}

int url_qryparse_getvarnum(void* qrylist)
{
    qrylist_t* list = (qrylist_t*)qrylist;
    return list->varnum;
}

void url_qryparse_iterator_init(void* qrylist)
{
    qrylist_t* list = (qrylist_t*)qrylist;

    list->curvar    = NULL;
}

const char* url_qryparse_iterator_next(void* qrylist, char** varname)
{
    qrylist_t* list = (qrylist_t*)qrylist;
    qryvar_t*  nextvar;

    if (!list->curvar)
        nextvar = list->head;
    else
        nextvar = list->curvar->next;

    if (!nextvar)
        return NULL;

    if (varname)
        *varname = nextvar->name;
    list->curvar = nextvar;
    return nextvar->value;
}

void url_qryparse_free(void* qrylist)
{
    qrylist_t* list = (qrylist_t*)qrylist;
    qryvar_t * var, *v;

    var = list->head;
    while (var) {
        xfree(var->name);
        xfree(var->value);
        v   = var;
        var = var->next;
        xfree(v);
    }

    xfree(list);
}

// fullpath = /path[?query]
int url_isdynamic(const char* fullpath)
{
    const char* p;
    p = fullpath;
    while (*p) {
        if (*p == '?') {
            return 1;
        }
        p++;
    }
    return 0;
}

/* Does this url starts with a protocol name */
int url_startwithprotocol(char* s)
{
    uint32_t i = 0;
    while (isalnum(s[i])) {
        i++;
    }
    return s[i] == ':';
}

/* normalize a file name
 * return true if it is ok, false otherwise (cgi-bin)
 */
int url_filenormalize(char* file)
{
    int i = 0;
    while (file[i] != 0 && file[i] != '#') {
        if (file[i] == '/') {
            if (file[i + 1] == '.' && file[i + 2] == '/') {
                // suppress /./
                int j = i + 3;
                while (file[j] != 0) {
                    file[j - 2] = file[j];
                    j++;
                }
                file[j - 2] = 0;
            } else if (file[i + 1] == '/') {
                // replace // by /
                int j = i + 2;
                while (file[j] != 0) {
                    file[j - 1] = file[j];
                    j++;
                }
                file[j - 1] = 0;
            } else if (file[i + 1] == '.' && file[i + 2] == '.' && file[i + 3] == '/') {
                // suppress /../
                if (i == 0) {
                    // the file name starts with /../ : error
                    // eg. /./../../../uploadfile/UpPicmore/2.jpg
                    int j = i + 4;
                    while (file[j] != 0) {
                        file[j - 3] = file[j];
                        j++;
                    }
                    file[j - 3] = 0;
                    // return 0;
                } else {
                    int j = i + 4, dec;
                    i--;
                    while (file[i] != '/') {
                        i--;
                    }
                    dec = i + 1 - j;  // dec < 0
                    while (file[j] != 0) {
                        file[j + dec] = file[j];
                        j++;
                    }
                    file[j + dec] = 0;
                }
            } else if (file[i + 1] == '.' && file[i + 2] == 0) {
                // suppress /.
                file[i + 1] = 0;
                return 1;
            } else if (file[i + 1] == '.' && file[i + 2] == '.' && file[i + 3] == 0) {
                // suppress /..
                if (i == 0) {
                    // the file name starts with /.. : error
                    return 0;
                } else {
                    i--;
                    while (file[i] != '/') {
                        i--;
                    }
                    file[i + 1] = 0;
                    return 1;
                }
            } else {  // nothing special, go forward
                i++;
            }
        }
        /* xinz- comment from line 145 to line 171
        else if (file[i] == '%')
        {
        int v1 = int_of_hexa(file[i+1]);
        int v2 = int_of_hexa(file[i+2]);
        if (v1 < 0 || v2 < 0) return false;
        char c = 16 * v1 + v2;
        if (isgraph(c))
        {
        file[i] = c;
        int j = i+3;
        while (file[j] != 0)
        {
        file[j-2] = file[j];
        j++;
        }
        file[j-2] = 0;
        i++;
        }
        else if (c == ' ' || c == '/')
        { // keep it with the % notation
        i += 3;
        }
        else
        { // bad url
        return false;
        }
        } */
        else {  // nothing special, go forward
            i++;
        }
    }
    file[i] = 0;
    return 1;
}

/*
  /path=/dir/file
  eg:
  /          dir_depth=0
  /a/        dir_depth=1
  /a/b/      dir_depth=2
  /a/b/c     dir_depth=2
  /a/b/c/    dir_depth=3
*/
int url_dirdepth(const char* path)
{
    const char* p   = path;
    int         dep = 0;
    if (!p) return -1;
    if (*p == '/') ++p;
    while (*p) {
        if (*p == '/') dep++;
        p++;
    }
    return dep;
}

int url_validipv4address(const char* str, const char* end)
{
    int saw_digit = 0;
    int octets    = 0;
    int val       = 0;

    while (str < end) {
        int ch = *str++;

        if (ch >= '0' && ch <= '9') {
            val = val * 10 + (ch - '0');

            if (val > 255)
                return 0;
            if (saw_digit == 0) {
                if (++octets > 4)
                    return 0;
                saw_digit = 1;
            }
        } else if (ch == '.' && saw_digit == 1) {
            if (octets == 4)
                return 0;
            val       = 0;
            saw_digit = 0;
        } else
            return 0;
    }
    if (octets < 4)
        return 0;

    return 1;
}

//////////////////////////////////////////////

#define ISALNUM(x)      isalnum(x)
#define SCHEME_CHAR(ch) (ISALNUM(ch) || (ch) == '-' || (ch) == '+')

/* Return 1 if the URL begins with any "scheme", 0 otherwise.  As
   currently implemented, it returns true if URL begins with
   [-+a-zA-Z0-9]+: .  */

static int url_hasscheme(const char* url)
{
    const char* p = url;

    /* The first char must be a scheme char. */
    if (!*p || !SCHEME_CHAR(*p))
        return 0;
    ++p;
    /* Followed by 0 or more scheme chars. */
    while (*p && SCHEME_CHAR(*p))
        ++p;
    /* Terminated by ':'. */
    return *p == ':';
}

/* 类似于strpbrk，不同的是当在s中没有找到accept中的字符时，返回指向s的'\0'的指针。*/
static char* strpbrk_or_eos(const char* s, const char* accept)
{
    char* p = strpbrk(s, accept);
    if (!p)
        p = strchr(s, '\0');
    return p;
}

/* Return the length of URL's path.  Path is considered to be
   terminated by one of '?', ';', '#', or by the end of the
   string.  */

static int path_length(const char* url)
{
    const char* q = strpbrk_or_eos(url, "?;#");
    return q - url;
}

/* Find the last occurrence of character C in the range [b, e), or
   NULL, if none are present.  We might want to use memrchr (a GNU
   extension) under GNU libc.  */

static const char* find_last_char(const char* b, const char* e, char c)
{
    for (; e > b; e--)
        if (*e == c)
            return e;
    return NULL;
}

/* Merge BASE with LINK and return the resulting URI.

   Either of the URIs may be absolute or relative, complete with the
   host name, or path only.  This tries to reasonably handle all
   foreseeable cases.  It only employs minimal URL parsing, without
   knowledge of the specifics of schemes.

   I briefly considered making this function call path_simplify after
   the merging process, as rfc1738 seems to suggest.  This is a bad
   idea for several reasons: 1) it complexifies the code, and 2)
   url_parse has to simplify path anyway, so it's wasteful to boot.
   注意: 返回的是动态内存，需要释放。base值可以base tag值也可以是refer url
   */
static char* url_merge(const char* base, const char* link)
{
    int         linklength;
    const char* end;
    char*       merge;

    if (url_hasscheme(link))
        return xstrdup(link);

    /* We may not examine BASE past END. */
    end        = base + path_length(base);
    linklength = strlen(link);

    if (!*link) {
        /* Empty LINK points back to BASE, query string and all. */
        return xstrdup(base);
    } else if (*link == '?') {
        /* LINK points to the same location, but changes the query
           string.  Examples: */
        /* uri_merge("path",         "?new") -> "path?new"     */
        /* uri_merge("path?foo",     "?new") -> "path?new"     */
        /* uri_merge("path?foo#bar", "?new") -> "path?new"     */
        /* uri_merge("path#foo",     "?new") -> "path?new"     */
        int baselength = end - base;
        merge          = xmalloc(baselength + linklength + 1);
        memcpy(merge, base, baselength);
        memcpy(merge + baselength, link, linklength);
        merge[baselength + linklength] = '\0';
    } else if (*link == '#') {
        /* uri_merge("path",         "#new") -> "path#new"     */
        /* uri_merge("path#foo",     "#new") -> "path#new"     */
        /* uri_merge("path?foo",     "#new") -> "path?foo#new" */
        /* uri_merge("path?foo#bar", "#new") -> "path?foo#new" */
        int         baselength;
        const char* end1 = strchr(base, '#');
        if (!end1)
            end1 = base + strlen(base);
        baselength = end1 - base;
        merge      = xmalloc(baselength + linklength + 1);
        memcpy(merge, base, baselength);
        memcpy(merge + baselength, link, linklength);
        merge[baselength + linklength] = '\0';
    } else if (*link == '/' && *(link + 1) == '/') {
        /* LINK begins with "//" and so is a net path: we need to
           replace everything after (and including) the double slash
           with LINK. */

        /* uri_merge("foo", "//new/bar")            -> "//new/bar"      */
        /* uri_merge("//old/foo", "//new/bar")      -> "//new/bar"      */
        /* uri_merge("http://old/foo", "//new/bar") -> "http://new/bar" */

        int         span;
        const char* slash;
        const char* start_insert;

        /* Look for first slash. */
        slash = memchr(base, '/', end - base);
        /* If found slash and it is a double slash, then replace
           from this point, else default to replacing from the
           beginning.  */
        if (slash && *(slash + 1) == '/')
            start_insert = slash;
        else
            start_insert = base;

        span  = start_insert - base;
        merge = (char*)xmalloc(span + linklength + 1);
        if (span)
            memcpy(merge, base, span);
        memcpy(merge + span, link, linklength);
        merge[span + linklength] = '\0';
    } else if (*link == '/') {
        /* LINK is an absolute path: we need to replace everything
           after (and including) the FIRST slash with LINK.

           So, if BASE is "http://host/whatever/foo/bar", and LINK is
           "/qux/xyzzy", our result should be
           "http://host/qux/xyzzy".  */
        int         span;
        const char* slash;
        const char* start_insert     = NULL; /* for gcc to shut up. */
        const char* pos              = base;
        int         seen_slash_slash = 0;
        /* We're looking for the first slash, but want to ignore
           double slash. */
    again:
        slash = memchr(pos, '/', end - pos);
        if (slash && !seen_slash_slash)
            if (*(slash + 1) == '/') {
                pos              = slash + 2;
                seen_slash_slash = 1;
                goto again;
            }

        /* At this point, SLASH is the location of the first / after
           "//", or the first slash altogether.  START_INSERT is the
           pointer to the location where LINK will be inserted.  When
           examining the last two examples, keep in mind that LINK
           begins with '/'. */

        if (!slash && !seen_slash_slash)
            /* example: "foo" */
            /*           ^    */
            start_insert = base;
        else if (!slash && seen_slash_slash)
            /* example: "http://foo" */
            /*                     ^ */
            start_insert = end;
        else if (slash && !seen_slash_slash)
            /* example: "foo/bar" */
            /*           ^        */
            start_insert = base;
        else if (slash && seen_slash_slash)
            /* example: "http://something/" */
            /*                           ^  */
            start_insert = slash;

        span  = start_insert - base;
        merge = (char*)xmalloc(span + linklength + 1);
        if (span)
            memcpy(merge, base, span);
        memcpy(merge + span, link, linklength);
        merge[span + linklength] = '\0';
    } else {
        /* LINK is a relative URL: we need to replace everything
           after last slash (possibly empty) with LINK.

           So, if BASE is "whatever/foo/bar", and LINK is "qux/xyzzy",
           our result should be "whatever/foo/qux/xyzzy".  */
        int         need_explicit_slash = 0;
        int         span;
        const char* start_insert;
        const char* last_slash = find_last_char(base, end, '/');
        if (!last_slash) {
            /* No slash found at all.  Replace what we have with LINK. */
            start_insert = base;
        } else if (last_slash && last_slash >= base + 2 && last_slash[-2] == ':' && last_slash[-1] == '/') {
            /* example: http://host"  */
            /*                      ^ */
            start_insert        = end + 1;
            need_explicit_slash = 1;
        } else {
            /* example: "whatever/foo/bar" */
            /*                        ^    */
            start_insert = last_slash + 1;
        }

        span  = start_insert - base;
        merge = (char*)xmalloc(span + linklength + 1);
        if (span)
            memcpy(merge, base, span);
        if (need_explicit_slash)
            merge[span - 1] = '/';
        memcpy(merge + span, link, linklength);
        merge[span + linklength] = '\0';
    }

    return merge;
}

// pageisutf8=1并且URL包含GBK汉字: GBK->UTF-8 返回时结果需要free。
char* url_transcode(char* url, int pageisutf8, iconv_t cd)
{
    char*  p;
    size_t obsize, inlen, outlen;
    char * ob, *inbuf, *outbuf;

    if (!url || !*url)
        return NULL;

    if (pageisutf8 != 1)
        return NULL;

    p = url;
    while (*p && !ISGBK(*p)) ++p;
    if (!*p) return NULL;

    inlen  = strlen(url);
    inbuf  = url;
    obsize = inlen * 5;
    ob     = (char*)xcalloc(obsize, sizeof(char));
    outbuf = ob;
    outlen = obsize;
    if (iconv(cd, &inbuf, &inlen, &outbuf, &outlen) != 0) {
        xfree(ob);
        return NULL;
    }
    iconv(cd, NULL, NULL, NULL, NULL);
    return ob;
}

char* url_mergenormalize(char* url, char* base_url, char* ref_url, int pageisutf8, iconv_t cd, char* output_url_buf, int buf_size)
{
    int   url_len, len, isfree = 0;
    char* file;
    char *murl, *p;

    // url路径中可能有汉字，需要根据来源网页的编码进行转码，避免出现404错误。
    p = url_transcode(url, pageisutf8, cd);
    if (p) {
        url    = p;
        isfree = 1;
    }

    // 先对URL进行重编码，汉字同样会进行转码。例如:
    // http://img.star365.com/images/baopinye0930/fish oil_90's/image/fish-oil_90's_02.jpg
    p = reencode_escapes(url);
    if (p != url) {
        if (isfree) xfree(url);
        url    = p;
        isfree = 1;
    }

    url_len = strlen(url);
    if (strstartwithignorecase("http://", url)) { /* absolute url */
        if (url_len >= buf_size)
            goto ERROR;
        else
            strcpy(output_url_buf, url);
    } else if (base_url != NULL) { /* base url + relative url */
        if (!strstartwithignorecase("http://", base_url))
            goto ERROR;

        if (url_startwithprotocol(url)) /* Unknown protocol (mailto, ftp, news, file, gopher...) */
            goto ERROR;

        if (strstartwithignorecase("http:", url))
            murl = url_merge(base_url, url + 5);
        else
            murl = url_merge(base_url, url);

        len = strlen(murl);
        if (len >= buf_size) {
            xfree(murl);
            goto ERROR;
        } else {
            strcpy(output_url_buf, murl);
            xfree(murl);
        }
    } else if (ref_url != NULL) { /* refer url + relative url */
        if (!strstartwithignorecase("http://", ref_url))
            goto ERROR;

        if (url_startwithprotocol(url))
            goto ERROR; /* Unknown protocol (mailto, ftp, news, file, gopher...) */

        if (strstartwithignorecase("http:", url))
            murl = url_merge(ref_url, url + 5);
        else
            murl = url_merge(ref_url, url);

        len = strlen(murl);
        if (len >= buf_size) {
            xfree(murl);
            goto ERROR;
        } else {
            strcpy(output_url_buf, murl);
            xfree(murl);
        }
    } else {
        goto ERROR;
    }

    file = strchr(output_url_buf + 7, '/');
    if (!file) goto ERROR;
    if (!url_filenormalize(file)) goto ERROR;

    if (isfree) xfree(url);
    return output_url_buf;

ERROR:
    if (isfree) xfree(url);
    return NULL;
}

#ifdef TEST_URL

int main()
{
    void*       qrylist;
    const char* cp;
    char        buf[1024], *p;
    iconv_t     cd;

    cd = iconv_open("UTF-8//IGNORE", "GBK");
    if (cd == (iconv_t)-1) {
        printf("iconv_open() error.\n");
        return -1;
    }

    p = url_transcode("http://www.joyi365.com/assets/webdata/02.jpg", 1, cd);
    if (p) printf("transcode: %s\n", p);
    p = url_transcode("http://tsl.dajiankang.com/webimg/productinfo/1.jpg", 1, cd);
    if (p) printf("transcode: %s\n", p);
    p = url_transcode("http://img.star365.com/images/baopinye0930/fish oil_90's/image/fish-oil_90's_02.jpg", 1, cd);
    if (p) printf("transcode: %s\n", p);

    p = url_mergenormalize("http://www.baidu.com/s?wd=24%C8%AF&rsv_bp= 0&rsv_spt=3&oq=2&rsp=3&f=3&rsv_sug2=1&inputT=2059", NULL, NULL, 1, cd, buf, sizeof(buf));
    p = url_mergenormalize("http://img.star365.com/images/baopinye0930/fish oil_90's/image/fish-oil_90's_02.jpg", NULL, NULL, 1, cd, buf, sizeof(buf));
    p = url_mergenormalize("/images/baopinye0930/fish oil_90's/image/fish-oil_90's_02.jpg", NULL, "http://www.huihao.com/yswShow.php?id=2081", 1, cd, buf, sizeof(buf));
    p = url_mergenormalize("http://www.joyi365.com/assets/webdata/强生亮眸2周抛_02.jpg", NULL, "http://www.huihao.com/yswShow.php?id=2081", 1, cd, buf, sizeof(buf));
    p = url_mergenormalize("./../../../uploadfile/UpPicmore/2.jpg", NULL, "http://www.huihao.com/yswShow.php?id=2081", 1, cd, buf, sizeof(buf));

    strcpy(buf, "/./../../../uploadfile/UpPicmore/2.jpg");
    url_filenormalize(buf);

    qrylist = url_qryparse_init();
    // url_qryparse(qrylist,"wd=%E5%88%98%E5%85%B4%E4%B9%8B&rsp=7&oq=i&f=1&rsv_ers=xn0",1);
    // url_qryparse(qrylist,"wd=%E5%88%98%E5%85%B4%E4%B9%8B&rsp=7&oq=i&f=1&rsv_ers=xn0",0);
    url_qryparse2(qrylist, "wd=%E5%88%98%E5%85%B4%E4%B9%8B&rsp=7&oq=i&f=1&rsv_ers=xn0", 1, '=', '&');
    cp = url_qryparse_get(qrylist, "wd");
    if (cp) printf("wd: %s\n", cp);
    cp = url_qryparse_get(qrylist, "rsp");
    if (cp) printf("rsp: %s\n", cp);
    cp = url_qryparse_get(qrylist, "oq");
    if (cp) printf("oq: %s\n", cp);
    cp = url_qryparse_get(qrylist, "rsv_ers");
    if (cp) printf("rsv_ers: %s\n", cp);
    cp = url_qryparse_get(qrylist, "f");
    if (cp) printf("f: %s\n", cp);
    cp = url_qryparse_get(qrylist, "pn");
    if (cp) printf("pn: %s\n", cp);
    url_qryparse_free(qrylist);

    iconv_close(cd);
    return 0;
}

#endif
