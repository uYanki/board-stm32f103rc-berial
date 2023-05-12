#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include "urlparse.h"

#ifndef TEST_URLPARSE
#include "xmalloc.h"
#else
#define xmalloc malloc
#define xstrdup strdup
#define xfree   free
#define xmalloc0 malloc
#define xfree_null(p) if (!(p)) ; else xfree (p)
#define xnew0(type) ((type *) xmalloc0 (sizeof (type)))
#endif 

#define ISDIGIT(x)  isdigit (x)
#define ISXDIGIT(x) isxdigit (x)
#define ISALNUM(x)  isalnum (x)
#define TOLOWER(x)  tolower (x)
#define TOUPPER(x)  toupper (x)
#define ISUPPER(x)  isupper (x)

static int lowercase_str (char *str);
static char * strdupdelim (const char *beg, const char *end);
static char *strpbrk_or_eos (const char *s, const char *accept);

/* The number of elements in an array.  For example:
   static char a[] = "foo";     -- countof(a) == 4 (note terminating \0)
   int a[5] = {1, 2};           -- countof(a) == 5
   char *a[] = {                -- countof(a) == 3
   "foo", "bar", "baz"
   }; */
#define countof(array) (sizeof (array) / sizeof ((array)[0]))

/* Convert an ASCII hex digit to the corresponding number between 0
   and 15.  H should be a hexadecimal digit that satisfies isxdigit;
   otherwise, the result is undefined.  */
#define XDIGIT_TO_NUM(h) ((h) < 'A' ? (h) - '0' : TOUPPER (h) - 'A' + 10)
#define X2DIGITS_TO_NUM(h1, h2) ((XDIGIT_TO_NUM (h1) << 4) + XDIGIT_TO_NUM (h2))

/* The reverse of the above: convert a number in the [0, 16) range to
   the ASCII representation of the corresponding hexadecimal digit.
   `+ 0' is there so you can't accidentally use it as an lvalue.  */
#define XNUM_TO_DIGIT(x) ("0123456789ABCDEF"[x] + 0)
#define XNUM_TO_digit(x) ("0123456789abcdef"[x] + 0)

#define SCHEME_CHAR(ch)   (ISALNUM (ch) || (ch) == '-' || (ch) == '+')

/* Forward declarations: */
static int path_simplify (char *);
static void split_path(const char *, char **, char **);

#define PE_NO_ERROR			              0
#define PE_UNSUPPORTED_SCHEME		      1
#define PE_INVALID_HOST_NAME		      2
#define PE_BAD_PORT_NUMBER		        3
#define PE_INVALID_USER_NAME		      4
#define PE_UNTERMINATED_IPV6_ADDRESS	5
#define PE_IPV6_NOT_SUPPORTED		      6
#define PE_INVALID_IPV6_ADDRESS		    7

static const char *parse_errors[] = {
    "No error",
    "Unsupported scheme",
    "Invalid host name",
    "Bad port number",
    "Invalid user name",
    "Unterminated IPv6 numeric address",
    "IPv6 addresses not supported",
    "Invalid IPv6 numeric address"
};

struct scheme_data
{
    const char *name;
    const char *leading_string;
    int default_port;
    int enabled;
};

/* Supported schemes: */
static struct scheme_data supported_schemes[] =
{
    { "http",	  "http://",  DEFAULT_HTTP_PORT,  1 },
    { "https",	"https://", DEFAULT_HTTPS_PORT, 1 },
    { "ftp",	  "ftp://",   DEFAULT_FTP_PORT,   1 },
    { NULL,	    NULL,       -1,                 0 }  /* SCHEME_INVALID */
};


/* Count the digits in the provided number.  Used to allocate space
   when printing numbers.  */
static int numdigit (int number)
{
    int cnt = 1;
    if (number < 0)
	++cnt;			/* accomodate '-' */
    while ((number /= 10) != 0)
	++cnt;
    return cnt;
}

/* Print NUMBER to BUFFER in base 10.  This is equivalent to
   `sprintf(buffer, "%lld", (long long) number)', only typically much
   faster and portable to machines without long long.

   The speedup may make a difference in programs that frequently
   convert numbers to strings.  Some implementations of sprintf,
   particularly the one in GNU libc, have been known to be extremely
   slow when converting integers to strings.

   Return the pointer to the location where the terminating zero was
   printed.  (Equivalent to calling buffer+strlen(buffer) after the
   function is done.)

   BUFFER should be big enough to accept as many bytes as you expect
   the number to take up.  On machines with 64-bit longs the maximum
   needed size is 24 bytes.  That includes the digits needed for the
   largest 64-bit number, the `-' sign in case it's negative, and the
   terminating '\0'.  */

static char *number_to_string (char *buffer, int number)
{
    char *p = buffer;
    sprintf(p,"%d",number);
    p += strlen (buffer);
    return p;
}

/* Support for escaping and unescaping of URL strings.  */

/* Table of "reserved" and "unsafe" characters.  Those terms are
   rfc1738-speak, as such largely obsoleted by rfc2396 and later
   specs, but the general idea remains.

   A reserved character is the one that you can't decode without
   changing the meaning of the URL.  For example, you can't decode
   "/foo/%2f/bar" into "/foo///bar" because the number and contents of
   path components is different.  Non-reserved characters can be
   changed, so "/foo/%78/bar" is safe to change to "/foo/x/bar".  The
   unsafe characters are loosely based on rfc1738, plus "$" and ",",
   as recommended by rfc2396, and minus "~", which is very frequently
   used (and sometimes unrecognized as %7E by broken servers).

   An unsafe character is the one that should be encoded when URLs are
   placed in foreign environments.  E.g. space and newline are unsafe
   in HTTP contexts because HTTP uses them as separator and line
   terminator, so they must be encoded to %20 and %0A respectively.
   "*" is unsafe in shell context, etc.

   We determine whether a character is unsafe through static table
   lookup.  This code assumes ASCII character set and 8-bit chars.  */

