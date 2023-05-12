/* Collect URLs from HTML source. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "htmlparse.h"
#include "urlparse.h"
#include "fsfhash.h"
#include "htmlurl.h"
#include "xmalloc.h"

/* The number of elements in an array.  For example:
   static char a[] = "foo";     -- countof(a) == 4 (note terminating \0)
   int a[5] = {1, 2};           -- countof(a) == 5
   char *a[] = {                -- countof(a) == 3
   "foo", "bar", "baz"
   }; */
#define countof(array) (sizeof (array) / sizeof ((array)[0]))

# define ISSPACE(x) isspace (x)
# define ISDIGIT(x) isdigit (x)
# define ISXDIGIT(x) isxdigit (x)
# define ISALPHA(x) isalpha (x)
# define ISALNUM(x) isalnum (x)
# define TOLOWER(x) tolower (x)
# define TOUPPER(x) toupper (x)


struct map_context;

typedef void (*tag_handler_t) (int, struct taginfo *,struct map_context *);

#define DECLARE_TAG_HANDLER(fun)					\
static void fun (int, struct taginfo *, struct map_context *)

    DECLARE_TAG_HANDLER (tag_find_urls);
    DECLARE_TAG_HANDLER (tag_handle_base);
    DECLARE_TAG_HANDLER (tag_handle_form);
    DECLARE_TAG_HANDLER (tag_handle_link);
    DECLARE_TAG_HANDLER (tag_handle_meta);

    enum {
	TAG_A,
	TAG_APPLET,
	TAG_AREA,
	TAG_BASE,
	TAG_BGSOUND,
	TAG_BODY,
	TAG_EMBED,
	TAG_FIG,
	TAG_FORM,
	TAG_FRAME,
	TAG_IFRAME,
	TAG_IMG,
	TAG_INPUT,
	TAG_LAYER,
	TAG_LINK,
	TAG_META,
	TAG_OBJECT,
	TAG_OVERLAY,
	TAG_SCRIPT,
	TAG_TABLE,
	TAG_TD,
	TAG_TH
    };

/* The list of known tags and functions used for handling them.  Most
   tags are simply harvested for URLs. */
static struct known_tag {
    int tagid;
    const char *name;
    tag_handler_t handler;
} known_tags[] = {
    { TAG_A,	 "a",		tag_find_urls },
    { TAG_APPLET,	 "applet",	tag_find_urls },
    { TAG_AREA,	 "area",	tag_find_urls },
    { TAG_BASE,	 "base",	tag_handle_base },
    { TAG_BGSOUND, "bgsound",	tag_find_urls },
    { TAG_BODY,	 "body",	tag_find_urls },
    { TAG_EMBED,	 "embed",	tag_find_urls },
    { TAG_FIG,	 "fig",		tag_find_urls },
    { TAG_FORM,	 "form",	tag_handle_form },
    { TAG_FRAME,	 "frame",	tag_find_urls },
    { TAG_IFRAME,	 "iframe",	tag_find_urls },
    { TAG_IMG,	 "img",		tag_find_urls },
    { TAG_INPUT,	 "input",	tag_find_urls },
    { TAG_LAYER,	 "layer",	tag_find_urls },
    { TAG_LINK,	 "link",	tag_handle_link },
    { TAG_META,	 "meta",	tag_handle_meta },
    { TAG_OBJECT,  "object",	tag_find_urls },
    { TAG_OVERLAY, "overlay",	tag_find_urls },
    { TAG_SCRIPT,	 "script",	tag_find_urls },
    { TAG_TABLE,	 "table",	tag_find_urls },
    { TAG_TD,	 "td",		tag_find_urls },
    { TAG_TH,	 "th",		tag_find_urls }
};

/* tag_url_attributes documents which attributes of which tags contain
   URLs to harvest.  It is used by tag_find_urls.  */

/* Defines for the FLAGS. */

/* The link is "inline", i.e. needs to be retrieved for this document
   to be correctly rendered.  Inline links include inlined images,
   stylesheets, children frames, etc.  */
