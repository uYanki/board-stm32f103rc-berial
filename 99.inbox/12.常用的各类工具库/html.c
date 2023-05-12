/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "comn.h"
#include "htmlextract.h"
#include "xmalloc.h"
#include "xstring.h"
#include "hbm.h"
#include "charset.h"
#include "html.h"
#include "utils.h"

static spec_tag_t spectaglist[] = {
    {"script",   "</script",   SKIP_TAG, NULL, NULL},
    {"style",    "</style",    SKIP_TAG, NULL, NULL},
 //{"form",      "</form",     SKIP_TAG,   NULL,   NULL},
    {"label",    "</label",    SKIP_TAG, NULL, NULL},
    {"button",   "</button",   SKIP_TAG, NULL, NULL},
    {"fieldset", "</fieldset", SKIP_TAG, NULL, NULL},
    {"noscript", "</noscript", SKIP_TAG, NULL, NULL},
    {"map",      "</map",      SKIP_TAG, NULL, NULL},
    {"noframes", "</noframes", SKIP_TAG, NULL, NULL},
    {"iframe",   "</iframe",   SKIP_TAG, NULL, NULL},
    {"select",   "</select",   SKIP_TAG, NULL, NULL},
    {"textarea", "</textarea", SKIP_TAG, NULL, NULL}
};

static tag_attr_t tagattrlist[] = {
    {"src", "img", NULL, NULL},
    {"alt", "img", NULL, NULL}  //  保留原来img中的alt text,必须设置，不能去掉
};

typedef struct
{
    int      xssfilter;
    buffer_t textbuf;

    spec_tag_t*        tags;
    tag_attr_t*        attrs;
    struct hash_table* spectagmap;
    struct hash_table* tagattrmap;

    int    reservetagnum;
    char** reservetags;

    int    skiptagnum;
    char** skiptagstart;
    char** skiptagclose;

    int    reserveattrnum;
    char** attrbeg;
    char** attrend;
} filtertag_t;

// reservetags,skiptags: tag,tag,...
// reserveattrs: attr:tag,...
// 多线程环境下，每一个线程都需要初始化一份
void* html_filtertag_init(const char* reservetags,
                          const char* skiptags,
                          const char* reserveattrs,
                          int         maxtextsize,
                          int         xssfilter)
{
    int          i, tagindex, attrindex, taglistsize, attrlistsize;
    filtertag_t* ft;
    char         buf[1024], *p;

    ft            = (filtertag_t*)xcalloc(1, sizeof(filtertag_t));
    ft->xssfilter = xssfilter;

    taglistsize = 0;
    if (reservetags) {
        ft->reservetagnum = strgetfrags(reservetags, ",", &ft->reservetags);
        if (ft->reservetagnum <= 0) {
            xfree(ft);
            return NULL;
        }

        taglistsize += ft->reservetagnum;
    }

    if (skiptags) {
        ft->skiptagnum = strgetfrags(skiptags, ",", &ft->skiptagstart);
        if (ft->skiptagnum <= 0)
            goto freemem;

        ft->skiptagclose = (char**)xcalloc(ft->skiptagnum, sizeof(char*));
        for (i = 0; i < ft->skiptagnum; i++) {
            snprintf(buf, sizeof(buf), "</%s", ft->skiptagstart[i]);
            ft->skiptagclose[i] = xstrdup(buf);
        }

        taglistsize += ft->skiptagnum;
    }

    attrlistsize = 0;
    if (reserveattrs) {
        ft->reserveattrnum = strgetfrags(reserveattrs, ",", &ft->attrbeg);
        if (ft->reserveattrnum <= 0)
            goto freemem1;

        ft->attrend = (char**)xcalloc(ft->reserveattrnum, sizeof(char*));
        for (i = 0; i < ft->reserveattrnum; i++) {
            p = strchr(ft->attrbeg[i], ':');
            if (!p) goto freemem2;
            *p++ = 0;
            trim(ft->attrbeg[i]);
            trim(p);
            ft->attrend[i] = xstrdup(p);
        }

        attrlistsize += ft->reserveattrnum;
    }

    if (!xssfilter) {
        taglistsize += countof(spectaglist);
        attrlistsize += countof(tagattrlist);
    }

    if (taglistsize)
        ft->tags = (spec_tag_t*)xcalloc(taglistsize, sizeof(spec_tag_t));
    if (attrlistsize)
        ft->attrs = (tag_attr_t*)xcalloc(attrlistsize, sizeof(tag_attr_t));

    tagindex  = 0;
    attrindex = 0;
    if (!xssfilter) {
        memcpy(ft->tags, spectaglist, sizeof(spec_tag_t) * countof(spectaglist));
        memcpy(ft->attrs, tagattrlist, sizeof(tag_attr_t) * countof(tagattrlist));
        tagindex  = countof(spectaglist);
        attrindex = countof(tagattrlist);
    }

    for (i = 0; i < ft->reservetagnum; i++) {
        ft->tags[tagindex + i].tag_name    = ft->reservetags[i];
        ft->tags[tagindex + i].end_str     = NULL;
        ft->tags[tagindex + i].type        = RESERVE_TAG;
        ft->tags[tagindex + i].handler     = NULL;
        ft->tags[tagindex + i].handler_arg = NULL;
    }

    tagindex += ft->reservetagnum;

    for (i = 0; i < ft->skiptagnum; i++) {
        ft->tags[tagindex + i].tag_name    = ft->skiptagstart[i];
        ft->tags[tagindex + i].end_str     = ft->skiptagclose[i];
        ft->tags[tagindex + i].type        = SKIP_TAG;
        ft->tags[tagindex + i].handler     = NULL;
        ft->tags[tagindex + i].handler_arg = NULL;
    }

    for (i = 0; i < ft->reserveattrnum; i++) {
        ft->attrs[attrindex + i].tag_attr = ft->attrbeg[i];
        ft->attrs[attrindex + i].tag_name = ft->attrend[i];
        ft->attrs[attrindex + i].next     = NULL;
        ft->attrs[attrindex + i].tail     = NULL;
    }

    INIT_BUFFER(ft->textbuf, maxtextsize);
    extract_text_init(ft->tags, taglistsize, &ft->spectagmap, ft->attrs, attrlistsize, &ft->tagattrmap);
    return ft;

freemem2:
    for (i = 0; i < ft->reserveattrnum; i++) {
        xfree(ft->attrbeg[i]);
        xfree(ft->attrend[i]);
    }
    xfree(ft->attrbeg);
    xfree(ft->attrend);
freemem1:
    for (i = 0; i < ft->skiptagnum; i++) {
        xfree(ft->skiptagstart[i]);
        xfree(ft->skiptagclose[i]);
    }
    xfree(ft->skiptagstart);
    xfree(ft->skiptagclose);
freemem:
    for (i = 0; i < ft->reservetagnum; i++)
        xfree(ft->reservetags[i]);
    xfree(ft->reservetags);
    xfree(ft);
    return NULL;
}