enum {
    /* rfc1738 reserved chars + "$" and ",".  */
    urlchr_reserved = 1,

    /* rfc1738 unsafe chars, plus non-printables.  */
    urlchr_unsafe   = 2
};

#define urlchr_test(c, mask) (urlchr_table[(uint8_t)(c)] & (mask))
#define URL_RESERVED_CHAR(c) urlchr_test(c, urlchr_reserved)
#define URL_UNSAFE_CHAR(c) urlchr_test(c, urlchr_unsafe)

/* Shorthands for the table: */
#define R  urlchr_reserved
#define U  urlchr_unsafe
#define RU R|U

static const uint8_t urlchr_table[256] =
{
    U,  U,  U,  U,   U,  U,  U,  U,   /* NUL SOH STX ETX  EOT ENQ ACK BEL */
    U,  U,  U,  U,   U,  U,  U,  U,   /* BS  HT  LF  VT   FF  CR  SO  SI  */
    U,  U,  U,  U,   U,  U,  U,  U,   /* DLE DC1 DC2 DC3  DC4 NAK SYN ETB */
    U,  U,  U,  U,   U,  U,  U,  U,   /* CAN EM  SUB ESC  FS  GS  RS  US  */
    U,  0,  U, RU,   R,  U,  R,  0,   /* SP  !   "   #    $   %   &   '   */
    0,  0,  0,  R,   R,  0,  0,  R,   /* (   )   *   +    ,   -   .   /   */
    0,  0,  0,  0,   0,  0,  0,  0,   /* 0   1   2   3    4   5   6   7   */
    0,  0, RU,  R,   U,  R,  U,  R,   /* 8   9   :   ;    <   =   >   ?   */
    RU,  0,  0,  0,   0,  0,  0,  0,   /* @   A   B   C    D   E   F   G   */
    0,  0,  0,  0,   0,  0,  0,  0,   /* H   I   J   K    L   M   N   O   */
    0,  0,  0,  0,   0,  0,  0,  0,   /* P   Q   R   S    T   U   V   W   */
    0,  0,  0, RU,   U, RU,  U,  0,   /* X   Y   Z   [    \   ]   ^   _   */
    U,  0,  0,  0,   0,  0,  0,  0,   /* `   a   b   c    d   e   f   g   */
    0,  0,  0,  0,   0,  0,  0,  0,   /* h   i   j   k    l   m   n   o   */
    0,  0,  0,  0,   0,  0,  0,  0,   /* p   q   r   s    t   u   v   w   */
    0,  0,  0,  U,   U,  U,  0,  U,   /* x   y   z   {    |   }   ~   DEL */

    U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
    U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
    U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
    U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,

    U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
    U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
    U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
    U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
};
#undef R
#undef U
#undef RU

/* URL-unescape the string S.

   This is done by transforming the sequences "%HH" to the character
   represented by the hexadecimal digits HH.  If % is not followed by
   two hexadecimal digits, it is inserted literally.

   The transformation is done in place.  If you need the original
   string intact, make a copy before calling this function.  

   把字符串中的"%HH"编码转换为字符。
   注意：
   -- 如果%后没有两个十六进制数，则不转换。
   -- 该函数会修改字符串S，调用该函数时注意根据需要备份原字符串。 
 */
void url_unescape (char *s)
{
    char *t = s;			/* t - tortoise */
    char *h = s;			/* h - hare     */

    for (; *h; h++, t++)
    {
	if (*h != '%')
	{
copychar:
	    *t = *h;
	}
	else
	{
	    char c;
	    /* Do nothing if '%' is not followed by two hex digits. */
	    if (!h[1] || !h[2] || !(ISXDIGIT (h[1]) && ISXDIGIT (h[2])))
		goto copychar;
	    c = X2DIGITS_TO_NUM (h[1], h[2]);
	    /* Don't unescape %00 because there is no way to insert it
	       into a C string without effectively truncating it. */
	    if (c == '\0')
		goto copychar;
	    *t = c;
	    h += 2;
	}
    }
    *t = '\0';
}

/* The core of url_escape_* functions.  Escapes the characters that
   match the provided mask in urlchr_table.

   If ALLOW_PASSTHROUGH is non-zero, a string with no unsafe chars
   will be returned unchanged.  If ALLOW_PASSTHROUGH is zero, a
   freshly allocated string will be returned in all cases.  */
/* Note : error return NULL */

static char *url_escape_1 (const char *s, uint8_t mask, int allow_passthrough)
{
    const char *p1;
    char *p2, *newstr;
    int newlen;
    int addition = 0;
    /* char *pc; */

    for (p1 = s; *p1; p1++)
	if (urlchr_test (*p1, mask))
	    addition += 2;		/* Two more characters (hex digits) */

    if (!addition)
	return allow_passthrough ? (char *)s : xstrdup (s);

    newlen = (p1 - s) + addition;
    newstr = (char *)xmalloc (newlen + 1);

    p1 = s;
    p2 = newstr;
    while (*p1)
    {
	/* Quote the characters that match the test mask. */
	if (urlchr_test (*p1, mask))
	{
	    uint8_t c = *p1++;
	    *p2++ = '%';
	    *p2++ = XNUM_TO_DIGIT (c >> 4);
	    *p2++ = XNUM_TO_DIGIT (c & 0xf);
	}
	else
	    *p2++ = *p1++;
    }
    assert (p2 - newstr == newlen);
    *p2 = '\0';

    return newstr;
}

/* URL-escape the unsafe characters (see urlchr_table) in a given
   string, returning a freshly allocated string.  */

char *url_escape (const char *s)
{
    return url_escape_1 (s, urlchr_unsafe, 0);
}

/* URL-escape the unsafe characters (see urlchr_table) in a given
   string.  If no characters are unsafe, S is returned.  */

static char *url_escape_allow_passthrough (const char *s)
{
    return url_escape_1 (s, urlchr_unsafe, 1);
}