#define ATTR_INLINE	1

/* The link is expected to yield HTML contents.  It's important not to
   try to follow HTML obtained by following e.g. <img src="...">
   regardless of content-type.  Doing this causes infinite loops for
   "images" that return non-404 error pages with links to the same
   image.  */
#define ATTR_HTML	2

/* For tags handled by tag_find_urls: attributes that contain URLs to
   download. */
static struct {
    int tagid;
    const char *attr_name;
    int flags;
} tag_url_attributes[] = {
    { TAG_A,		"href",		ATTR_HTML },
    { TAG_APPLET,		"code",		ATTR_INLINE },
    { TAG_AREA,		"href",		ATTR_HTML },
    { TAG_BGSOUND,	"src",		ATTR_INLINE },
    { TAG_BODY,		"background",	ATTR_INLINE },
    { TAG_EMBED,		"href",		ATTR_HTML },
    { TAG_EMBED,		"src",		ATTR_INLINE | ATTR_HTML },
    { TAG_FIG,		"src",		ATTR_INLINE },
    { TAG_FRAME,		"src",		ATTR_INLINE | ATTR_HTML },
    { TAG_IFRAME,		"src",		ATTR_INLINE | ATTR_HTML },
    { TAG_IMG,		"href",		ATTR_INLINE },
    { TAG_IMG,		"lowsrc",	ATTR_INLINE },
    { TAG_IMG,		"src",		ATTR_INLINE },
    { TAG_INPUT,		"src",		ATTR_INLINE },
    { TAG_LAYER,		"src",		ATTR_INLINE | ATTR_HTML },
    { TAG_OBJECT,		"data",		ATTR_INLINE },
    { TAG_OVERLAY,	"src",		ATTR_INLINE | ATTR_HTML },
    { TAG_SCRIPT,		"src",		ATTR_INLINE },
    { TAG_TABLE,		"background",	ATTR_INLINE },
    { TAG_TD,		"background",	ATTR_INLINE },
    { TAG_TH,		"background",	ATTR_INLINE }
};

/* The lists of interesting tags and attributes are built dynamically,
   from the information above.  However, some places in the code refer
   to the attributes not mentioned here.  We add them manually.  */
static const char *additional_attributes[] = {
    "rel",			/* used by tag_handle_link */
    "http-equiv",			/* used by tag_handle_meta */
    "name",			/* used by tag_handle_meta */
    "content",			/* used by tag_handle_meta */
    "action"			/* used by tag_handle_form */
};

struct hash_table *interesting_tags;
struct hash_table *interesting_attributes;

    static void
init_interesting (void)
{
    /* Init the variables interesting_tags and interesting_attributes
       that are used by the HTML parser to know which tags and
       attributes we're interested in.  We initialize this only once,
       for performance reasons.

       Here we also make sure that what we put in interesting_tags
       matches the user's preferences as specified through --ignore-tags
       and --follow-tags.  */

    int i;
    interesting_tags = make_nocase_string_hash_table (countof (known_tags));

    /* First, add all the tags we know hot to handle, mapped to their
       respective entries in known_tags.  */
    for (i = 0; i < countof (known_tags); i++)
	hash_table_put (interesting_tags, known_tags[i].name, known_tags + i);

    /* Then remove the tags ignored through --ignore-tags.  */
    /* If --follow-tags is specified, use only those tags.  */

    /* Add the attributes we care about. */
    interesting_attributes = make_nocase_string_hash_table (10);
    for (i = 0; i < countof (additional_attributes); i++)
	hash_table_put (interesting_attributes, additional_attributes[i], "1");
    for (i = 0; i < countof (tag_url_attributes); i++)
	hash_table_put (interesting_attributes,
		tag_url_attributes[i].attr_name, "1");
}

/* Find the value of attribute named NAME in the taginfo TAG.  If the
   attribute is not present, return NULL.  If ATTRIND is non-NULL, the
   index of the attribute in TAG will be stored there.  */

    static char *