void html_filtertag_free(void* tags)
{
    int          i;
    filtertag_t* ft = (filtertag_t*)tags;

    extract_text_free(ft->spectagmap, ft->tagattrmap);
    FREE_BUFFER(ft->textbuf);

    for (i = 0; i < ft->reservetagnum; i++)
        xfree(ft->reservetags[i]);
    xfree(ft->reservetags);

    for (i = 0; i < ft->skiptagnum; i++) {
        xfree(ft->skiptagstart[i]);
        xfree(ft->skiptagclose[i]);
    }
    xfree(ft->skiptagstart);
    xfree(ft->skiptagclose);

    for (i = 0; i < ft->reserveattrnum; i++) {
        xfree(ft->attrbeg[i]);
        xfree(ft->attrend[i]);
    }
    xfree(ft->attrbeg);
    xfree(ft->attrend);

    xfree(ft->tags);
    xfree(ft->attrs);
    xfree(ft);
}

// 返回的是字符串。
const char* html_filtertags(void* tags, const char* html, int htmllen, int codetype, int* sp)
{
    filtertag_t* ft = (filtertag_t*)tags;

    if (!htmllen) {
        *sp = 0;
        return NULL;
    }

    CLEAR_BUFFER(ft->textbuf, ft->textbuf.size);
    extract_text(html, htmllen, &ft->textbuf, ft->spectagmap, ft->tagattrmap, 1, codetype, ft->xssfilter);
    if (ft->textbuf.pos) {
        *sp                            = ft->textbuf.pos;
        ft->textbuf.p[ft->textbuf.pos] = 0;
        return ft->textbuf.p;
    } else {
        *sp = 0;
        return NULL;
    }
}

///////////////////////////////////////////////////
#define NAME_CHAR_P(x) ((x) > 32 && (x) < 127 && (x) != '=' && (x) != '>' && (x) != '/')