/* Decide whether the char at position P needs to be encoded.  (It is
   not enough to pass a single char *P because the function may need
   to inspect the surrounding context.)

   Return 1 if the char should be escaped as %XX, 0 otherwise.  */
static int char_needs_escaping (const char *p)
{
    if (*p == '%')
    {
	if (ISXDIGIT (*(p + 1)) && ISXDIGIT (*(p + 2)))
	    return 0;
	else
	    /* Garbled %.. sequence: encode `%'. */
	    return 1;
    }
    else if (URL_UNSAFE_CHAR (*p) && !URL_RESERVED_CHAR (*p))
	return 1;
    else
	return 0;
}

/* Translate a %-escaped (but possibly non-conformant) input string S
   into a %-escaped (and conformant) output string.  If no characters
   are encoded or decoded, return the same string S; otherwise, return
   a freshly allocated string with the new contents.

   After a URL has been run through this function, the protocols that
   use `%' as the quote character can use the resulting string as-is,
   while those that don't can use url_unescape to get to the intended
   data.  This function is stable: once the input is transformed,
   further transformations of the result yield the same output.

   Let's discuss why this function is needed.

   Imagine Wget is asked to retrieve `http://abc.xyz/abc def'.  Since
   a raw space character would mess up the HTTP request, it needs to
   be quoted, like this:

   GET /abc%20def HTTP/1.0

   It would appear that the unsafe chars need to be quoted, for
   example with url_escape.  But what if we're requested to download
   `abc%20def'?  url_escape transforms "%" to "%25", which would leave
   us with `abc%2520def'.  This is incorrect -- since %-escapes are
   part of URL syntax, "%20" is the correct way to denote a literal
   space on the Wget command line.  This leads to the conclusion that
   in that case Wget should not call url_escape, but leave the `%20'
   as is.  This is clearly contradictory, but it only gets worse.

   What if the requested URI is `abc%20 def'?  If we call url_escape,
   we end up with `/abc%2520%20def', which is almost certainly not
   intended.  If we don't call url_escape, we are left with the
   embedded space and cannot complete the request.  What the user
   meant was for Wget to request `/abc%20%20def', and this is where
   reencode_escapes kicks in.

   Wget used to solve this by first decoding %-quotes, and then
   encoding all the "unsafe" characters found in the resulting string.
   This was wrong because it didn't preserve certain URL special
   (reserved) characters.  For instance, URI containing "a%2B+b" (0x2b
   == '+') would get translated to "a%2B%2Bb" or "a++b" depending on
   whether we considered `+' reserved (it is).  One of these results
   is inevitable because by the second step we would lose information
   on whether the `+' was originally encoded or not.  Both results
   were wrong because in CGI parameters + means space, while %2B means
   literal plus.  reencode_escapes correctly translates the above to
   "a%2B+b", i.e. returns the original string.

   This function uses a modified version of the algorithm originally
   proposed by Anon Sricharoenchai:

 * Encode all "unsafe" characters, except those that are also
 "reserved", to %XX.  See urlchr_table for which characters are
 unsafe and reserved.

 * Encode the "%" characters not followed by two hex digits to
 "%25".

 * Pass through all other characters and %XX escapes as-is.  (Up to
 Wget 1.10 this decoded %XX escapes corresponding to "safe"
 characters, but that was obtrusive and broke some servers.)

 Anon's test case:

 "http://abc.xyz/%20%3F%%36%31%25aa% a?a=%61+a%2Ba&b=b%26c%3Dc"
 ->
 "http://abc.xyz/%20%3F%25%36%31%25aa%25%20a?a=%61+a%2Ba&b=b%26c%3Dc"

 Simpler test cases:

 "foo bar"         -> "foo%20bar"
 "foo%20bar"       -> "foo%20bar"
 "foo %20bar"      -> "foo%20%20bar"
 "foo%%20bar"      -> "foo%25%20bar"       (0x25 == '%')
 "foo%25%20bar"    -> "foo%25%20bar"
 "foo%2%20bar"     -> "foo%252%20bar"
 "foo+bar"         -> "foo+bar"            (plus is reserved!)
 "foo%2b+bar"      -> "foo%2b+bar"  */

char *reencode_escapes (const char *s)
{
    const char *p1;
    char *newstr, *p2;
    int oldlen, newlen;

    int encode_count = 0;

    /* First pass: inspect the string to see if there's anything to do,
       and to calculate the new length.  */
    for (p1 = s; *p1; p1++)
	if (char_needs_escaping (p1))
	    ++encode_count;

    if (!encode_count)
	/* The string is good as it is. */
	return (char *) s;		/* C const model sucks. */

    oldlen = p1 - s;
    /* Each encoding adds two characters (hex digits).  */
    newlen = oldlen + 2 * encode_count;
    newstr = xmalloc (newlen + 1);

    /* Second pass: copy the string to the destination address, encoding
       chars when needed.  */
    p1 = s;
    p2 = newstr;

    while (*p1)
	if (char_needs_escaping (p1))
	{
	    uint8_t c = *p1++;
	    *p2++ = '%';
	    *p2++ = XNUM_TO_DIGIT (c >> 4);
	    *p2++ = XNUM_TO_DIGIT (c & 0xf);
	}
	else
	    *p2++ = *p1++;

    *p2 = '\0';
    assert (p2 - newstr == newlen);
    return newstr;
}

/* Returns the scheme type if the scheme is supported, or
   SCHEME_INVALID if not.  */

enum url_scheme url_scheme (const char *url)
{
    int i;

    for (i = 0; supported_schemes[i].leading_string; i++)
	if (0 == strncasecmp (url, supported_schemes[i].leading_string,
		    strlen (supported_schemes[i].leading_string)))
	{
	    if (supported_schemes[i].enabled)
		return (enum url_scheme) i;
	    else
		return SCHEME_INVALID;
	}

    return SCHEME_INVALID;
}