find_attr (struct taginfo *tag, const char *name, int *attrind)
{
    int i;
    for (i = 0; i < tag->nattrs; i++)
	if (!strcasecmp (tag->attrs[i].name, name))
	{
	    if (attrind)
		*attrind = i;
	    return tag->attrs[i].value;
	}
    return NULL;
}

struct map_context {
    char *text;			/* HTML text. */
    char *base;			/* Base URI of the document, possibly
				   changed through <base href=...>. */
    const char *parent_base;	/* Base of the current document. */
    const char *document_file;	/* File name of this document. */
    int nofollow;			/* whether NOFOLLOW was specified in a
					   <meta name=robots> tag. */

    struct urlpos *head, *tail;	/* List of URLs that is being
				   built. */
};

/* Append LINK_URI to the urlpos structure that is being built.

   LINK_URI will be merged with the current document base.  TAG and
   ATTRIND are the necessary context to store the position and
   size.  */

    static struct urlpos *
append_url (const char *link_uri,
	struct taginfo *tag, int attrind, struct map_context *ctx)
{
    int link_has_scheme = url_has_scheme (link_uri);
    struct urlpos *newel;
    const char *base = ctx->base ? ctx->base : ctx->parent_base;
    struct url *url;

    if (!base)
    {

	if (!link_has_scheme)
	{
	    /* Base URL is unavailable, and the link does not have a
	       location attached to it -- we have to give up.  Since
	       this can only happen when using `--force-html -i', print
	       a warning.  */
	    return NULL;
	}

	url = url_parse (link_uri, NULL);
	if (!url)
	{
	    return NULL;
	}
    }
    else
    {
	/* Merge BASE with LINK_URI, but also make sure the result is
	   canonicalized, i.e. that "../" have been resolved.
	   (parse_url will do that for us.) */

	char *complete_uri = uri_merge (base, link_uri);

	url = url_parse (complete_uri, NULL);
	if (!url)
	{
	    xfree (complete_uri);
	    return NULL;
	}
	xfree (complete_uri);
    }

    newel = xnew0 (struct urlpos);
    newel->url = url;
    newel->pos = tag->attrs[attrind].value_raw_beginning - ctx->text;
    newel->size = tag->attrs[attrind].value_raw_size;

    /* A URL is relative if the host is not named, and the name does not
       start with `/'.  */
    if (!link_has_scheme && *link_uri != '/')
	newel->link_relative_p = 1;
    else if (link_has_scheme)
	newel->link_complete_p = 1;

    if (ctx->tail)
    {
	ctx->tail->next = newel;
	ctx->tail = newel;
    }
    else
	ctx->tail = ctx->head = newel;

    return newel;
}

/* All the tag_* functions are called from collect_tags_mapper, as
   specified by KNOWN_TAGS.  */

/* Default tag handler: collect URLs from attributes specified for
   this tag by tag_url_attributes.  */

    static void
tag_find_urls (int tagid, struct taginfo *tag, struct map_context *ctx)
{
    int i, attrind;
    int first = -1;

    for (i = 0; i < countof (tag_url_attributes); i++)
	if (tag_url_attributes[i].tagid == tagid)
	{
	    /* We've found the index of tag_url_attributes where the
	       attributes of our tag begin.  */
	    first = i;
	    break;
	}
    assert (first != -1);

    /* Loop over the "interesting" attributes of this tag.  In this
       example, it will loop over "src" and "lowsrc".

       <img src="foo.png" lowsrc="bar.png">

       This has to be done in the outer loop so that the attributes are
       processed in the same order in which they appear in the page.
       This is required when converting links.  */

    for (attrind = 0; attrind < tag->nattrs; attrind++)
    {
	/* Find whether TAG/ATTRIND is a combination that contains a
	   URL. */
	char *link = tag->attrs[attrind].value;
	const int size = countof (tag_url_attributes);

	/* If you're cringing at the inefficiency of the nested loops,
	   remember that they both iterate over a very small number of
	   items.  The worst-case inner loop is for the IMG tag, which
	   has three attributes.  */
	for (i = first; i < size && tag_url_attributes[i].tagid == tagid; i++)
	{
	    if (0 == strcasecmp (tag->attrs[attrind].name,
			tag_url_attributes[i].attr_name))
	    {
		struct urlpos *up = append_url (link, tag, attrind, ctx);
		if (up)
		{
		    int flags = tag_url_attributes[i].flags;
		    if (flags & ATTR_INLINE)
			up->link_inline_p = 1;
		    if (flags & ATTR_HTML)
			up->link_expect_html = 1;
		}
	    }
	}
    }
}