static char* advance_declaration(char* beg, char* end)
{
    char* p          = beg;
    char  quote_char = '\0'; /* shut up, gcc! */
    char  ch;

    enum {
        AC_S_DONE,
        AC_S_BACKOUT,
        AC_S_BANG,
        AC_S_DEFAULT,
        AC_S_DCLNAME,
        AC_S_DASH1,
        AC_S_DASH2,
        AC_S_COMMENT,
        AC_S_DASH3,
        AC_S_DASH4,
        AC_S_QUOTE1,
        AC_S_IN_QUOTE,
        AC_S_QUOTE2
    } state = AC_S_BANG;

    if (beg == end)
        return beg;
    ch = *p++;

    /* It looked like a good idea to write this as a state machine, but
       now I wonder...  */

    while (state != AC_S_DONE && state != AC_S_BACKOUT) {
        if (p == end)
            state = AC_S_BACKOUT;
        switch (state) {
            case AC_S_DONE:
            case AC_S_BACKOUT:
                break;
            case AC_S_BANG:
                if (ch == '!') {
                    ch    = *p++;
                    state = AC_S_DEFAULT;
                } else
                    state = AC_S_BACKOUT;
                break;
            case AC_S_DEFAULT:
                switch (ch) {
                    case '-':
                        state = AC_S_DASH1;
                        break;
                    case ' ':
                    case '\t':
                    case '\r':
                    case '\n':
                        ch = *p++;
                        break;
                    case '>':
                        state = AC_S_DONE;
                        break;
                    case '\'':
                    case '\"':
                        state = AC_S_QUOTE1;
                        break;
                    default:
                        if (NAME_CHAR_P(ch))
                            state = AC_S_DCLNAME;
                        else
                            state = AC_S_BACKOUT;
                        break;
                }
                break;
            case AC_S_DCLNAME:
                if (ch == '-')
                    state = AC_S_DASH1;
                else if (NAME_CHAR_P(ch))
                    ch = *p++;
                else
                    state = AC_S_DEFAULT;
                break;
            case AC_S_QUOTE1:
                /* We must use 0x22 because broken assert macros choke on
                   '"' and '\"'.  */
                assert(ch == '\'' || ch == 0x22);
                quote_char = ch; /* cheating -- I really don't feel like
                        introducing more different states for
                        different quote characters. */
                ch         = *p++;
                state      = AC_S_IN_QUOTE;
                break;
            case AC_S_IN_QUOTE:
                if (ch == quote_char)
                    state = AC_S_QUOTE2;
                else
                    ch = *p++;
                break;
            case AC_S_QUOTE2:
                assert(ch == quote_char);
                ch    = *p++;
                state = AC_S_DEFAULT;
                break;
            case AC_S_DASH1:
                assert(ch == '-');
                ch    = *p++;
                state = AC_S_DASH2;
                break;
            case AC_S_DASH2:
                switch (ch) {
                    case '-':
                        ch    = *p++;
                        state = AC_S_COMMENT;
                        break;
                    default:
                        state = AC_S_BACKOUT;
                }
                break;
            case AC_S_COMMENT:
                switch (ch) {
                    case '-':
                        state = AC_S_DASH3;
                        break;
                    default:
                        ch = *p++;
                        break;
                }
                break;
            case AC_S_DASH3:
                assert(ch == '-');
                ch    = *p++;
                state = AC_S_DASH4;
                break;
            case AC_S_DASH4:
                switch (ch) {
                    case '-':
                        ch    = *p++;
                        state = AC_S_DEFAULT;
                        break;
                    default:
                        state = AC_S_COMMENT;
                        break;
                }
                break;
        }
    }

    if (state == AC_S_BACKOUT) {
        return beg + 1;
    }
    return p;
}

/* Find the first occurrence of the substring "-->" in [BEG, END) and
   return the pointer to the character after the substring.  If the
   substring is not found, return NULL.  */

static char* find_comment_end(char* beg, char* end)
{
    /* Open-coded Boyer-Moore search for "-->".  Examine the third char;
       if it's not '>' or '-', advance by three characters.  Otherwise,
       look at the preceding characters and try to find a match.  */

    char* p = beg - 1;

    while ((p += 3) < end)
        switch (p[0]) {
            case '>':
                if (p[-1] == '-' && p[-2] == '-')
                    return p + 1;
                break;
            case '-':
            at_dash:
                if (p[-1] == '-') {
                at_dash_dash:
                    if (++p == end) return NULL;
                    switch (p[0]) {
                        case '>':
                            return p + 1;
                        case '-':
                            goto at_dash_dash;
                    }
                } else {
                    if ((p += 2) >= end) return NULL;
                    switch (p[0]) {
                        case '>':
                            if (p[-1] == '-')
                                return p + 1;
                            break;
                        case '-':
                            goto at_dash;
                    }
                }
        }
    return NULL;
}