/* Return 1 if the URL begins with any "scheme", 0 otherwise.  As
   currently implemented, it returns true if URL begins with
   [-+a-zA-Z0-9]+: .  */

int url_has_scheme (const char *url)
{
    const char *p = url;

    /* The first char must be a scheme char. */
    if (!*p || !SCHEME_CHAR (*p))
	return 0;
    ++p;
    /* Followed by 0 or more scheme chars. */
    while (*p && SCHEME_CHAR (*p))
	++p;
    /* Terminated by ':'. */
    return *p == ':';
}

int scheme_default_port (enum url_scheme scheme)
{
    return supported_schemes[scheme].default_port;
}

void scheme_disable (enum url_scheme scheme)
{
    supported_schemes[scheme].enabled = 0;
}

/* Skip the username and password, if present in the URL.  The
   function should *not* be called with the complete URL, but with the
   portion after the scheme.

   If no username and password are found, return URL.  */
/* char *strpbrk(const char *s, const char *accept);
   DESCRIPTION
   The  strpbrk() function locates the first occurrence in the string s of any of the characters 
   in the string accept.
   RETURN VALUE
   The strpbrk() function returns a pointer to the character in s that matches one of the  
   characters  in  accept, or, NULL if no such character is found.
 */

static const char * url_skip_credentials (const char *url)
{
    /* Look for '@' that comes before terminators, such as '/', '?',
       '#', or ';'.  */
    const char *p = (const char *)strpbrk (url, "@/?#;");
    if (!p || *p != '@')
	return url;
    return p + 1;
}

/* Parse credentials contained in [BEG, END).  The region is expected
   to have come from a URL and is unescaped.  */

static int parse_credentials (const char *beg, const char *end, char **user, char **passwd)
{
    char *colon;
    const char *userend;

    if (beg == end)
	return 0;			/* empty user name */

    colon = memchr (beg, ':', end - beg);
    if (colon == beg)
	return 0;			/* again empty user name */

    if (colon)
    {
	*passwd = strdupdelim (colon + 1, end);
	userend = colon;
	url_unescape (*passwd);
    }
    else
    {
	*passwd = NULL;
	userend = end;
    }
    *user = strdupdelim (beg, userend);
    url_unescape (*user);
    return 1;
}

/* Used by main.c: detect URLs written using the "shorthand" URL forms
   popularized by Netscape and NcFTP.  HTTP shorthands look like this:

   www.foo.com[:port]/dir/file   -> http://www.foo.com[:port]/dir/file
   www.foo.com[:port]            -> http://www.foo.com[:port]

   FTP shorthands look like this:

   foo.bar.com:dir/file          -> ftp://foo.bar.com/dir/file
   foo.bar.com:/absdir/file      -> ftp://foo.bar.com//absdir/file

   If the URL needs not or cannot be rewritten, return NULL.  */

char *rewrite_shorthand_url (const char *url)
{
    const char *p;

    if (url_scheme (url) != SCHEME_INVALID)
	return NULL;

    /* Look for a ':' or '/'.  The former signifies NcFTP syntax, the
       latter Netscape.  */
    for (p = url; *p && *p != ':' && *p != '/'; p++)
	;

    if (p == url)
	return NULL;

    /* If we're looking at "://", it means the URL uses a scheme we
       don't support, which may include "https" when compiled without
       SSL support.  Don't bogusly rewrite such URLs.  */
    if (p[0] == ':' && p[1] == '/' && p[2] == '/')
	return NULL;

    if (*p == ':')
    {
	const char *pp;
	char *res;
	/* If the characters after the colon and before the next slash
	   or end of string are all digits, it's HTTP.  */
	int digits = 0;
	for (pp = p + 1; ISDIGIT (*pp); pp++)
	    ++digits;
	if (digits > 0 && (*pp == '/' || *pp == '\0'))
	    goto http;

	/* Prepend "ftp://" to the entire URL... */
	res = xmalloc (6 + strlen (url) + 1);
	sprintf (res, "ftp://%s", url);
	/* ...and replace ':' with '/'. */
	res[6 + (p - url)] = '/';
	return res;
    }
    else
    {
	char *res;
http:
	/* Just prepend "http://" to what we have. */
	res = xmalloc (7 + strlen (url) + 1);
	sprintf (res, "http://%s", url);
	return res;
    }
}

/* Parse a URL.

   Return a new struct url if successful, NULL on error.  In case of
   error, and if ERROR is not NULL, also set *ERROR to the appropriate
   error code. */
struct url * url_parse (const char *url, int *error)
{
    struct url *u;
    const char *p;
    int path_modified, host_modified;

    enum url_scheme scheme;

    const char *uname_b,     *uname_e;
    const char *host_b,      *host_e;
    const char *path_b,      *path_e;
    const char *params_b,    *params_e;
    const char *query_b,     *query_e;
    const char *fragment_b,  *fragment_e;

    int port;
    char *user = NULL, *passwd = NULL;

    char *url_encoded = NULL;

    int error_code;

    scheme = url_scheme (url);
    if (scheme == SCHEME_INVALID)
    {
	error_code = PE_UNSUPPORTED_SCHEME;
	goto err;
    }

    url_encoded = reencode_escapes (url);
    p = url_encoded;

    p += strlen (supported_schemes[scheme].leading_string);
    uname_b = p;
    p = url_skip_credentials (p);
    uname_e = p;

    /* scheme://user:pass@host[:port]... */
    /*                    ^              */

    /* We attempt to break down the URL into the components path,
       params, query, and fragment.  They are ordered like this:

scheme://host[:port][/path][;params][?query][#fragment]  */

    params_b   = params_e   = NULL;
    query_b    = query_e    = NULL;
    fragment_b = fragment_e = NULL;

    host_b = p;