/* Handle the BASE tag, for <base href=...>. */

    static void
tag_handle_base (int tagid, struct taginfo *tag, struct map_context *ctx)
{
    struct urlpos *base_urlpos;
    int attrind;
    char *newbase = find_attr (tag, "href", &attrind);
    if (!newbase)
	return;

    base_urlpos = append_url (newbase, tag, attrind, ctx);
    if (!base_urlpos)
	return;
    base_urlpos->ignore_when_downloading = 1;
    base_urlpos->link_base_p = 1;

    if (ctx->base)
	xfree (ctx->base);
    if (ctx->parent_base)
	ctx->base = uri_merge (ctx->parent_base, newbase);
    else
	ctx->base = xstrdup (newbase);
}

/* Mark the URL found in <form action=...> for conversion. */

    static void
tag_handle_form (int tagid, struct taginfo *tag, struct map_context *ctx)
{
    int attrind;
    char *action = find_attr (tag, "action", &attrind);
    if (action)
    {
	struct urlpos *up = append_url (action, tag, attrind, ctx);
	if (up)
	    up->ignore_when_downloading = 1;
    }
}

/* Handle the LINK tag.  It requires special handling because how its
   links will be followed in -p mode depends on the REL attribute.  */

    static void
tag_handle_link (int tagid, struct taginfo *tag, struct map_context *ctx)
{
    int attrind;
    char *href = find_attr (tag, "href", &attrind);

    /* All <link href="..."> link references are external, except those
       known not to be, such as style sheet and shortcut icon:

       <link rel="stylesheet" href="...">
       <link rel="shortcut icon" href="...">
     */
    if (href)
    {
	struct urlpos *up = append_url (href, tag, attrind, ctx);
	if (up)
	{
	    char *rel = find_attr (tag, "rel", NULL);
	    if (rel
		    && (0 == strcasecmp (rel, "stylesheet")
			|| 0 == strcasecmp (rel, "shortcut icon")))
		up->link_inline_p = 1;
	    else
		/* The external ones usually point to HTML pages, such as
		   <link rel="next" href="..."> */
		up->link_expect_html = 1;
	}
    }
}

/* Handle the META tag.  This requires special handling because of the
   refresh feature and because of robot exclusion.  */

    static void
tag_handle_meta (int tagid, struct taginfo *tag, struct map_context *ctx)
{
    char *name = find_attr (tag, "name", NULL);
    char *http_equiv = find_attr (tag, "http-equiv", NULL);

    if (http_equiv && 0 == strcasecmp (http_equiv, "refresh"))
    {
	/* Some pages use a META tag to specify that the page be
	   refreshed by a new page after a given number of seconds.  The
	   general format for this is:

	   <meta http-equiv=Refresh content="NUMBER; URL=index2.html">

	   So we just need to skip past the "NUMBER; URL=" garbage to
	   get to the URL.  */

	struct urlpos *entry;
	int attrind;
	int timeout = 0;
	char *p;

	char *refresh = find_attr (tag, "content", &attrind);
	if (!refresh)
	    return;

	for (p = refresh; ISDIGIT (*p); p++)
	    timeout = 10 * timeout + *p - '0';
	if (*p++ != ';')
	    return;

	while (ISSPACE (*p))
	    ++p;
	if (!(   TOUPPER (*p)       == 'U'
		    && TOUPPER (*(p + 1)) == 'R'
		    && TOUPPER (*(p + 2)) == 'L'
		    &&          *(p + 3)  == '='))
	    return;
	p += 4;
	while (ISSPACE (*p))
	    ++p;

	entry = append_url (p, tag, attrind, ctx);
	if (entry)
	{
	    entry->link_refresh_p = 1;
	    entry->refresh_timeout = timeout;
	    entry->link_expect_html = 1;
	}
    }
    else if (name && 0 == strcasecmp (name, "robots"))
    {
	/* Handle stuff like:
	   <meta name="robots" content="index,nofollow"> */
	char *content = find_attr (tag, "content", NULL);
	if (!content)
	    return;
	if (!strcasecmp (content, "none"))
	    ctx->nofollow = 1;
	else
	{
	    while (*content)
	    {
		/* Find the next occurrence of ',' or the end of
		   the string.  */
		char *end = strchr (content, ',');
		if (end)
		    ++end;
		else
		    end = content + strlen (content);
		if (!strncasecmp (content, "nofollow", end - content))
		    ctx->nofollow = 1;
		content = end;
	    }
	}
    }
}