/* Advance P (a char pointer), with the explicit intent of being able
   to read the next character.  If this is not possible, go to finish.  */

#define ADVANCE(p)       \
    do {                 \
        ++p;             \
        if (p >= end)    \
            goto finish; \
    } while (0)

/* Skip whitespace, if any. */

#define SKIP_WS(p)            \
    do {                      \
        while (ISSPACE(*p)) { \
            ADVANCE(p);       \
        }                     \
    } while (0)

/* Skip non-whitespace, if any. */

#define SKIP_NON_WS(p)         \
    do {                       \
        while (!ISSPACE(*p)) { \
            ADVANCE(p);        \
        }                      \
    } while (0)

/* 根据标签的开始mark('<'),找到对应的结束(mark'>')
 * 如果不需要获得标签名，则tagname_beg,tagname_end设为NULL.
 * 因为标签的属性中可以包含js代码，也就可能包含'>'符合。
 */

char* html_findtagclosemark(char* tag_start_mark, int len, char** tagname_beg, char** tagname_end)
{
    char *p, *end;
    char *tag_name_begin, *tag_name_end;
    int   end_tag;

    if (!tag_start_mark || !len)
        return NULL;

    p       = tag_start_mark;
    end     = tag_start_mark + len;
    end_tag = 0;

    ADVANCE(p);
    if (*p == '!') {
        // comment, declaration tag 处理后的返回值要做检查，
        // 只有找到一个正确的注释和声明tag且p指向的是tag的结束字符。
        char* old_p = p;
        if (p < end + 3 && p[1] == '-' && p[2] == '-')  // comment tag
        {
            char* comment_end = find_comment_end(p + 3, end);
            if (comment_end)
                p = comment_end;
        } else {
            p = advance_declaration(p, end);  // declaration tag
        }

        if (p != old_p && p != old_p + 1)
            goto noname_result;
        else
            goto finish;
    } else if (*p == '/')  // close tag
    {
        end_tag = 1;
        ADVANCE(p);
        if (tagname_beg)
            *tagname_beg = NULL;
        if (tagname_end)
            *tagname_end = NULL;
    }

    // tagname
    tag_name_begin = p;
    while (NAME_CHAR_P(*p))
        ADVANCE(p);

    if (p == tag_name_begin)
        goto finish;

    tag_name_end = p;
    SKIP_WS(p);
    if (end_tag && *p != '>')
        goto finish;
    else if (end_tag && *p == '>')
        return p;

    /* start tag or like xml tag,Find the attributes. */
    while (1) {
        char *attr_name_begin, *attr_name_end;
        char *attr_value_begin, *attr_value_end;

        SKIP_WS(p);
        if (*p == '/')  // like XML tag,<tagname />
        {
            /* <foo a=b c=d /> */
            /*                         ^    */
            ADVANCE(p);
            SKIP_WS(p);
            if (*p != '>')
                goto finish;
        }

        /* Check for end of tag definition. */
        if (*p == '>')
            goto name_result;

        /* Establish bounds of attribute name. */
        attr_name_begin = p;
        /* <foo bar ...> */
        /*          ^        */
        while (NAME_CHAR_P(*p))
            ADVANCE(p);
        attr_name_end = p;
        /* <foo bar ...> */
        /*                ^     */
        if (attr_name_begin == attr_name_end)
            goto finish;

        /* Establish bounds of attribute value. */
        SKIP_WS(p);
        if (NAME_CHAR_P(*p) || *p == '/' || *p == '>') {
            /* Minimized attribute syntax allows `=' to be omitted.
               For example, <UL COMPACT> is a valid shorthand for <UL
               COMPACT="compact">.  Even if such attributes are not
               useful to Wget, we need to support them, so that the
               tags containing them can be parsed correctly. */
            attr_value_begin = attr_name_begin;
            attr_value_end   = attr_name_end;
        } else if (*p == '=') {
            ADVANCE(p);
            SKIP_WS(p);
            if (*p == '\"' || *p == '\'') {
                int  newline_seen = 0;
                char quote_char   = *p;

                ADVANCE(p);
                attr_value_begin = p; /* <foo bar="baz"> */
                /*                    ^     */
                while (*p != quote_char) {
                    if (!newline_seen && *p == '\n') {
                        /* If a newline is seen within the quotes, it
                           is most likely that someone forgot to close
                           the quote.  In that case, we back out to
                           the value beginning, and terminate the tag
                           at either `>' or the delimiter, whichever
                           comes first.  Such a tag terminated at `>'
                           is discarded.  */
                        p            = attr_value_begin;
                        newline_seen = 1;
                        continue;
                    } else if (newline_seen && *p == '>')
                        break;

                    ADVANCE(p);
                }                   // 退出这个while循环有两种情况，遇到quote_char or 出现换行并且遇到'>'字符。
                attr_value_end = p; /* <foo bar="baz"> */
                /*              ^  */
                if (*p == quote_char)
                    ADVANCE(p);
                else
                    goto name_result;
            } else {
                attr_value_begin = p; /* <foo bar=baz> */

                /* According to SGML, a name token should consist only
                   of alphanumerics, . and -.  However, this is often
                   violated by, for instance, `%' in `width=75%'.
                   We'll be liberal and allow just about anything as
                   an attribute value.  */
                while (!ISSPACE(*p) && *p != '>')
                    ADVANCE(p);
                attr_value_end = p; /* <foo bar=baz qux=quix> */

                if (attr_value_begin == attr_value_end) /* <foo bar=> */
                    goto finish;
            }
        } else {
            /* We skipped the whitespace and found something that is
               neither `=' nor the beginning of the next attribute's name.  Back out.  */
            goto finish; /* <foo bar [... */
        }
    }

name_result:
    if (tagname_beg)
        *tagname_beg = tag_name_begin;
    if (tagname_end)
        *tagname_end = tag_name_end;
    return p;

noname_result:
    if (tagname_beg)
        *tagname_beg = NULL;
    if (tagname_end)
        *tagname_end = NULL;
    return (p - 1);

finish:
    return NULL;
}