    if (*p == '[')
    {
	/* Handle IPv6 address inside square brackets.  Ideally we'd
	   just look for the terminating ']', but rfc2732 mandates
	   rejecting invalid IPv6 addresses.  */

	/* The address begins after '['. */
	host_b = p + 1;
	host_e = strchr (host_b, ']');

	if (!host_e)
	{
	    error_code = PE_UNTERMINATED_IPV6_ADDRESS;
	    goto err;
	}

#ifdef ENABLE_IPV6
	/* Check if the IPv6 address is valid. */
	if (!is_valid_ipv6_address(host_b, host_e))
	{
	    error_code = PE_INVALID_IPV6_ADDRESS;
	    goto err;
	}

	/* Continue parsing after the closing ']'. */
	p = host_e + 1;
#else
	error_code = PE_IPV6_NOT_SUPPORTED;
	goto err;
#endif

	/* The closing bracket must be followed by a separator or by the
	   null char.  */
	/* http://[::1]... */
	/*             ^   */
	if (!strchr (":/;?#", *p))
	{
	    /* Trailing garbage after []-delimited IPv6 address. */
	    error_code = PE_INVALID_HOST_NAME;
	    goto err;
	}
    }
    else
    {
	p = strpbrk_or_eos (p, ":/;?#");
	host_e = p;
    }

    if (host_b == host_e)
    {
	error_code = PE_INVALID_HOST_NAME;
	goto err;
    }

    port = scheme_default_port (scheme);
    if (*p == ':')
    {
	const char *port_b, *port_e, *pp;

	/* scheme://host:port/tralala */
	/*              ^             */
	++p;
	port_b = p;
	p = strpbrk_or_eos (p, "/;?#");
	port_e = p;

	/* Allow empty port, as per rfc2396. */
	if (port_b != port_e)
	{
	    for (port = 0, pp = port_b; pp < port_e; pp++)
	    {
		if (!ISDIGIT (*pp))
		{
		    /* http://host:12randomgarbage/blah */
		    /*               ^                  */
		    error_code = PE_BAD_PORT_NUMBER;
		    goto err;
		}
		port = 10 * port + (*pp - '0');
		/* Check for too large port numbers here, before we have
		   a chance to overflow on bogus port values.  */
		if (port > 65535)
		{
		    error_code = PE_BAD_PORT_NUMBER;
		    goto err;
		}
	    }
	}
    }

    if (*p == '/')
    {
	++p;
	path_b = p;
	p = strpbrk_or_eos (p, ";?#");
	path_e = p;
    }
    else
    {
	/* Path is not allowed not to exist. */
	path_b = path_e = p;
    }

    if (*p == ';')
    {
	++p;
	params_b = p;
	p = strpbrk_or_eos (p, "?#");
	params_e = p;
    }
    if (*p == '?')
    {
	++p;
	query_b = p;
	p = strpbrk_or_eos (p, "#");
	query_e = p;

	/* Hack that allows users to use '?' (a wildcard character) in
	   FTP URLs without it being interpreted as a query string
	   delimiter.  */
	if (scheme == SCHEME_FTP)
	{
	    query_b = query_e = NULL;
	    path_e = p;
	}
    }
    if (*p == '#')
    {
	++p;
	fragment_b = p;
	p += strlen (p);
	fragment_e = p;
    }
    assert (*p == 0);

    if (uname_b != uname_e)
    {
	/* http://user:pass@host */
	/*        ^         ^    */
	/*     uname_b   uname_e */
	if (!parse_credentials (uname_b, uname_e - 1, &user, &passwd))
	{
	    error_code = PE_INVALID_USER_NAME;
	    goto err;
	}
    }

    u = xnew0 (struct url);
    u->scheme = scheme;
    u->host   = strdupdelim (host_b, host_e);
    u->port   = port;
    u->user   = user;
    u->passwd = passwd;

    u->path = strdupdelim (path_b, path_e);
    path_modified = path_simplify (u->path);
    split_path (u->path, &u->dir, &u->file);

    host_modified = lowercase_str (u->host);

    /* Decode %HH sequences in host name.  This is important not so much
       to support %HH sequences in host names (which other browser
       don't), but to support binary characters (which will have been
       converted to %HH by reencode_escapes).  */
    if (strchr (u->host, '%'))
    {
	url_unescape (u->host);
	host_modified = 1;
    }

    if (params_b)
	u->params = strdupdelim (params_b, params_e);
    if (query_b)
	u->query = strdupdelim (query_b, query_e);
    if (fragment_b)
	u->fragment = strdupdelim (fragment_b, fragment_e);

    if (path_modified || u->fragment || host_modified || path_b == path_e)
    {
	/* If we suspect that a transformation has rendered what
	   url_string might return different from URL_ENCODED, rebuild
	   u->url using url_string.  */
	u->url = url_string (u, 0);

	if (url_encoded != url)
	    xfree ((char *) url_encoded);
    }
    else
    {
	if (url_encoded == url)
	    u->url = xstrdup (url);
	else
	    u->url = url_encoded;
    }

    return u;

err:
    /* Cleanup in case of error: */
    if (url_encoded && url_encoded != url)
	xfree (url_encoded);

    /* Transmit the error code to the caller, if the caller wants to
       know.  */
    if (error)
	*error = error_code;
    return NULL;
}


/* Return the error message string from ERROR_CODE, which should have
   been retrieved from url_parse.  The error message is translated.  */

const char * url_error (int error_code)
{
    assert (error_code >= 0 && error_code < countof (parse_errors));
    return parse_errors[error_code];
}

/* Split PATH into DIR and FILE.  PATH comes from the URL and is
   expected to be URL-escaped.

   The path is split into directory (the part up to the last slash)
   and file (the part after the last slash), which are subsequently
   unescaped.  Examples:

   PATH                 DIR           FILE
   "foo/bar/baz"        "foo/bar"     "baz"
   "foo/bar/"           "foo/bar"     ""
   "foo"                ""            "foo"
   "foo/bar/baz%2fqux"  "foo/bar"     "baz/qux" (!)

   DIR and FILE are freshly allocated.  */