/* Dispatch the tag handler appropriate for the tag we're mapping
   over.  See known_tags[] for definition of tag handlers.  */

    static const char *
collect_tags_mapper (struct taginfo *tag, void *arg)
{
    struct map_context *ctx = (struct map_context *)arg;

    /* Find the tag in our table of tags.  This must not fail because
       map_html_tags only returns tags found in interesting_tags.  */
    struct known_tag *t = hash_table_get (interesting_tags, tag->name);
    assert (t != NULL);

    t->handler (t->tagid, tag, ctx);
    return NULL;
}

#define HYPHENP(x) (*(x) == '-' && !*((x) + 1))

/* Read FILE into memory.  A pointer to `struct file_memory' are
   returned; use struct element `content' to access file contents, and
   the element `length' to know the file length.  `content' is *not*
   zero-terminated, and you should *not* read or write beyond the [0,
   length) range of characters.

   After you are done with the file contents, call read_file_free to
   release the memory.

   Depending on the operating system and the type of file that is
   being read, read_file() either mmap's the file into memory, or
   reads the file into the core using read().

   If file is named "-", fileno(stdin) is used for reading instead.
   If you want to read from a real file named "-", use "./-" instead.  */

    struct file_memory *
read_file (const char *file)
{
    int fd;
    struct file_memory *fm;
    long size;
    int inhibit_close = 0;

    /* Some magic in the finest tradition of Perl and its kin: if FILE
       is "-", just use stdin.  */
    if (HYPHENP (file))
    {
	fd = fileno (stdin);
	inhibit_close = 1;
	/* Note that we don't inhibit mmap() in this case.  If stdin is
	   redirected from a regular file, mmap() will still work.  */
    }
    else
	fd = open (file, O_RDONLY);
    if (fd < 0)
	return NULL;
    fm = xnew (struct file_memory);

#ifdef HAVE_MMAP
    {
	struct_fstat buf;
	if (fstat (fd, &buf) < 0)
	    goto mmap_lose;
	fm->length = buf.st_size;
	/* NOTE: As far as I know, the callers of this function never
	   modify the file text.  Relying on this would enable us to
	   specify PROT_READ and MAP_SHARED for a marginal gain in
	   efficiency, but at some cost to generality.  */
	fm->content = mmap (NULL, fm->length, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);
	if (fm->content == (char *)MAP_FAILED)
	    goto mmap_lose;
	if (!inhibit_close)
	    close (fd);

	fm->mmap_p = 1;
	return fm;
    }

mmap_lose:
    /* The most common reason why mmap() fails is that FD does not point
       to a plain file.  However, it's also possible that mmap() doesn't
       work for a particular type of file.  Therefore, whenever mmap()
       fails, we just fall back to the regular method.  */
#endif /* HAVE_MMAP */

    fm->length = 0;
    size = 512;			/* number of bytes fm->contents can
				   hold at any given time. */
    fm->content = xmalloc (size);
    while (1)
    {
	long nread; /* xinz+ */
	if (fm->length > size / 2)
	{
	    /* #### I'm not sure whether the whole exponential-growth
	       thing makes sense with kernel read.  On Linux at least,
	       read() refuses to read more than 4K from a file at a
	       single chunk anyway.  But other Unixes might optimize it
	       better, and it doesn't *hurt* anything, so I'm leaving
	       it.  */

	    /* Normally, we grow SIZE exponentially to make the number
	       of calls to read() and realloc() logarithmic in relation
	       to file size.  However, read() can read an amount of data
	       smaller than requested, and it would be unreasonable to
	       double SIZE every time *something* was read.  Therefore,
	       we double SIZE only when the length exceeds half of the
	       entire allocated size.  */
	    size <<= 1;
	    fm->content = xrealloc (fm->content, size);
	}
	nread = read (fd, fm->content + fm->length, size - fm->length);
	if (nread > 0)
	    /* Successful read. */
	    fm->length += nread;
	else if (nread < 0)
	    /* Error. */
	    goto lose;
	else
	    /* EOF */
	    break;
    }
    if (!inhibit_close)
	close (fd);
    if (size > fm->length && fm->length != 0)
	/* Due to exponential growth of fm->content, the allocated region
	   might be much larger than what is actually needed.  */
	fm->content = xrealloc (fm->content, fm->length);
    fm->mmap_p = 0;
    return fm;

lose:
    if (!inhibit_close)
	close (fd);
    xfree (fm->content);
    xfree (fm);
    return NULL;
}