// 过滤掉raw page中的js脚本,这样天涯、雅虎问答快照就不会出现问题。
// input != output，输出buffer大小不能小于输入内容长度
int html_filterjavascript(const char* input, int inlen, char* output, int outlen, int codetype)
{
    char *pb, *pe;       // 指向js标记的开始和结束
    char *txt, *txtend;  // 指向查找正文的开始和结束
    int   len, pos;      // 输出buffer的位置

    txt    = (char*)input;
    txtend = (char*)(input + inlen);
    pos    = 0;

    // <script ...> ... </script ...>
    while ((pb = (char*)hbm_patternmatch((uint8_t*)txt, txtend - txt, (uint8_t*)"<script", 7, 1, codetype)) != NULL) {
        pe = stristrb(pb, txtend - pb, "</script", 8);
        if (!pe) break;
        pe = html_findtagclosemark(pe, txtend - pe, NULL, NULL);
        if (!pe) break;

        len = pb - txt;
        if (len != 0) {
            memcpy(output + pos, txt, len);
            pos += len;
        }

        txt = pe + 1;
    }

    memcpy(output + pos, txt, txtend - txt);
    pos += (txtend - txt);
    output[pos] = 0;
    return pos;
}

// 提取的URL存放到buffer中,可以指定URL前缀, 错误返回-1,没有图片返回0,ok 返回1
int html_getimgsrc(const char* html, const char* prefix, char* buffer, int bufsize)
{
    char *beg, *end;
    int   len, plen;

    beg = stristr(html, "<img src=\"");
    if (!beg) return 0;
    beg += strlen("<img src=\"");
    end = strchr(beg, '"');
    if (!end) return 0;

    if (!prefix)
        plen = 0;
    else
        plen = strlen(prefix);

    len = end - beg;
    if ((len + plen) >= bufsize) return -1;

    if (!prefix) {
        memcpy(buffer, beg, len);
        buffer[len] = 0;
    } else {
        memcpy(buffer, prefix, plen);
        memcpy(buffer + plen, beg, len);
        buffer[plen + len] = 0;
    }

    return 1;
}