static void split_path (const char *path, char **dir, char **file)
{
    char *last_slash = strrchr (path, '/');
    if (!last_slash)
    {
	*dir = xstrdup ("");
	*file = xstrdup (path);
    }
    else
    {
	*dir = strdupdelim (path, last_slash);
	*file = xstrdup (last_slash + 1);
    }
    url_unescape (*dir);
    url_unescape (*file);
}

/* Note: URL's "full path" is the path with the query string and
   params appended.  The "fragment" (#foo) is intentionally ignored,
   but that might be changed.  For example, if the original URL was
   "http://host:port/foo/bar/baz;bullshit?querystring#uselessfragment",
   the full path will be "/foo/bar/baz;bullshit?querystring".  */

/* Return the length of the full path, without the terminating
   zero.  */

static int full_path_length (const struct url *url)
{
    int len = 0;

#define FROB(el) if (url->el) len += 1 + strlen (url->el)

    FROB (path);
    FROB (params);
    FROB (query);

#undef FROB

    return len;
}

/* Write out the full path. */

static void full_path_write (const struct url *url, char *where)
{
#define FROB(el, chr) do {			\
    char *f_el = url->el;				\
    if (f_el) {					\
	int l = strlen (f_el);			\
	*where++ = chr;				\
	memcpy (where, f_el, l);			\
	where += l;					\
    }						\
} while (0)

FROB (path, '/');
FROB (params, ';');
FROB (query, '?');

#undef FROB
}

/* Public function for getting the "full path".  E.g. if u->path is
   "foo/bar" and u->query is "param=value", full_path will be
   "/foo/bar?param=value". */

char * url_full_path (const struct url *url)
{
    int length = full_path_length (url);
    char *full_path = (char *) xmalloc (length + 1);

    full_path_write (url, full_path);
    full_path[length] = '\0';

    return full_path;
}

/* Unescape CHR in an otherwise escaped STR.  Used to selectively
   escaping of certain characters, such as "/" and ":".  Returns a
   count of unescaped chars.  */

static void unescape_single_char (char *str, char chr)
{
    const char c1 = XNUM_TO_DIGIT (chr >> 4);
    const char c2 = XNUM_TO_DIGIT (chr & 0xf);
    char *h = str;		/* hare */
    char *t = str;		/* tortoise */
    for (; *h; h++, t++)
    {
	if (h[0] == '%' && h[1] == c1 && h[2] == c2)
	{
	    *t = chr;
	    h += 2;
	}
	else
	    *t = *h;
    }
    *t = '\0';
}

/* Escape unsafe and reserved characters, except for the slash
   characters.  */

static char * url_escape_dir (const char *dir)
{
    char *newdir = url_escape_1 (dir, urlchr_unsafe | urlchr_reserved, 1);
    if (newdir == dir)
	return (char *)dir;

    unescape_single_char (newdir, '/');
    return newdir;
}

/* Sync u->path and u->url with u->dir and u->file.  Called after
   u->file or u->dir have been changed, typically by the FTP code.  */

static void sync_path (struct url *u)
{
    char *newpath, *efile, *edir;

    xfree (u->path);

    /* u->dir and u->file are not escaped.  URL-escape them before
       reassembling them into u->path.  That way, if they contain
       separators like '?' or even if u->file contains slashes, the
       path will be correctly assembled.  (u->file can contain slashes
       if the URL specifies it with %2f, or if an FTP server returns
       it.)  */
    edir = url_escape_dir (u->dir);
    efile = url_escape_1 (u->file, urlchr_unsafe | urlchr_reserved, 1);

    if (!*edir)
	newpath = xstrdup (efile);
    else
    {
	int dirlen = strlen (edir);
	int filelen = strlen (efile);

	/* Copy "DIR/FILE" to newpath. */
	char *p = newpath = xmalloc (dirlen + 1 + filelen + 1);
	memcpy (p, edir, dirlen);
	p += dirlen;
	*p++ = '/';
	memcpy (p, efile, filelen);
	p += filelen;
	*p = '\0';
    }

    u->path = newpath;

    if (edir != u->dir)
	xfree (edir);
    if (efile != u->file)
	xfree (efile);

    /* Regenerate u->url as well.  */
    xfree (u->url);
    u->url = url_string (u, 0);
}

/* Mutators.  Code in ftp.c insists on changing u->dir and u->file.
   This way we can sync u->path and u->url when they get changed.  */

void url_set_dir (struct url *url, const char *newdir)
{
    xfree (url->dir);
    url->dir = xstrdup (newdir);
    sync_path (url);
}

void url_set_file (struct url *url, const char *newfile)
{
    xfree (url->file);
    url->file = xstrdup (newfile);
    sync_path (url);
}

void url_free (struct url *url)
{
    xfree (url->host);
    xfree (url->path);
    xfree (url->url);

    xfree_null (url->params);
    xfree_null (url->query);
    xfree_null (url->fragment);
    xfree_null (url->user);
    xfree_null (url->passwd);

    xfree (url->dir);
    xfree (url->file);

    xfree (url);
}

/* Resolve "." and ".." elements of PATH by destructively modifying
   PATH and return non-zero if PATH has been modified, zero otherwise.

   The algorithm is in spirit similar to the one described in rfc1808,
   implemented differently, in one pass.  To recap, path
   elements containing only "." are removed, and ".." is taken to mean
   "back up one element".  Single leading and trailing slashes are

   For example, "a/b/c/./../d/.." will yield "a/b/".  More exhaustive
   test examples are provided below.  If you change anything in this
   function, run test_path_simplify to make sure you haven't broken a
   test case.  */