/* Release the resources held by FM.  Specifically, this calls
   munmap() or xfree() on fm->content, depending whether mmap or
   malloc/read were used to read in the file.  It also frees the
   memory needed to hold the FM structure itself.  */

    void
read_file_free (struct file_memory *fm)
{
#ifdef HAVE_MMAP
    if (fm->mmap_p)
    {
	munmap (fm->content, fm->length);
    }
    else
#endif
    {
	xfree (fm->content);
    }
    xfree (fm);
}


/* Analyze HTML tags FILE and construct a list of URLs referenced from
   it.  It merges relative links in FILE with URL.  It is aware of
   <base href=...> and does the right thing.  */

    struct urlpos *
get_urls_html (const char *file, const char *url, int *meta_disallow_follow)
{
    struct file_memory *fm;
    struct map_context ctx;
    int flags;

    /* Load the file. */
    fm = read_file (file);
    if (!fm)
    {
	return NULL;
    }

    ctx.text = fm->content;
    ctx.head = ctx.tail = NULL;
    ctx.base = NULL;
    ctx.parent_base = url;
    ctx.document_file = file;
    ctx.nofollow = 0;

    if (!interesting_tags)
	init_interesting ();

    /* Specify MHT_TRIM_VALUES because of buggy HTML generators that
       generate <a href=" foo"> instead of <a href="foo"> (browsers
       ignore spaces as well.)  If you really mean space, use &32; or
       %20.  MHT_TRIM_VALUES also causes squashing of embedded newlines,
       e.g. in <img src="foo.[newline]html">.  Such newlines are also
       ignored by IE and Mozilla and are presumably introduced by
       writing HTML with editors that force word wrap.  */
    flags = MHT_TRIM_VALUES;
    /* xinz+
       if (opt.strict_comments)
       flags |= MHT_STRICT_COMMENTS; */

    map_html_tags (fm->content, fm->length, collect_tags_mapper, (void *)&ctx, flags,
	    interesting_tags, interesting_attributes);


    if (meta_disallow_follow)
	*meta_disallow_follow = ctx.nofollow;

    xfree_null (ctx.base);
    read_file_free (fm);
    return ctx.head;
}

    void
cleanup_html_url (void)
{
    /* Destroy the hash tables.  The hash table keys and values are not
       allocated by this code, so we don't need to free them here.  */
    if (interesting_tags)
	hash_table_destroy (interesting_tags);
    if (interesting_attributes)
	hash_table_destroy (interesting_attributes);
}
    