// @return: need free it.
// 注意: 只支持标准的格式的修改: <img src="..." alt="old" /> -->
//                               <img src="..." alt="new" title="..." />
char* html_modifiedimgattr(const char* html, const char* alt, const char* title)
{
    int      htmlen;
    char *   start, *end, *imgbeg, *imgend, *srcbeg, *srcend, *altbeg, *altend;
    buffer_t resbuf;

    htmlen = strlen(html);
    INIT_BUFFER(resbuf, htmlen * 2);

    start = (char*)html;
    end   = (char*)(html + htmlen);
    while ((imgbeg = stristr(start, "<img"))) {
        imgend = strchr(imgbeg, '>');
        if (!imgend) goto freemem;
        imgend++;

        srcbeg = stristrb(imgbeg, imgend - imgbeg, "src=\"", strlen("src=\""));
        if (!srcbeg) goto freemem;
        srcbeg += strlen("src=\"");
        srcend = stristrb(srcbeg, imgend - srcbeg, "\"", 1);
        if (!srcend) goto freemem;

        altbeg = stristrb(imgbeg, imgend - imgbeg, "alt=\"", strlen("alt=\""));
        if (altbeg) {
            altbeg += strlen("alt=\"");
            altend = stristrb(altbeg, imgend - altbeg, "\"", 1);
            if (!altend) goto freemem;
        }

        APPEND_BUFFER(resbuf, start, imgbeg - start);
        APPEND_BUFFER(resbuf, "<img src=\"", strlen("<img src=\""));
        APPEND_BUFFER(resbuf, srcbeg, srcend - srcbeg);
        APPEND_BUFFER(resbuf, "\" ", 2);
        if (alt) {
            APPEND_BUFFER(resbuf, "alt=\"", strlen("alt=\""));
            APPEND_BUFFER(resbuf, alt, strlen(alt));
            APPEND_BUFFER(resbuf, "\" ", 2);
        } else if (altbeg) {
            APPEND_BUFFER(resbuf, "alt=\"", strlen("alt=\""));
            APPEND_BUFFER(resbuf, altbeg, altend - altbeg);
            APPEND_BUFFER(resbuf, "\" ", 2);
        }

        if (title) {
            APPEND_BUFFER(resbuf, "title=\"", strlen("title=\""));
            APPEND_BUFFER(resbuf, title, strlen(title));
            APPEND_BUFFER(resbuf, "\" ", 2);
        }

        APPEND_BUFFER(resbuf, "/>", 2);
        start = imgend;
    }

    APPEND_BUFFER(resbuf, start, end - start);
    return resbuf.p;

freemem:
    FREE_BUFFER(resbuf);
    return NULL;
}

/* HTML文本中关键词高亮,注意: title和html tag 中的文本不要高亮。
 * <B style='color:black;background-color:#ffff66'>百度</B>
 * <span style="background:#ffff66;color:black;font-weight:bold">google</span>
 *
 * 关键词定位
 * 跳转到本页面中的另外一个地方: <a href="#name"> ... </a> <a name="name"> ... </a>
 * 跳转到其他页面中的某一个地方: <a href="URL#name"> ... </a> <a name="name"> ... </a>
 * 注意: anchor text被定位时会出现链接失效的问题。
 * <a href="http://www.baidu.com/"> <a name="f"></a> this</a>
 * 应该为<a name="gekangsnapshot0"></a> <a href="http://www.baidu.com/">this</a>
 */

typedef struct
{
    int   count;  // 被高亮的次数，第一次高亮时带定位信息。
    char* style;  // 关键词高亮显示样式
} highlight_t;

// 如果要高亮的是anchor text，且有第一次高亮的keyword。<a name=""></a> <a href=""><span ...>this</span></a>
typedef struct
{
    int   is_anchor_text;
    char* anchor_tag_begin;
    char* anchor_tag_end;
} hlposarg_t;

#define COPY_TO_BUFFER(B, BLEN, BSIZE, S, SLEN) \
    if ((SLEN) < ((BSIZE) - (BLEN))) {          \
        memcpy(B + BLEN, S, SLEN);              \
        BLEN += (SLEN);                         \
    } else {                                    \
        return -1;                              \
    }