static int path_simplify (char *path)
{
    char *h = path;		/* hare */
    char *t = path;		/* tortoise */
    char *beg = path;		/* boundary for backing the tortoise */
    char *end = path + strlen (path);

    while (h < end)
    {
	/* Hare should be at the beginning of a path element. */

	if (h[0] == '.' && (h[1] == '/' || h[1] == '\0'))
	{
	    /* Ignore "./". */
	    h += 2;
	}
	else if (h[0] == '.' && h[1] == '.' && (h[2] == '/' || h[2] == '\0'))
	{
	    /* Handle "../" by retreating the tortoise by one path
	       element -- but not past beggining.  */
	    if (t > beg)
	    {
		/* Move backwards until T hits the beginning of the
		   previous path element or the beginning of path. */
		for (--t; t > beg && t[-1] != '/'; t--)
		    ;
	    }
	    else
	    {
		/* If we're at the beginning, copy the "../" literally
		   move the beginning so a later ".." doesn't remove
		   it.  */
		beg = t + 3;
		goto regular;
	    }
	    h += 3;
	}
	else
	{
regular:
	    /* A regular path element.  If H hasn't advanced past T,
	       simply skip to the next path element.  Otherwise, copy
	       the path element until the next slash.  */
	    if (t == h)
	    {
		/* Skip the path element, including the slash.  */
		while (h < end && *h != '/')
		    t++, h++;
		if (h < end)
		    t++, h++;
	    }
	    else
	    {
		/* Copy the path element, including the final slash.  */
		while (h < end && *h != '/')
		    *t++ = *h++;
		if (h < end)
		    *t++ = *h++;
	    }
	}
    }

    if (t != h)
	*t = '\0';

    return t != h;
}

/* Return the length of URL's path.  Path is considered to be
   terminated by one of '?', ';', '#', or by the end of the
   string.  */

static int path_length (const char *url)
{
    const char *q = strpbrk_or_eos (url, "?;#");
    return q - url;
}

/* Find the last occurrence of character C in the range [b, e), or
   NULL, if none are present.  We might want to use memrchr (a GNU
   extension) under GNU libc.  */

static const char * find_last_char (const char *b, const char *e, char c)
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
   url_parse has to simplify path anyway, so it's wasteful to boot.  */

char * uri_merge (const char *base, const char *link)
{
    int linklength;
    const char *end;
    char *merge;

    if (url_has_scheme (link))
	return xstrdup (link);

    /* We may not examine BASE past END. */
    end = base + path_length (base);
    linklength = strlen (link);

    if (!*link)
    {
	/* Empty LINK points back to BASE, query string and all. */
	return xstrdup (base);
    }
    else if (*link == '?')
    {
	/* LINK points to the same location, but changes the query
	   string.  Examples: */
	/* uri_merge("path",         "?new") -> "path?new"     */
	/* uri_merge("path?foo",     "?new") -> "path?new"     */
	/* uri_merge("path?foo#bar", "?new") -> "path?new"     */
	/* uri_merge("path#foo",     "?new") -> "path?new"     */
	int baselength = end - base;
	merge = xmalloc (baselength + linklength + 1);
	memcpy (merge, base, baselength);
	memcpy (merge + baselength, link, linklength);
	merge[baselength + linklength] = '\0';
    }
    else if (*link == '#')
    {
	/* uri_merge("path",         "#new") -> "path#new"     */
	/* uri_merge("path#foo",     "#new") -> "path#new"     */
	/* uri_merge("path?foo",     "#new") -> "path?foo#new" */
	/* uri_merge("path?foo#bar", "#new") -> "path?foo#new" */
	int baselength;
	const char *end1 = strchr (base, '#');
	if (!end1)
	    end1 = base + strlen (base);
	baselength = end1 - base;
	merge = xmalloc (baselength + linklength + 1);
	memcpy (merge, base, baselength);
	memcpy (merge + baselength, link, linklength);
	merge[baselength + linklength] = '\0';
    }
    else if (*link == '/' && *(link + 1) == '/')
    {
	/* LINK begins with "//" and so is a net path: we need to
	   replace everything after (and including) the double slash
	   with LINK. */

	/* uri_merge("foo", "//new/bar")            -> "//new/bar"      */
	/* uri_merge("//old/foo", "//new/bar")      -> "//new/bar"      */
	/* uri_merge("http://old/foo", "//new/bar") -> "http://new/bar" */

	int span;
	const char *slash;
	const char *start_insert;

	/* Look for first slash. */
	slash = memchr (base, '/', end - base);
	/* If found slash and it is a double slash, then replace
	   from this point, else default to replacing from the
	   beginning.  */
	if (slash && *(slash + 1) == '/')
	    start_insert = slash;
	else
	    start_insert = base;

	span = start_insert - base;
	merge = (char *)xmalloc (span + linklength + 1);
	if (span)
	    memcpy (merge, base, span);
	memcpy (merge + span, link, linklength);
	merge[span + linklength] = '\0';
    }
    else if (*link == '/')
    {
	/* LINK is an absolute path: we need to replace everything
	   after (and including) the FIRST slash with LINK.

	   So, if BASE is "http://host/whatever/foo/bar", and LINK is
	   "/qux/xyzzy", our result should be
	   "http://host/qux/xyzzy".  */
	int span;
	const char *slash;
	const char *start_insert = NULL; /* for gcc to shut up. */
	const char *pos = base;
	int seen_slash_slash = 0;
	/* We're looking for the first slash, but want to ignore
	   double slash. */
again:
	slash = memchr (pos, '/', end - pos);
	if (slash && !seen_slash_slash)
	    if (*(slash + 1) == '/')
	    {
		pos = slash + 2;
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

	span = start_insert - base;
	merge = (char *)xmalloc (span + linklength + 1);
	if (span)
	    memcpy (merge, base, span);
	memcpy (merge + span, link, linklength);
	merge[span + linklength] = '\0';
    }
    else
    {
	/* LINK is a relative URL: we need to replace everything
	   after last slash (possibly empty) with LINK.

	   So, if BASE is "whatever/foo/bar", and LINK is "qux/xyzzy",
	   our result should be "whatever/foo/qux/xyzzy".  */
	int need_explicit_slash = 0;
	int span;
	const char *start_insert;
	const char *last_slash = find_last_char (base, end, '/');
	if (!last_slash)
	{
	    /* No slash found at all.  Replace what we have with LINK. */
	    start_insert = base;
	}
	else if (last_slash && last_slash >= base + 2
		&& last_slash[-2] == ':' && last_slash[-1] == '/')
	{
	    /* example: http://host"  */
	    /*                      ^ */
	    start_insert = end + 1;
	    need_explicit_slash = 1;
	}
	else
	{
	    /* example: "whatever/foo/bar" */
	    /*                        ^    */
	    start_insert = last_slash + 1;
	}

	span = start_insert - base;
	merge = (char *)xmalloc (span + linklength + 1);
	if (span)
	    memcpy (merge, base, span);
	if (need_explicit_slash)
	    merge[span - 1] = '/';
	memcpy (merge + span, link, linklength);
	merge[span + linklength] = '\0';
    }

    return merge;
}

#define APPEND(p, s) do {			\
    int len = strlen (s);				\
    memcpy (p, s, len);				\
    p += len;					\
} while (0)