// only support utf-8
static int highlight_pos_replace(char* text, int text_len, char* kw[], highlight_t* hl, char* first_pos[], int kw_num, hlposarg_t* arg, char* outputbuffer, int bufsize)
{
    int   i, j, len, buf_len, clen;
    char *po, *po_end;
    char* poo;
    char  firstposinfo[2048];
    int   pos_len;
    char  anchor_text_buffer[2048];
    int   anchor_text_len;

    po     = text;
    po_end = po + text_len;
    i      = 0;
    j      = 0;

    buf_len               = 0;
    firstposinfo[0]       = 0;
    pos_len               = 0;
    anchor_text_buffer[0] = 0;
    anchor_text_len       = 0;

    if (!arg->is_anchor_text)  // 直接把hightlight text --> outputbuffer
    {
    restart:
        while (po != po_end) {
            for (i = 0; i < kw_num; i++) {
                poo = po;
                while (*po == *(*(kw + i) + j)) {
                    if (!*(*(kw + i) + j + 1)) {
                        if (hl[i].count == 0) {
                            COPY_TO_BUFFER(outputbuffer, buf_len, bufsize, first_pos[i], strlen(first_pos[i]))
                        }
                        COPY_TO_BUFFER(outputbuffer, buf_len, bufsize, hl[i].style, strlen(hl[i].style))
                        hl[i].count++;
                        po++;
                        j = 0;
                        goto restart;
                    }

                    po++;
                    j++;
                }

                po = poo;
                j  = 0;
            }

            // 需要考虑多字节字符: GBK中"等他" --> B5 C8 CB FB  "人"--> C8 CB，这样就会被过滤或替换掉。
            clen = UTF8CHLEN(*((uint8_t*)po));
            if (po + clen > po_end)  // 出界
                clen = po_end - po;
            COPY_TO_BUFFER(outputbuffer, buf_len, bufsize, po, clen);
            po += clen;
        }
    } else  // 分成三段: positon info ,anchor start tag,hightlight anchor text --> outputbuffer
    {
    restart_a:
        while (po != po_end) {
            for (i = 0; i < kw_num; i++) {
                poo = po;
                while (*po == *(*(kw + i) + j)) {
                    if (!*(*(kw + i) + j + 1)) {
                        if (hl[i].count == 0) {
                            COPY_TO_BUFFER(firstposinfo, pos_len, 2047, first_pos[i], strlen(first_pos[i]))
                        }
                        COPY_TO_BUFFER(anchor_text_buffer, anchor_text_len, 2047, hl[i].style, strlen(hl[i].style))
                        hl[i].count++;
                        po++;
                        j = 0;
                        goto restart_a;
                    }

                    po++;
                    j++;
                }

                po = poo;
                j  = 0;
            }

            clen = UTF8CHLEN(*((uint8_t*)po));
            if (po + clen > po_end)  // 出界
                clen = po_end - po;
            COPY_TO_BUFFER(anchor_text_buffer, anchor_text_len, 2047, po, clen);
            po += clen;
        }

        COPY_TO_BUFFER(outputbuffer, buf_len, bufsize, firstposinfo, pos_len)
        len = arg->anchor_tag_end - arg->anchor_tag_begin + 1;
        COPY_TO_BUFFER(outputbuffer, buf_len, bufsize, arg->anchor_tag_begin, len)
        COPY_TO_BUFFER(outputbuffer, buf_len, bufsize, anchor_text_buffer, anchor_text_len)
    }

    return buf_len;
}

typedef struct
{
    char* tag_name;
    char* end_str;
} block_tag_t;

block_tag_t block_list[] =
    {
        {"title",    "</title"   }, /* special put on first position */
        {"a",        "</a"       }, /* special put on second position */
        {"script",   "</script"  },
        {"style",    "</style"   },
        {"form",     "</form"    },
        {"noscript", "</noscript"},
        {"map",      "</map"     },
        {"noframes", "</noframes"},
        {"iframe",   "</iframe"  },
        {"select",   "</select"  },
        {"textarea", "</textarea"}
};

/*
 * FUNCTION: find the tagname's offset in block_list[]
 * RETURN: offset of `tagname' in block_list[] if `tagname' is in block_list[] else -1
 */
static int block_tag_search(const char* tagname, int name_len, block_tag_t* block_list, int blk_size)
{
    int i;
    for (i = 0; i < blk_size; i++) {
        if (strncasecmp(tagname, (block_list + i)->tag_name, name_len) == 0)
            return i;
    }
    return -1;
}