/* Use this instead of password when the actual password is supposed
   to be hidden.  We intentionally use a generic string without giving
   away the number of characters in the password, like previous
   versions did.  */
#define HIDDEN_PASSWORD "*password*"

/* Recreate the URL string from the data in URL.

   If HIDE is non-zero (as it is when we're calling this on a URL we
   plan to print, but not when calling it to canonicalize a URL for
   use within the program), password will be hidden.  Unsafe
   characters in the URL will be quoted.  */

char * url_string (const struct url *url, int hide_password)
{
    int size;
    char *result, *p;
    char *quoted_host, *quoted_user = NULL, *quoted_passwd = NULL;

    int scheme_port  = supported_schemes[url->scheme].default_port;
    const char *scheme_str = supported_schemes[url->scheme].leading_string;
    int fplen = full_path_length (url);

    int brackets_around_host;

    assert (scheme_str != NULL);

    /* Make sure the user name and password are quoted. */
    if (url->user)
    {
	quoted_user = url_escape_allow_passthrough (url->user);
	if (url->passwd)
	{
	    if (hide_password)
		quoted_passwd = HIDDEN_PASSWORD;
	    else
		quoted_passwd = url_escape_allow_passthrough (url->passwd);
	}
    }

    /* In the unlikely event that the host name contains non-printable
       characters, quote it for displaying to the user.  */
    quoted_host = url_escape_allow_passthrough (url->host);

    /* Undo the quoting of colons that URL escaping performs.  IPv6
       addresses may legally contain colons, and in that case must be
       placed in square brackets.  */
    if (quoted_host != url->host)
	unescape_single_char (quoted_host, ':');
    brackets_around_host = strchr (quoted_host, ':') != NULL;

    size = (strlen (scheme_str)
	    + strlen (quoted_host)
	    + (brackets_around_host ? 2 : 0)
	    + fplen
	    + 1);
    if (url->port != scheme_port)
	size += 1 + numdigit (url->port);
    if (quoted_user)
    {
	size += 1 + strlen (quoted_user);
	if (quoted_passwd)
	    size += 1 + strlen (quoted_passwd);
    }

    p = result = (char *)xmalloc (size);

    APPEND (p, scheme_str);
    if (quoted_user)
    {
	APPEND (p, quoted_user);
	if (quoted_passwd)
	{
	    *p++ = ':';
	    APPEND (p, quoted_passwd);
	}
	*p++ = '@';
    }

    if (brackets_around_host)
	*p++ = '[';
    APPEND (p, quoted_host);
    if (brackets_around_host)
	*p++ = ']';
    if (url->port != scheme_port)
    {
	*p++ = ':';
	p = number_to_string (p, url->port);
    }

    full_path_write (url, p);
    p += fplen;
    *p++ = '\0';

    assert (p - result == size);

    if (quoted_user && quoted_user != url->user)
	xfree (quoted_user);
    if (quoted_passwd && !hide_password && quoted_passwd != url->passwd)
	xfree (quoted_passwd);
    if (quoted_host != url->host)
	xfree (quoted_host);

    return result;
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

/* Like strpbrk, with the exception that it returns the pointer to the
   terminating zero (end-of-string aka "eos") if no matching character
   is found.

   Although I normally balk at Gcc-specific optimizations, it probably
   makes sense here: glibc has optimizations that detect strpbrk being
   called with literal string as ACCEPT and inline the search.  That
   optimization is defeated if strpbrk is hidden within the call to
   another function.  (And no, making strpbrk_or_eos inline doesn't
   help because the check for literal accept is in the
   preprocessor.)  */
/* 类似于strpbrk，不同的是当在s中没有找到accept中的字符时，返回指向s的'\0'的指针。*/
char *strpbrk_or_eos (const char *s, const char *accept)
{
    char *p = strpbrk (s, accept);
    if (!p)
	p = strchr (s, '\0');
    return p;
}

/* Turn STR into lowercase; return non-zero if a character was
   actually changed. */
int lowercase_str (char *str)
{
    int change = 0;
    for (; *str; str++)
	if (isupper (*str))
	{
	    change = 1;
	    *str = tolower (*str);
	}
    return change;
}

#ifdef TEST_URLPARSE
int main()
{
  int ecode;
  struct url *url;
  char path[1024];

  url = url_parse("http://www.hh.com",&ecode);
  url = url_parse("http://www.huihao.com/",&ecode);
  url = url_parse("http://www.huihao.com/dir1/dir2/",&ecode);
  url = url_parse("http://www.huihao.com/dir/file",&ecode);
  url = url_parse("http://www.huihao.com/./../../../uploadfile/UpPicmore/2.jpg",&ecode);
  strcpy(path,"./../../../uploadfile/UpPicmore/2.jpg");
  path_simplify(path);
  url = url_parse("http://www.gekang.COM:88/path/p 2/file.exe?word=中国 人民&page=1 %2f",&ecode);
}
#endif