// @功能: 关键词高亮和定位。title ==NULL,表示不提取标题
int html_highlight_position(char* page, int page_size,
                            char* kw[],   // 高亮关键词
                            char* hl[],   // 带高亮样式的关键词: <span style="...">关键词</span>
                            char* pos[],  // 关键词定位信息: <a name="gksnapshot1"></a>
                            int   kw_num,
                            char* hl_page,
                            int   hlpage_bufsize,
                            char* title,
                            int   title_bufsize)
{
    char *      p, *end, *des, *q;
    char *      tag_beg, *tag_end;
    int         des_len, des_size;
    int         i, len, result;
    highlight_t hlight[MAX_HLIGHT_KW];
    hlposarg_t  arg;
    char *      tagname_beg, *tagname_end;
    int         blk_size = sizeof(block_list) / sizeof(block_tag_t);

    if (!page || !page_size || !kw || !hl || !kw_num || !hl_page || !hlpage_bufsize || !pos)
        return -1;

    if (kw_num > MAX_HLIGHT_KW)
        return -1;

    p        = page;
    end      = p + page_size;
    des      = hl_page;
    des_len  = 0;
    des_size = hlpage_bufsize;

    for (i = 0; i < kw_num; i++) {
        hlight[i].count = 0;
        hlight[i].style = hl[i];
    }

    // xxx<tag ...> ...</tag>...
    //^    ^          ^
    // p  tag_beg tag_end

look_for_tag:
    arg.is_anchor_text = 0;
    len                = end - p;
    tag_beg            = memchr(p, '<', len);
    if (!tag_beg)
        goto hl_pos_replace_end;

    tag_end = html_findtagclosemark(tag_beg, end - tag_beg, &tagname_beg, &tagname_end);
    if (!tag_end) {
        result = highlight_pos_replace(p, tag_beg - p + 1, kw, hlight, pos, kw_num, &arg, des + des_len, des_size - des_len);
        if (result == -1)
            return -1;
        des_len += result;

        p = tag_beg + 1;
        goto look_for_tag;
    }

    // replace  p ~ tag_beg
    result = highlight_pos_replace(p, tag_beg - p, kw, hlight, pos, kw_num, &arg, des + des_len, des_size - des_len);
    if (result == -1)
        return -1;
    des_len += result;

    // copy tag to buffer and extract title or replace anchor text
    if (tagname_beg && tagname_end) {
        result = block_tag_search(tagname_beg, tagname_end - tagname_beg, block_list, blk_size);
        if (result == 0 && title)  // title
        {
            q = stristrb(tag_beg, end - tag_beg, block_list[result].end_str, strlen(block_list[result].end_str));
            if (q) {
                len = q - tag_end - 1;
                if (len <= title_bufsize) {
                    char* tmp;
                    memcpy(title, tag_end + 1, len);
                    title[len] = 0;

                    // 去掉网站名，_  or -开始
                    while ((tmp = strrchr(title, '_')) != NULL || (tmp = strrchr(title, '-')) != NULL || (tmp = strstr(title, "－")) != NULL || (tmp = strrchr(title, '|')) != NULL) {
                        if (tmp == title)
                            break;

                        *tmp = 0;
                    }
                }

                tag_end = q - 1;
            }
            // not a </title,copy to <title> to buffer
        } else if (result == 1)  // anchor
        {
            q = stristrb(tag_beg, end - tag_beg, block_list[result].end_str, strlen(block_list[result].end_str));
            if (q) {
                arg.is_anchor_text   = 1;
                arg.anchor_tag_begin = tag_beg;
                arg.anchor_tag_end   = tag_end;
                result               = highlight_pos_replace(tag_end + 1, q - tag_end - 1, kw, hlight, pos, kw_num, &arg, des + des_len, des_size - des_len);
                if (result == -1)
                    return -1;
                des_len += result;

                p = q;
                goto look_for_tag;
            }

        } else if (result >= 0)  // block tag,当不提取标题时，title标签在这里处理
        {
            q = stristrb(tag_beg, end - tag_beg, block_list[result].end_str, strlen(block_list[result].end_str));
            if (q) {
                tag_end = q - 1;
            }
        } else  // not block tag
        {
        }
    }

    // copy tag to buffer
    COPY_TO_BUFFER(des, des_len, des_size, tag_beg, tag_end - tag_beg + 1)
    p = tag_end + 1;
    goto look_for_tag;

hl_pos_replace_end:
    result = highlight_pos_replace(p, len, kw, hlight, pos, kw_num, &arg, des + des_len, des_size - des_len);
    if (result == -1)
        return -1;
    des_len += result;
    return des_len;
}

#ifdef TEST_HTML

int main()
{
    void*       tags;
    char *      html, buf[500 * 1024], *p;
    int         htmllen, sp;
    const char* cp;

    tags = html_filtertag_init("img,a,meta", NULL, "href:a,title:a,name:meta,content:meta", 1024 * 1024, 0);
    if (!tags) {
        printf("html_filtertag_init() error.\n");
        return -1;
    }

    html = loadfile("test/test.html", (long*)&htmllen);
    if (!htmllen) {
        printf("can't load file.\n");
        return -1;
    }

    cp = html_filtertags(tags, html, htmllen, CODETYPE_UTF8, &sp);
    savefile("test/filtertag.html", (void*)cp, sp);

    // filter js
    sp = html_filterjavascript(html, htmllen, buf, sizeof(buf), CODETYPE_UTF8);
    savefile("test/filterjs.html", buf, sp);

    // get img src
    html_getimgsrc(cp, NULL, buf, sizeof(buf));
    printf("%s\n", buf);
    html_getimgsrc(cp, "http://img.gekang.com/", buf, sizeof(buf));
    printf("%s\n", buf);

    // modified img attribute
    p = html_modifiedimgattr(cp, "test", "testmodified");
    if (!p) {
        printf("html_modifiedimgattr() error.\n");
        return -1;
    }
    savefile("test/modifiedimgattr.html", p, strlen(p));
    xfree(p);

    xfree(html);
    html_filtertag_free(tags);
    return 0;
}

#endif
