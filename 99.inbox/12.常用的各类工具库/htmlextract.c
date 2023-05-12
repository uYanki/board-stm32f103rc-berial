/** Copyright (c) 2008 Liu xing zhi. 
    @2010.11.12: added support utf-8.
    @2011.04.10: added reserved interesting tag attributes.
    @2012.09.18: added extract list url.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "comn.h"
#include "utils.h"
#include "htmlparse.h"
#include "htmlextract.h"
#include "fsfhash.h"
#include "md5.h"
#include "xmalloc.h"
#include "charset.h"
#include "xstring.h"
#include "url.h"
#include "crc32.h"

const char * extract_text_mapfun(struct taginfo *taginfo, void *maparg);
const char * extract_interesting_tag_attr_mapfun(struct taginfo *taginfo, void *maparg);

// tag_attr_list不能为空，为空表示所有属性解析过程中都会返回，可以杜撰一个不存在的属性。
void extract_text_init(spec_tag_t *spec_tag_list, int tag_list_size, struct hash_table ** spec_tag_ht_pp,
                       tag_attr_t *tag_attr_list, int attr_list_size, struct hash_table ** tag_attr_ht_pp)
{
  int i;
  struct hash_table * ht;
  tag_attr_t * tagattr;

  ht = make_nocase_string_hash_table(tag_list_size);
  for (i = 0; i < tag_list_size; i++)
    hash_table_put(ht, spec_tag_list[i].tag_name, spec_tag_list + i);
  *spec_tag_ht_pp = ht;

  ht = make_nocase_string_hash_table(attr_list_size);
  for (i = 0; i < attr_list_size; i++) {
     // 先检查属性是否存在...
     tagattr = hash_table_get(ht,tag_attr_list[i].tag_attr);
     if(!tagattr) {
       // 不存在，插入为首节点
       tag_attr_list[i].tail = tag_attr_list + i;
       hash_table_put(ht, tag_attr_list[i].tag_attr, tag_attr_list + i);
     } else {
       // 存在，插入到链表的末尾
       tagattr->tail->next = tag_attr_list + i;
       tagattr->tail = tag_attr_list + i;
     }
   }
  *tag_attr_ht_pp = ht;
}

void extract_text_free(struct hash_table *spec_tag_ht, struct hash_table * tag_attr_ht)
{
  if (spec_tag_ht)
    hash_table_destroy(spec_tag_ht);

  if (tag_attr_ht)
    hash_table_destroy(tag_attr_ht);
}

/*
  pure text, text <tag>,<tag> text
*/
void extract_text(const char *html_content, int content_len, buffer_t *text_buffer_p, const struct hash_table *spec_tag_ht, const struct hash_table *tag_attr_ht, int is_reserve_indent,int codetype,int xssfilter)
{
  int len;
  map_arg_t maparg;
  maparg.html_content_end_position = html_content + content_len;
  maparg.last_tag_end_position = html_content;/* text <tag> */
  maparg.spec_tag_ht = spec_tag_ht;
  maparg.tag_attr_ht = tag_attr_ht;
  maparg.text_buffer_p = text_buffer_p;
  maparg.text_buffer_p->reserve1 = 1;
  maparg.is_reserve_indent = is_reserve_indent;
  maparg.codetype = codetype;
  maparg.xssfilter = xssfilter;

  //注意: 感兴趣标签列表为NULL，表示对所有标签都感兴趣
  map_html_tags(html_content, content_len, extract_text_mapfun, &maparg, MHT_TRIM_VALUES, NULL, tag_attr_ht);

  /* pure text */
  if (maparg.last_tag_end_position == html_content && text_buffer_p->free >= content_len)
  {
    memcpy(text_buffer_p->p + text_buffer_p->pos, html_content, content_len);
    text_buffer_p->free -= content_len;
    text_buffer_p->pos += content_len;
    return;
  }

  /* <tag> text */
  len = maparg.html_content_end_position - maparg.last_tag_end_position;
  if (maparg.last_tag_end_position != maparg.html_content_end_position && text_buffer_p->free >= len)
  {
    memcpy(text_buffer_p->p + text_buffer_p->pos, maparg.last_tag_end_position, len);
    text_buffer_p->free -= len;
    text_buffer_p->pos += len;
  }
  return;
}

/* return NULL -- error,html page parser is end.
   return (taginfo->end_position - 1) -- normal.
   return (skip_close_tag_end_position - 1) -- html parse goto skip_close_tag_end_position - 1.

   arg->text_buffer_p->reserve1 //为1:表示该buffer最后一个字节是空格字符
 */
const char * extract_text_mapfun(struct taginfo *taginfo, void *maparg)
{
  const char *p;
  spec_tag_t *spec_tag = NULL;
  map_arg_t *arg = (map_arg_t *)maparg;

  /* process between last tag and this tag text */
  /*  <last tag>text<this tag> */
  /*            ^   ^          */
  /*        src_beg src_end    */
  {
    const char *src_beg, *src_end;
    char *dst;
    int i, char_cnt;

    if (arg->last_tag_end_position == NULL)
      src_beg = taginfo->start_position;
    else
      src_beg = arg->last_tag_end_position;

    src_end = taginfo->start_position;
    dst = arg->text_buffer_p->p + arg->text_buffer_p->pos;
    char_cnt = src_end - src_beg;

    for (i = 0;i < char_cnt;i++)
    {
      /* whitespace char */
      if (*(src_beg + i) < ' ' && *(src_beg + i) >= 0)
        continue;

      // xssfilter时标签之间的特殊字符需要encode
      if(arg->xssfilter && (*(src_beg + i) == '<' || *(src_beg + i) == '>')) { // &lt; &gt;
        if (arg->text_buffer_p->free <= 5) {
          arg->last_tag_end_position = taginfo->end_position;
          return NULL; /* html page parser is end */
        }
        if(*(src_beg + i) == '<') memcpy(dst,"&lt;",4);
        if(*(src_beg + i) == '>') memcpy(dst,"&gt;",4);
        dst += 4;
        arg->text_buffer_p->reserve1 = 0;
        arg->text_buffer_p->pos += 4;
        arg->text_buffer_p->free -= 4;
        if (arg->text_buffer_p->free <= 1) {
          arg->last_tag_end_position = taginfo->end_position;
          return NULL; /* html page parser is end */
        }
        continue;
      }

      /* space  */
      if (*(src_beg + i) == ' ') {
        if (arg->text_buffer_p->reserve1 == 1)
          continue;
        *dst = ' ';
        arg->text_buffer_p->reserve1 = 1;
        goto dst_add;
      }

      /* decode entities &nbsp; only*/
      if (!arg->xssfilter && *(src_beg + i) == '&' && *(src_beg + i + 1) == 'n' 
        && *(src_beg + i + 2) == 'b' && *(src_beg + i + 3) == 's' 
        && *(src_beg + i + 4) == 'p' && *(src_beg + i + 5) == ';')
      {
        const char *tp = src_beg + i;
        int ret;
        ret = decode_entity(&tp, src_end);
        if (ret != -1)
        {
          if (ret == ' ')
          {
            if (arg->text_buffer_p->reserve1 == 1)
            {
              /* &nbsp; */
              /* ^     ^tp */
              i = tp - src_beg - 1;
              continue;
            }
            *dst = ' ';
            arg->text_buffer_p->reserve1 = 1;
            i = tp - src_beg - 1;
            goto dst_add;
          }

          *dst = ret;
          arg->text_buffer_p->reserve1 = 0;
          i = tp - src_beg - 1;
          goto dst_add;
        }
      }

      if (*((uint8_t*)(src_beg + i)) < 0x80)
      {
        *dst = *(src_beg + i);
        arg->text_buffer_p->reserve1 = 0;
        goto dst_add;
      }
      else //multi-byte char
      {
        int chlen,cn;

        if(arg->codetype == CODETYPE_GBK)
          chlen = GBKCHLEN((uint8_t)src_beg[i]);
        else if(arg->codetype == CODETYPE_UTF8)
          chlen = UTF8CHLEN((uint8_t)src_beg[i]);
        else
          chlen = 2;

        if (i + chlen - 1 < char_cnt)  //防止*(src_beg+i+1)= '<'
        {
          //GBK中中文空格为A1A1,UTF-8中中文空格为E38080
          if (arg->is_reserve_indent == 0 && ( \
            (arg->codetype == CODETYPE_GBK  && chlen == 2 && *(src_beg + i) == '\xa1' && *(src_beg + i + 1) == '\xa1')\
          ||(arg->codetype == CODETYPE_UTF8 && chlen == 3 \
            && *(src_beg + i) == '\xe3' && *(src_beg + i + 1) == '\x80' && *(src_beg + i + 2) == '\x80') \
          )) {
            if (arg->text_buffer_p->reserve1 == 1) {
              i += (chlen-1);
              continue;
            } else {
              *dst = ' ';
              arg->text_buffer_p->reserve1 = 1;
              i += (chlen -1);
              goto dst_add;
            }
          } else {
            for(cn=0;cn<chlen;++cn) {
              *dst = *(src_beg + i + cn);
              ++dst;
              arg->text_buffer_p->pos++;
              arg->text_buffer_p->free--;
              if (arg->text_buffer_p->free <= 1) {
                arg->last_tag_end_position = taginfo->end_position;
                return NULL; /* html page parser is end */
              }
            }
            arg->text_buffer_p->reserve1 = 0;
            i += (chlen -1);
            continue;
          }
        } else { //this multi byte char error.
          break;
        }
      }

    dst_add:
      ++dst;
      arg->text_buffer_p->pos++;
      arg->text_buffer_p->free--;
      if (arg->text_buffer_p->free <= 1) {
        arg->last_tag_end_position = taginfo->end_position;
        return NULL; /* html page parser is end */
      }
    }
  } /* end of process between last tag and this tag text */

  /* comment tag or declare tag or  close tag ?*/
  if (taginfo->end_tag_p == DECLARE_TAG /*|| taginfo->end_tag_p == CLOSE_TAG*/)
  {
    arg->last_tag_end_position = taginfo->end_position;
    return (taginfo->end_position - 1);
  }

  /* this tag is special tag ? */
  spec_tag = hash_table_get(arg->spec_tag_ht, taginfo->name);
  if (!spec_tag)  /* not a speical tag */
  {
    arg->last_tag_end_position = taginfo->end_position;
    return (taginfo->end_position - 1);
  }

  switch (spec_tag->type)
  {
    case SKIP_TAG:
      if (taginfo->end_tag_p == CLOSE_TAG)
      {
        arg->last_tag_end_position = taginfo->end_position;
        return (taginfo->end_position - 1);
      }

      if (spec_tag->handler == NULL)
      {
        /* process block tag */
        /* <block >....block....</blcok> */
        /*         ^            ^      ^ */
        /*        block_beg    block_end p */
        const char *block_beg, *block_end;
        block_beg = taginfo->end_position;
        block_end = stristrb(block_beg, arg->html_content_end_position - block_beg, spec_tag->end_str, strlen(spec_tag->end_str));
        if (block_end == NULL)
        {
          arg->last_tag_end_position = taginfo->end_position;
          return (taginfo->end_position - 1);
        }
        else
        {
          p = memchr(block_end, '>', arg->html_content_end_position - block_end);
          if (p == NULL)
          {
            arg->last_tag_end_position = taginfo->end_position;
            return NULL;
          }

          arg->last_tag_end_position = p + 1;
          return p;
        }
      }
      else
      {
        return spec_tag->handler(taginfo, arg, spec_tag);
      }

    case RESERVE_TAG:
      if (spec_tag->handler == NULL)
      {
        int tag_len,k, blen, bpos;;
        const char *tag_ptr;
        char tagbuf[1024];
        tag_attr_t * tagattr;
        
        // 保留标签，但需要过滤掉属性，只保留指定的属性,added @2011.4.10
        if(taginfo->end_tag_p == START_TAG) {
          tagbuf[0] = 0;
          bpos = 0;
          blen = snprintf(tagbuf+bpos,sizeof(tagbuf)-bpos,"<%s",taginfo->name);
          bpos += blen;
          
          for(k=0;k<taginfo->nattrs;k++) {
            tagattr = hash_table_get(arg->tag_attr_ht, taginfo->attrs[k].name);
            if(tagattr) 
            {
              do {
                if(strcasecmp(taginfo->name, tagattr->tag_name) == 0)
                  break;
                else 
                  tagattr=tagattr->next;
              } while(tagattr);

              if(!tagattr)
                continue;

              if(arg->xssfilter) {
                if( (strcasecmp(taginfo->name,"img") == 0 && strcasecmp(taginfo->attrs[k].name, "src") == 0)
                  ||(strcasecmp(taginfo->name,"a") == 0 && strcasecmp(taginfo->attrs[k].name, "href") == 0)) {
                  if(  taginfo->attrs[k].value \
                    && !strcasebeginwith("http://",taginfo->attrs[k].value) \
                    && !strcasebeginwith("https://",taginfo->attrs[k].value) )
                    continue;
                }
              }
              
              if(taginfo->attrs[k].value)
                blen = snprintf(tagbuf+bpos,sizeof(tagbuf)-bpos," %s=\"%s\"",taginfo->attrs[k].name,taginfo->attrs[k].value);
              else
                blen = snprintf(tagbuf+bpos,sizeof(tagbuf)-bpos," %s=\"%s\"",taginfo->attrs[k].name,taginfo->attrs[k].name);

              if(blen < 0 || blen >= sizeof(tagbuf)-bpos) {
                arg->last_tag_end_position = taginfo->end_position;
                return NULL;
              }

              bpos += blen;
            }
          }

          if(strcasecmp(taginfo->name,"br") == 0 || strcasecmp(taginfo->name,"img") == 0)
            blen = snprintf(tagbuf + bpos, sizeof(tagbuf) - bpos, " />");
          else if(arg->xssfilter && strcasecmp(taginfo->name,"a") == 0) 
            blen = snprintf(tagbuf + bpos, sizeof(tagbuf) - bpos, " target=\"_blank\" >");
          else
            blen = snprintf(tagbuf + bpos, sizeof(tagbuf) - bpos, " >");
          bpos += blen;

          tag_len = bpos;
          tag_ptr = tagbuf;
        } else {
          tag_len = taginfo->end_position - taginfo->start_position;
          tag_ptr = taginfo->start_position;
        }

        if (arg->text_buffer_p->free >= tag_len) {
          memcpy(arg->text_buffer_p->p + arg->text_buffer_p->pos, tag_ptr, tag_len);
          arg->text_buffer_p->pos += tag_len;
          arg->text_buffer_p->free -= tag_len;
          arg->last_tag_end_position = taginfo->end_position;
          return (taginfo->end_position - 1);
        } else {
          arg->last_tag_end_position = taginfo->end_position;
          return NULL;
        }
      }
      else
      {
        return spec_tag->handler(taginfo, arg, spec_tag);
      }

    case EXTRACT_TAG:
      if (taginfo->end_tag_p == CLOSE_TAG)
      {
        arg->last_tag_end_position = taginfo->end_position;
        return (taginfo->end_position - 1);
      }

      if (spec_tag->handler == NULL)
      {
        arg->last_tag_end_position = taginfo->end_position;
        return (taginfo->end_position - 1);
      }
      else
      {
        return spec_tag->handler(taginfo, arg, spec_tag);
      }
    default:
      arg->last_tag_end_position = taginfo->end_position;
      return (taginfo->end_position - 1);
  }
}

/* process block tag */
/* <block >....block....</blcok> */
/*         ^            ^      ^ */
/*        block_beg    block_end p */

const char *tag_handler_title(struct taginfo *taginfo, map_arg_t* maparg, spec_tag_t *spec_tag)
{
  const char *block_beg, *block_end, *p;
  buffer_t *title_buffer = (buffer_t *)spec_tag->handler_arg;
  int no_title_len = strlen("no title");

  block_beg = taginfo->end_position;
  block_end = stristrb(block_beg, maparg->html_content_end_position - block_beg, spec_tag->end_str, strlen(spec_tag->end_str));

  if (title_buffer->reserve1 >= 1) //表示已经提取了一个title，不再提取了
  {
    if (block_end == NULL)
    {
      maparg->last_tag_end_position = taginfo->end_position;
      return (taginfo->end_position - 1);
    }
    else
    {
      p = memchr(block_end, '>', maparg->html_content_end_position - block_end);
      if (p == NULL)
      {
        maparg->last_tag_end_position = taginfo->end_position;
        return (taginfo->end_position - 1);
      }
      maparg->last_tag_end_position = p + 1;
      return p;
    }
  }

  if (block_end == NULL)
  {
    if (title_buffer->free > no_title_len)
    {
      strcpy(title_buffer->p + title_buffer->pos, "no title");
      title_buffer->pos += no_title_len;
      title_buffer->free -= no_title_len;
    }

    title_buffer->reserve1 = 1;
    maparg->last_tag_end_position = taginfo->end_position;
    return (taginfo->end_position - 1);
  }
  else
  {
    int title_len = block_end - block_beg;
    if (title_len == 0)
    {
      if (title_buffer->free > no_title_len)
      {
        strcpy(title_buffer->p + title_buffer->pos, "no title");
        title_buffer->pos += no_title_len;
        title_buffer->free -= no_title_len;
      }
    }
    else
    {
      if (title_len < title_buffer->free)
      {
        memcpy(title_buffer->p + title_buffer->pos, block_beg, title_len);
        title_buffer->pos += title_len;
        title_buffer->free -= title_len;
        *(title_buffer->p + title_buffer->pos) = 0;
      }
      else
      {
        memcpy(title_buffer->p + title_buffer->pos, block_beg, title_buffer->free - 1);
        title_buffer->pos += (title_buffer->free - 1);
        title_buffer->free -= (title_buffer->free - 1);
        *(title_buffer->p + title_buffer->pos) = 0;
      }

      trim(title_buffer->p);
      title_buffer->pos = strlen(title_buffer->p);
      if (title_buffer->pos == 0)
      {
        if (title_buffer->size > no_title_len)
        {
          strcpy(title_buffer->p, "no title");
          title_buffer->pos = no_title_len;
          title_buffer->free = title_buffer->size - no_title_len;
        }
        else
        {
          title_buffer->pos = 0;
          title_buffer->free = title_buffer->size;
        }
      }
    }

    title_buffer->reserve1 = 1;
    p = memchr(block_end, '>', maparg->html_content_end_position - block_end);
    if (p == NULL)
    {
      maparg->last_tag_end_position = taginfo->end_position;
      return (taginfo->end_position - 1);
    }
    maparg->last_tag_end_position = p + 1;
    return p;
  }
}

const char *tag_handler_anchor(struct taginfo *taginfo, map_arg_t* maparg, spec_tag_t *spec_tag)
{
  const char *block_beg, *block_end, *p;
  buffer_t *anchor_text_buffer = spec_tag->handler_arg;

  block_beg = taginfo->end_position;
  block_end = stristrb(block_beg, maparg->html_content_end_position - block_beg, spec_tag->end_str, strlen(spec_tag->end_str));
  if (block_end == NULL)
  {
    maparg->last_tag_end_position = taginfo->end_position;
    return (taginfo->end_position - 1);
  }
  else
  {
    /* copy anchor text to anchor_text_buffer */
    int n = 0;
    char * ank_dst;
    ank_dst = anchor_text_buffer->p + anchor_text_buffer->pos;
    while (block_beg + n != block_end)
    {
      /* whitespace char */
      if (*(block_beg + n) < ' ' && *(block_beg + n) >= 0)
      {
        n++;
        continue;
      }

      //space
      if (*(block_beg + n) == ' ')
      {
        if (anchor_text_buffer->reserve1 == 1)
        {
          n++;
          continue;
        }
        else
        {
          *ank_dst = ' ';
          anchor_text_buffer->reserve1 = 1;
          n++;
          goto ank_dst_add;
        }
      }

      /* decode entities &nbsp; only */
      if (*(block_beg + n) == '&' && *(block_beg + n + 1) == 'n' &&
          *(block_beg + n + 2) == 'b' && *(block_beg + n + 3) == 's' && 
          *(block_beg + n + 4) == 'p' && *(block_beg + n + 5) == ';')
      {
        const char *tp = block_beg + n;
        int ret;
        ret = decode_entity(&tp, block_end);
        if (ret != -1)
        {
          if (ret == ' ')
          {
            if (anchor_text_buffer->reserve1 == 1)
            {
              /* &nbsp; */
              /* ^     ^tp */
              n = tp - block_beg;
              continue;
            }
            *ank_dst = ' ';
            anchor_text_buffer->reserve1 = 1;
            n = tp - block_beg;
            goto ank_dst_add;
          }

          *ank_dst = ret;
          anchor_text_buffer->reserve1 = 0;
          n = tp - block_beg;
          goto ank_dst_add;
        }
      }

      /* skip html tag between <a>...</a> */
      if (*(block_beg + n) == '<')
      {
        char *close_tag;
        close_tag = (char *)memchr(block_beg + n + 1, '>', block_end - (block_beg + n + 1));
        if (close_tag == NULL) /* error html tag,skip this anchor tag */
        {
          p = memchr(block_end, '>', maparg->html_content_end_position - block_end);
          if (p == NULL)
          {
            maparg->last_tag_end_position = taginfo->end_position;
            return NULL;
          }
          maparg->last_tag_end_position = p + 1;
          return p;
        }

        n += (close_tag - (block_beg + n) + 1);
        continue;
      }

      /* ascii char */
      if (*((uint8_t *)(block_beg + n)) < 0x80)
      {
        *ank_dst = *(block_beg + n);
        anchor_text_buffer->reserve1 = 0;
        n++;
        goto ank_dst_add;
      }
      else  /* multi-byte char */
      {
        int chlen,cn;

        if(maparg->codetype == CODETYPE_GBK)
          chlen = GBKCHLEN((uint8_t)block_beg[n]);
        else if(maparg->codetype == CODETYPE_UTF8)
          chlen = UTF8CHLEN((uint8_t)block_beg[n]);
        else
          chlen = 2;

        if (block_beg + n + chlen <= block_end) {
          //GBK中中文空格为A1A1,UTF-8中中文空格为E38080
          if (maparg->is_reserve_indent == 0 && ( \
            (maparg->codetype == CODETYPE_GBK  && chlen == 2 && *(block_beg + n) == '\xa1' && *(block_beg + n + 1) == '\xa1')\
          ||(maparg->codetype == CODETYPE_UTF8 && chlen == 3 \
            && *(block_beg + n) == '\xe3' && *(block_beg + n + 1) == '\x80' && *(block_beg + n + 2) == '\x80')))
          {
              if (anchor_text_buffer->reserve1 == 1)
              {
                n += chlen;
                continue;
              }
              else
              {
                *ank_dst = ' ';
                anchor_text_buffer->reserve1 = 1;
                n += chlen;
                goto ank_dst_add;
              }
          } else {
            for(cn=0;cn<chlen;++cn) {
              *ank_dst = *(block_beg + n + cn);
              ++ank_dst;
              anchor_text_buffer->pos++;
              anchor_text_buffer->free--;
              if (anchor_text_buffer->free <= 1)
              {
                maparg->last_tag_end_position = taginfo->end_position;
                return NULL; /* html page parser is end */
              }
            }
            
            anchor_text_buffer->reserve1 = 0;
            n += chlen;
            continue;
          }
        } else { //this multi byte char error.
          break;
        }
      }

    ank_dst_add:
      ++ank_dst;
      anchor_text_buffer->pos++;
      anchor_text_buffer->free--;
      if (anchor_text_buffer->free <= 1)
      {
        maparg->last_tag_end_position = taginfo->end_position;
        return NULL; /* html page parser is end */
      }
    }/* end for while */

    if (anchor_text_buffer->reserve1 == 0)
    {
      *ank_dst = ' ';
      anchor_text_buffer->reserve1 = 1;
      ++ank_dst;
      anchor_text_buffer->pos++;
      anchor_text_buffer->free--;
      if (anchor_text_buffer->free == 0)
      {
        maparg->last_tag_end_position = taginfo->end_position;
        return NULL; /* html page parser is end */
      }
    }

    p = memchr(block_end, '>', maparg->html_content_end_position - block_end);
    if (p == NULL)
    {
      maparg->last_tag_end_position = taginfo->end_position;
      return NULL;
    }
    maparg->last_tag_end_position = p + 1;
    return p;
  }
}
/*
<meta http-equiv="content-type" content="text/html; charset=gb2312">
<meta name="description" content="中国造ARJ21-700飞机将于明年上半年首飞">
<meta name="keywords" content="飞机 支线">
*/
const char *tag_handler_meta(struct taginfo *taginfo, map_arg_t* maparg, spec_tag_t *spec_tag)
{
  struct _meta_text_buffer * b = (struct _meta_text_buffer *)spec_tag->handler_arg;
  buffer_t *meta_keyword_buffer = b->meta_keyword_buffer_p;
  buffer_t *meta_desc_buffer = b->meta_desc_buffer_p;
  int i, j, len;

  for (i = 0;i < taginfo->nattrs;i++)
  {
    if (strcasecmp(taginfo->attrs[i].name, "name") == 0 && strcasecmp(taginfo->attrs[i].value, "description") == 0)
    {
      for (j = 0;j < taginfo->nattrs;j++)
      {
        if (strcasecmp(taginfo->attrs[j].name, "content") == 0)
        {
          len = strlen(taginfo->attrs[j].value);
          if (meta_desc_buffer->free >= len) {
            memcpy(meta_desc_buffer->p + meta_desc_buffer->pos, taginfo->attrs[j].value, len);
            meta_desc_buffer->pos += len;
            meta_desc_buffer->free -= len;
          }
         
          maparg->last_tag_end_position = taginfo->end_position;
          return (taginfo->end_position - 1);
        }
      }
    }

    if (strcasecmp(taginfo->attrs[i].name, "name") == 0 && strcasecmp(taginfo->attrs[i].value, "keywords") == 0)
    {
      for (j = 0;j < taginfo->nattrs;j++)
      {
        if (strcasecmp(taginfo->attrs[j].name, "content") == 0)
        {
          len = strlen(taginfo->attrs[j].value);
          if (meta_keyword_buffer->free >= len) {
            memcpy(meta_keyword_buffer->p + meta_keyword_buffer->pos, taginfo->attrs[j].value, len);
            meta_keyword_buffer->pos += len;
            meta_keyword_buffer->free -= len;
          }
          
          maparg->last_tag_end_position = taginfo->end_position;
          return (taginfo->end_position - 1);
        }
      }
    }
  }

  maparg->last_tag_end_position = taginfo->end_position;
  return (taginfo->end_position - 1);
}

/*
<img src=http://counter.csdn.net/pv.aspx?id=23 border=0 width=0 height=0>
*/

const char *tag_handler_img(struct taginfo *taginfo, map_arg_t* maparg, spec_tag_t *spec_tag)
{
  struct _img_arg *img_arg = (struct _img_arg *)spec_tag->handler_arg;
  buffer_t *text_buffer = maparg->text_buffer_p;
  buffer_t *url_buffer = img_arg->url_buffer_p;
  int i, j, k, url_len, len, rpos, *w, imgextnum;
  char *file, *ext, *p, *alt;
  char url_md5[33], url[MAX_URL_LEN+1], rpath[MAX_PATH_LEN+1];
  const char *imgext[] = {".jpg", ".png", ".jpeg", ".gif", ".bmp"};

  switch (img_arg->op)
  {
    case IMG_OP_FILTER:
      goto end;
    case IMG_OP_RESERVE:
      for (i=0;i<taginfo->nattrs;i++) {
        if (strcasecmp(taginfo->attrs[i].name, "src") == 0) {
          if(!taginfo->attrs[i].value || !taginfo->attrs[i].value[0]) 
            goto end;
          p = url_mergenormalize(taginfo->attrs[i].value, img_arg->base, img_arg->referer, img_arg->pageisutf8, img_arg->cd,url, MAX_URL_LEN + 1);
          if (!p) goto end;
          APPEND_BUFFER((*text_buffer),"<img ",5);
          APPEND_BUFFER((*text_buffer),"src=\"",5);
          APPEND_BUFFER((*text_buffer),url,strlen(url));
          APPEND_BUFFER((*text_buffer),"\" ",2);
          for (j=0;j<taginfo->nattrs;j++) {
            if (strcasecmp(taginfo->attrs[j].name, "src") != 0) {
              if(!taginfo->attrs[j].value || !taginfo->attrs[j].value[0])
                continue;
              APPEND_BUFFER((*text_buffer),taginfo->attrs[j].name,strlen(taginfo->attrs[j].name));
              APPEND_BUFFER((*text_buffer),"=\"",2);
              APPEND_BUFFER((*text_buffer),taginfo->attrs[j].value,strlen(taginfo->attrs[j].value));
              APPEND_BUFFER((*text_buffer),"\" ",2);
            }
          }
          APPEND_BUFFER((*text_buffer)," />",3);
          goto end;
        }
      }
      break;
    /* tag_len = taginfo->end_position - taginfo->start_position;
      if (text_buffer->free >= tag_len) {
        memcpy(text_buffer->p + text_buffer->pos, taginfo->start_position, tag_len);
        text_buffer->pos += tag_len;
        text_buffer->free -= tag_len;
        goto end;
      } else {
        maparg->last_tag_end_position = taginfo->end_position;
        return NULL;
      } */
    case IMG_OP_REPLACE:
      for (i = 0;i < taginfo->nattrs;i++) {
        if (strcasecmp(taginfo->attrs[i].name, "src") == 0) {
          if(!taginfo->attrs[i].value || !taginfo->attrs[i].value[0])
            goto end;
          p = url_mergenormalize(taginfo->attrs[i].value, img_arg->base, img_arg->referer, img_arg->pageisutf8, img_arg->cd,url, MAX_URL_LEN + 1);
          if (!p) goto end;

          file = strchr(url + 7, '/');
          if (!file) goto end;
          url_len = strlen(url);
          md5s((uint8_t *)url, url_len, url_md5);

          rpos = 0;
          rpath[0] = 0;
          if (img_arg->image_option_dir) {
            strcpy(rpath, img_arg->image_option_dir);
            rpos += strlen(img_arg->image_option_dir);
            if (rpath[rpos-1] != PATH_SEPARATOR) {
              rpath[rpos] = PATH_SEPARATOR;
              rpath[++rpos] = '\0';
            }
          }

          if (img_arg->image_subdir_num) {
            p = url_md5;
            w = img_arg->image_subdirname_width;
            for (k = 0;k < img_arg->image_subdir_num;k++) {
              memcpy(rpath + rpos, p, w[k]);
              rpos += w[k];
              p += w[k];
              rpath[rpos++] = '/';
              rpath[rpos] = 0;
            }
          }

          memcpy(rpath + rpos, url_md5, 32);
          rpos += 32;
          rpath[rpos] = 0;

          ext = strrchr(file, '.');
          if (ext) {
            imgextnum = countof(imgext);
            for (k = 0;k < imgextnum;k++) {
              if (strcasecmp(ext, imgext[k]) == 0)
                break;
            }
            if (k != imgextnum) {
              len = strlen(ext);
              memcpy(rpath + rpos, ext, len);
              rpos += len;
              rpath[rpos] = 0;
            }
          }

          //xhtml中每个图片标签都必须有ALT说明文字。
          //<img src="ball.jpg" alt="large red ball" />
          //保留原来img中alt文本。
          alt = "";
          for (j=0;j<taginfo->nattrs;j++){
            if (strcasecmp(taginfo->attrs[j].name, "alt") == 0 && taginfo->attrs[j].value) {
              alt = taginfo->attrs[j].value;
              break;
            }
          }
          
          APPEND_BUFFER((*text_buffer),"<img ",5);
          APPEND_BUFFER((*text_buffer),"src=\"",5);
          if(img_arg->image_base_url) {
            APPEND_BUFFER((*text_buffer),img_arg->image_base_url,strlen(img_arg->image_base_url));
          }
          APPEND_BUFFER((*text_buffer),rpath,strlen(rpath));
          APPEND_BUFFER((*text_buffer),"\" ",2);
          APPEND_BUFFER((*text_buffer),"alt=\"",5);
          APPEND_BUFFER((*text_buffer),alt,strlen(alt));
          APPEND_BUFFER((*text_buffer),"\" ",2);
          for (j=0;j<taginfo->nattrs;j++) {
            if (strcasecmp(taginfo->attrs[j].name, "src") != 0 && \
                strcasecmp(taginfo->attrs[j].name, "alt") != 0) {
              if(!taginfo->attrs[j].value || !taginfo->attrs[j].value[0])
                continue;
              APPEND_BUFFER((*text_buffer),taginfo->attrs[j].name,strlen(taginfo->attrs[j].name));
              APPEND_BUFFER((*text_buffer),"=\"",2);
              APPEND_BUFFER((*text_buffer),taginfo->attrs[j].value,strlen(taginfo->attrs[j].value));
              APPEND_BUFFER((*text_buffer),"\" ",2);
            }
          }
          APPEND_BUFFER((*text_buffer)," />",3);
          
          len = strlen(img_arg->url_prefix);
          if (url_buffer->free < len + url_len + 1) { /* '\n' */
            RESIZE_BUFFER((*url_buffer), url_buffer->size, (len + url_len + 1)*2)
          }
          memcpy(url_buffer->p + url_buffer->pos, img_arg->url_prefix, len);
          url_buffer->pos += len;
          url_buffer->free -= len;
          memcpy(url_buffer->p + url_buffer->pos, url, url_len);
          url_buffer->pos += url_len;
          url_buffer->free -= url_len;
          *(url_buffer->p + url_buffer->pos) = '\n';
          url_buffer->pos ++;
          url_buffer->free --;
          url_buffer->reserve2++;
          goto end;
        }
      }
      break;
    default:
      break;
  }

end:
  maparg->last_tag_end_position = taginfo->end_position;
  return (taginfo->end_position - 1);
}

struct interesting_tag_attr
{
  char *interesting_tag_attr;
  buffer_t *interesting_tag_attr_buf_p;
  int count;
  int is_ok;
};

/* <BASE HREF="http://msdn.microsoft.com/workshop/author/dhtml/reference/"/>
   <a href=""> text </a>
   <img src="" >

   return:buffer:Url\nUrl\...\n
*/
int extract_interesting_tag_attr(char *html_content, int content_len,
                                 char *interesting_tag, char *interesting_tag_attr,
                                 buffer_t *interesting_tag_attr_buf_p, int count)
{
  struct interesting_tag_attr maparg;
  struct hash_table * interesting_tag_ht, *interesting_attr_ht;

  if (count == 0)
    return -1;

  interesting_tag_ht = make_nocase_string_hash_table(1);
  interesting_attr_ht = make_nocase_string_hash_table(1);
  hash_table_put(interesting_tag_ht, interesting_tag, interesting_tag_attr);
  hash_table_put(interesting_attr_ht, interesting_tag_attr, interesting_tag);

  maparg.interesting_tag_attr = interesting_tag_attr;
  maparg.interesting_tag_attr_buf_p = interesting_tag_attr_buf_p;
  maparg.is_ok = 0;
  maparg.count = count;

  map_html_tags(html_content, content_len, extract_interesting_tag_attr_mapfun, &maparg, MHT_TRIM_VALUES, interesting_tag_ht, interesting_attr_ht);

  if (interesting_tag_ht)
    hash_table_destroy(interesting_tag_ht);

  if (interesting_attr_ht)
    hash_table_destroy(interesting_attr_ht);

  if (maparg.is_ok)
    return 0;
  else
    return -1;
}

const char * extract_interesting_tag_attr_mapfun(struct taginfo *taginfo, void *maparg)
{
  struct interesting_tag_attr * arg = (struct interesting_tag_attr *)maparg;
  int i, len;

  if (taginfo->end_tag_p == CLOSE_TAG)
    return (taginfo->end_position - 1);

  for (i = 0;i < taginfo->nattrs;i++)
  {
    if (strcasecmp(taginfo->attrs[i].name, arg->interesting_tag_attr) == 0)
    {
      len = strlen(taginfo->attrs[i].value);
      if(!len) continue;
      if (arg->interesting_tag_attr_buf_p->free < len + 1) /* '\n' */
      {
        RESIZE_BUFFER((*(arg->interesting_tag_attr_buf_p)), arg->interesting_tag_attr_buf_p->size, (len + 1) * 2)
      }

      memcpy(arg->interesting_tag_attr_buf_p->p + arg->interesting_tag_attr_buf_p->pos, taginfo->attrs[i].value, len);
      arg->interesting_tag_attr_buf_p->free -= len;
      arg->interesting_tag_attr_buf_p->pos += len;

      *(arg->interesting_tag_attr_buf_p->p + arg->interesting_tag_attr_buf_p->pos) = '\n';
      arg->interesting_tag_attr_buf_p->free --;
      arg->interesting_tag_attr_buf_p->pos ++;

      arg->interesting_tag_attr_buf_p->reserve2 ++;

      arg->is_ok = 1;

      if (arg->count > 0)
        arg->count --;

      if (arg->count)
        return (taginfo->end_position - 1);
      else
        return NULL;
    }
  }

  return (taginfo->end_position - 1);
}

int get_content_type(const char *header)
{
  char *p;

  p = stristr(header, "content-type: ");
  if (!p)
    return OTHER_CONTENT;

  if (strstartwithignorecase("text/html", p + 14))
    return HTML_CONTENT;
  else if (strstartwithignorecase("image", p + 14))
    return IMAGE_CONTENT;
  else
    return OTHER_CONTENT;
}

char * get_last_modified(const char *header, char *last_modified, int size)
{
  char *pb, *pe;
  int len;

  pb = stristr(header, "Last-Modified: ");
  if (!pb)
    return NULL;

  pb += 15;
  pe = strstr(pb, "\r\n");
  if (!pe)
    return NULL;

  len = pe - pb;
  if (len >= size)
    return NULL;

  memcpy(last_modified, pb, len);
  *(last_modified + len) = 0;

  return last_modified;
}

char * get_base_url(char *html_content, int content_len, buffer_t *base_url_buffer_p, int buffer_size)
{
  int ret_val;

  ret_val = extract_interesting_tag_attr(html_content, content_len, "base", "href", base_url_buffer_p, 1);
  if (ret_val == -1 || base_url_buffer_p->reserve2 != 1)
    return NULL;

  base_url_buffer_p->pos--; /* delete '\n' */

  if (base_url_buffer_p->pos >= buffer_size)
  {
    RESUME_BUFFER((*base_url_buffer_p), buffer_size)
    return NULL;
  }
  else
  {
    *(base_url_buffer_p->p + base_url_buffer_p->pos) = '\0';
    return base_url_buffer_p->p;
  }
}

/* next mode:
     <a href=""> 下一页</a>
     <a href="/a/20080625/000392_1.htm"><img src="http://mat1.qq.com/2008/images/final/down.gif" alt="下一页" align="absmiddle"></a>
     <a href="./07122491944C333DH2H7BHD9H1A9CGF_2.html?lt=common" title="下一页"><font face="webdings">4</font></a>
     <a href= ""> [4]</a> [5] .... 下一页
     <a href ="">上一页</a> <a href="">[1]</a> [2]
      如果"下一页" 不是anchor text，则说明此页已经是最后一页了。

     all list mode:
     [1]
     <a href="/a/20080625/000392_1.htm">[2]</a>
     <a href="/a/20080625/000392_2.htm">[3]</a>
     <a href="/a/20080625/000392_3.htm">[4]</a>

     2 3 4 --> next page: 2
     1 3 4 --> next page: 3
     1 2 4 --> next page: 4
     1 2 3 --> not next page

     part list mode:暂不支持部分列表模式
     2 3 4 5 6  --> next page: 2
     4 5 7 8 9 --> next page: 7
     6 7 9 10 11 --> next page: 9

     return:0: buffer:Url\n,-1:not url,-2:not wait page url,is other wait url,or html content format error

*/

int get_next_page_url(char *html_content, int content_len, buffer_t *next_url_buffer_p, char * next_text, char *prev_text)
{
  char *pb, *pm, *pe;
  char *pbeg, *pend;
  int next_text_len, ret_val, prev_page, cur_page;
  //char *p;

  pend = html_content + content_len;
  next_text_len =  strlen(next_text);
  prev_page = 0;
  cur_page = 0;

  //<a href=""> [1] </a>
  //pb               pm     pe

  //is next mode ?
  if (stristrb(html_content, content_len, next_text, next_text_len) != NULL || stristrb(html_content, content_len, prev_text, strlen(prev_text)) != NULL)
  {
    pbeg = html_content;
    while ((pb = stristrb(pbeg, pend - pbeg, "<a", 2)) != NULL)
    {
      pe = stristrb(pb, pend - pb, "</a", 3);
      if (!pe)
        return -1;

      pm = stristrb(pb, pe - pb, ">", 1);
      if (!pm)
        return -1;

      //<a href =....>anchor text </a> has next text?
      if (stristrb(pb, pe - pb, next_text, next_text_len) != NULL)
      {
        ret_val = extract_interesting_tag_attr(pb, pm + 1 - pb, "a", "href", next_url_buffer_p, 1);
        return ret_val;
      }

      pbeg = pe;
    }

    return -1;
  }

  //all  list mode
  pbeg = html_content;
  pb = stristrb(pbeg, pend - pbeg, "<a", 2);//first anchor
  if (!pb)
    return -1;
  pe = stristrb(pb, pend - pb, "</a", 3);
  if (!pe)
    return -1;
  pm = stristrb(pb, pe - pb, ">", 1);
  if (!pm)
    return -1;

  cur_page = strextractint(pm, pe - pm);
  switch (cur_page)
  {
    case - 1: //not page url
      return -2;                ///////////////
    case 2:  // 2 3 4 ...
      ret_val = extract_interesting_tag_attr(pb, pm + 1 - pb, "a", "href", next_url_buffer_p, 1);
      return ret_val;
    case 1: // 1 . . .
      prev_page = 1;
      pbeg = pe;
      while ((pb = stristrb(pbeg, pend - pbeg, "<a", 2)) != NULL)
      {
        pe = stristrb(pb, pend - pb, "</a", 3);
        if (!pe)
          return -1;

        pm = stristrb(pb, pe - pb, ">", 1);
        if (!pm)
          return -1;

        cur_page = strextractint(pm, pe - pm);
        if (cur_page < 0)
          continue;

        if (cur_page - prev_page == 2)
        {
          ret_val = extract_interesting_tag_attr(pb, pm + 1 - pb, "a", "href", next_url_buffer_p, 1);
          return ret_val;
        }

        if (cur_page -prev_page != 1)
          return -1;

        prev_page = cur_page;
        pbeg = pe;
      }
      return -1;
    default://next page or part list mode
      return -1;
  }
}

///////////////////////////////////////////////////////////////////////////
// used for GBK
static char *anchor_filt[] = {
  "[","]","(",")","{","}",
  "\"","'",
  "【","】","〖","〗","〔","〕","〈","〉","《","》","「","」","『","』",
  " ","\r","\n","\t","\f","\v","\xa1\xa1","<(*)>"
};

// 为了防止把文章的第一页当作列表页，需要去掉表示往前翻页和第一页的标志。
static char *anchor_equal[] = {
  ">", ">>",  ">>>","∨", "→", "↓", "▼", "▽",
  "〇", "二", "三", "四", "五", "六", "七", "八", "九", "十",
  "零", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖", "拾",
  "Ⅱ", "Ⅲ", "Ⅳ", "Ⅴ", "Ⅵ", "Ⅶ", "Ⅷ", "Ⅸ", "Ⅹ", "Ⅺ", "Ⅻ",
  "⒉", "⒊", "⒋", "⒌", "⒍", "⒎", "⒏", "⒐", "⒑", "⒒", "⒓", "⒔", "⒕", "⒖", "⒗", "⒘", "⒙", "⒚", "⒛",
  "②", "③", "④", "⑤", "⑥", "⑦", "⑧", "⑨", "⑩", 
  "㈡", "㈢", "㈣", "㈤", "㈥", "㈦", "㈧", "㈨", "㈩", 
  "⑵", "⑶", "⑷", "⑸", "⑹", "⑺", "⑻", "⑼", "⑽", "⑾", "⑿", "⒀", "⒁", "⒂", "⒃", "⒄", "⒅", "⒆", "⒇"
};

// 为了防止把文章的第一页当作列表页，需要去掉表示往前翻页和第一页的文本。
static char *anchor_contain[] = {
  "下一页",   "下页",     "后一页",   "后页", 
  "尾页",     "末页",     "终点页",   "终页", 
  "最后一页", "最后页",   "最尾页",   "最末页",   "最终页", 
  "最尾一页", "最末一页", "最终一页", 
  "最尾1页",  "最末1页",  "最终1页",
  "最后1页",  "下1页",    "后1页", 
  "下5页",    "后5页",    "下10页",   "后10页", 
  "下五页",   "后五页",   "下十页",   "后十页"
};

struct listurl_maparg {
  struct hash_table *ht_tag;
  struct hash_table *ht_attr;

  int filtdup;
  int hashsize;
  struct hash_table *ht_listurl;
  
  iconv_t cd;
  const char *attr;
  const char *closetag;
  
  int digit_num;
  int alpha_num;
  int wrap_num;
  char **wraps;
  
  int contain_num;
  char **contains;
  int contain_vlen; 
  int contain_minlen;
  
  int equal_num;
  char **equals;
  int equal_vlen;
  
  int anchortext_maxlen;
  char *anchortext;

  buffer_t baseurl;
  buffer_t listurl;

  const char *pageend;
  const char *referer;
  const char *base;
  int pageisutf8;
  int is_ok;
};

void *extract_listurl_init(void *cd,int fd,int hashsize)
{
  struct listurl_maparg *ma;

  ma = (struct listurl_maparg *)xcalloc(1,sizeof(struct listurl_maparg));
  
  ma->ht_tag = make_nocase_string_hash_table(1);
  ma->ht_attr = make_nocase_string_hash_table(1);
  hash_table_put(ma->ht_tag, "a", "href");
  hash_table_put(ma->ht_attr, "href", "a");

  if(fd > 0 && fd != FD_LISTURL_CRC32 && fd != FD_LISTURL_MD5)
    ma->filtdup = FD_LISTURL_CRC32;
  else if(fd < 0)
    ma->filtdup = FD_LISTURL_NONE;
  else
    ma->filtdup = fd;

  if(ma->filtdup > 0) {
    if(hashsize <= 0)
      ma->hashsize = 50000;
    else
      ma->hashsize = hashsize;
    ma->ht_listurl = make_string_hash_table(ma->hashsize);
  } else {
    ma->hashsize = 0;
    ma->ht_listurl = NULL;
  }
  
  ma->cd = cd;
  ma->attr = "href";
  ma->closetag = "</a";

  ma->digit_num = 5;
  ma->alpha_num = 1;
  ma->wrap_num = countof(anchor_filt);
  ma->wraps = anchor_filt;
  
  ma->contain_num = countof(anchor_contain);
  ma->contains = anchor_contain;
  ma->contain_vlen = 10;
  ma->contain_minlen = 4;
  ma->equal_num = countof(anchor_equal);
  ma->equals = anchor_equal;
  ma->equal_vlen = 3;
  
  ma->anchortext_maxlen = 16;
  ma->anchortext = (char *)xcalloc(ma->anchortext_maxlen+1,sizeof(char));

  INIT_BUFFER(ma->listurl,(20*1024));
  INIT_BUFFER(ma->baseurl,1024);
  
  return ma;
}

static const char * extract_listurl_mapfun(struct taginfo *taginfo, void *maparg)
{
  struct listurl_maparg *ma = (struct listurl_maparg*)maparg;
  const char *abeg,*aend;
  int i,attrlen,alen,isin;
  char *a,*p,URL[1024],ABUF[1024];
  char uid[33]; // use for md5 or crc32
  
  if(taginfo->end_tag_p == CLOSE_TAG)
    return (taginfo->end_position - 1);

  abeg = taginfo->end_position;
  aend = stristrb(abeg, ma->pageend-abeg, ma->closetag, strlen(ma->closetag));
  if(!aend)
    return NULL; // 后面内容里不存在</a，可以终止提取了
  alen = aend - abeg;
  if(alen >= sizeof(ABUF))
    return (taginfo->end_position - 1);
  memcpy(ABUF,abeg,alen);
  ABUF[alen] = 0;
  trim(ABUF);   // 去掉锚文本两边的空白字符
  alen = strlen(ABUF);
  if(alen > ma->anchortext_maxlen)
    return (taginfo->end_position - 1);
  else {
    memcpy(ma->anchortext,ABUF,alen);
    ma->anchortext[alen] = 0;
  }

  // 对链接锚文本进行处理...
  gbkqj2bj(ma->anchortext,ma->anchortext);
  gbkqj2bjext(ma->anchortext,ma->anchortext);
  strtolower(ma->anchortext,strlen(ma->anchortext),ma->anchortext,CODETYPE_GBK);
  htmldecode(ma->anchortext); // htmldecode()不支持&nbsp;解码，&nbsp;从页面提取时就已经解码了。
  a = strfilter(ma->anchortext,strlen(ma->anchortext),ma->wraps,ma->wrap_num,CODETYPE_GBK);
  strcpy(ma->anchortext,a);
  xfree(a);
  alen = strlen(ma->anchortext);
  if(alen == 0 || (alen > ma->digit_num && \
                   alen > ma->alpha_num && \
                   alen > ma->contain_vlen && \
                   alen > ma->equal_vlen))
    return (taginfo->end_position - 1);

  // 对锚文本进行列表特征检查...
  if(!isdigit(ma->anchortext[0]) && !isalpha(ma->anchortext[0])) {
    if(alen <= ma->equal_vlen) {
      for(i=0;i<ma->equal_num;i++) {
        if(strcasecmp(ma->equals[i],ma->anchortext) == 0)
          goto EXTRACT_LISTURL;
      }
    }
    if(alen >= ma->contain_minlen && alen <= ma->contain_vlen) {
      isin = strcontain(ma->anchortext,ma->contains,ma->contain_num,CODETYPE_GBK);
      if(isin >= 0)
        goto EXTRACT_LISTURL;
    }
    return (taginfo->end_position - 1);
  } else if(isdigit(ma->anchortext[0])) {
    if(alen > ma->digit_num)
      return (taginfo->end_position - 1);
    for(i=1;i<alen;i++) {
      if(!isdigit(ma->anchortext[i]))
        return (taginfo->end_position - 1);
    }
    if(alen == 1 && ma->anchortext[0] == '1') // 保留第一页
      return (taginfo->end_position - 1);
  } else if(isalpha(ma->anchortext[0])) {
    if(alen > ma->alpha_num)
      return (taginfo->end_position - 1);
    for(i=1;i<alen;i++) {
      if(!isalpha(ma->anchortext[i]))
        return (taginfo->end_position - 1);
    }
  }
  
  // 该链接指向的是列表页，提取列表页的URL
  EXTRACT_LISTURL:
  for(i=0;i<taginfo->nattrs;i++) {
    if (strcasecmp(taginfo->attrs[i].name, ma->attr) == 0) {
      attrlen = strlen(taginfo->attrs[i].value);
      if(!attrlen) continue;
      p = strchr(taginfo->attrs[i].value,'#'); //如果是页内定位URL则不提取。
      if(p) 
        return (taginfo->end_position - 1);
      if(strcasebeginwith("javascript:",taginfo->attrs[i].value))
        return (taginfo->end_position - 1);
      
      p = url_mergenormalize(taginfo->attrs[i].value,(char *)ma->base,(char *)ma->referer,ma->pageisutf8,ma->cd,URL,1024);
      if (!p || !p[0]) continue;

      // 判断该URL是否已经存在了
      if(ma->filtdup != FD_LISTURL_NONE) {
        if(ma->filtdup == FD_LISTURL_CRC32) 
          snprintf(uid,sizeof(uid),"%u",CRC32((uint8_t *)p));
        else if(ma->filtdup == FD_LISTURL_MD5)
          md5s((uint8_t *)p,strlen(p),uid);
 
        if(hash_table_contains(ma->ht_listurl,uid))
          return (taginfo->end_position - 1);
        else 
          hash_table_put(ma->ht_listurl,uid,NULL);
      } 

      APPEND_BUFFER(ma->listurl,p,strlen(p));
      APPEND_BUFFER(ma->listurl,"\n",1);
      ma->is_ok = 1;
      return (taginfo->end_position - 1);
    }
  }

  return (taginfo->end_position - 1);
}

const char *extract_listurl(void *maparg, const char *html_content, int content_len, const char *url, int pageisutf8)
{
  struct listurl_maparg *ma = (struct listurl_maparg *)maparg;

  if(!maparg || !html_content || !content_len)
    return NULL;

  CLEAR_BUFFER(ma->baseurl,ma->baseurl.size);
  CLEAR_BUFFER(ma->listurl,ma->listurl.size);
  ma->anchortext[0] = 0;
  ma->is_ok = 0;
  ma->pageend = html_content+content_len;
  ma->referer = url;
  ma->pageisutf8 = pageisutf8;
  ma->base = get_base_url((char*)html_content,content_len,&ma->baseurl,1024);

  map_html_tags(html_content, content_len, extract_listurl_mapfun, maparg, MHT_TRIM_VALUES, ma->ht_tag, ma->ht_attr);

  if (ma->is_ok)
    return ma->listurl.p;
  else
    return NULL;
}

void extract_listurl_free(void *maparg)
{
  struct listurl_maparg *ma = (struct listurl_maparg *)maparg;
  
  hash_table_destroy(ma->ht_tag);
  hash_table_destroy(ma->ht_attr);
  if(ma->ht_listurl)
    hash_table_destroy(ma->ht_listurl);
  xfree(ma->anchortext);
  FREE_BUFFER(ma->listurl);
  FREE_BUFFER(ma->baseurl);
  xfree(ma);
}

///////////////////////////////////////////////////////////////////////////

#ifdef TEST_EXTRACT

int main()
{
  buffer_t text_buffer;
  spec_tag_t spec_tag_list[] = {
  {"script",    "</script",   SKIP_TAG, NULL, NULL},
  {"style",     "</style",    SKIP_TAG, NULL, NULL},
  {"noscript",  "</noscript", SKIP_TAG, NULL, NULL},
  {"map",       "</map",      SKIP_TAG, NULL, NULL},
  {"noframes",  "</noframes", SKIP_TAG, NULL, NULL},
  {"iframe",    "</iframe",   SKIP_TAG, NULL, NULL},
  {"select",    "</select",   SKIP_TAG, NULL, NULL},
  {"textarea",  "</textarea", SKIP_TAG, NULL, NULL}
 };
  
  tag_attr_t tag_attr_list[] = {
   {"src", "img"},
   {"alt", "img"}
 };

  int tag_list_size = COUNTOF(spec_tag_list);
  int attr_list_size = COUNTOF(tag_attr_list);
  struct hash_table *spec_tag_ht, *tag_attr_ht;
  char *html_content;
  int content_len;

  char *testlisturl = "test list url \
    <base href='http://adb.com/' />\
    <a href='1.html'>　 【 １ 】 </a> \
    <a href='http://a.com/1.html'> 12345 </a> \
    <a href='1.html'> &LT;dec&gt;1 </a> \
    <a href='http://a.com/1.html'> 12345 </a> \
    <a href='http://a.com/1.html'> 111222 </a>\
    <a href='./3.html'> t </a>\
    <a href='../4.html'> taf </a>\
    <a href='3.html'> extract_listurl_init too long </a>";
  void *maparg = extract_listurl_init(NULL,0,0);
  const char *lu = extract_listurl(maparg,testlisturl,strlen(testlisturl),"http://www.gekang.com/",0);
  printf("%s",lu);
  testlisturl = "test list url \
    <base href='http://xxx.com/' />\
    <a href='1.html'>　 【 １ 】 </a> \
    <a href='http://a.com/1.html'> 12345 </a> \
    <a href='1.html'> &LT;dec&gt;1 </a> \
    <a href='http://a.com/1.html'> 12345 </a> \
    <a href='http://a.com/1.html'> 111222 </a>\
    <a href='./3.html'> t </a>\
    <a href='../4.html'> taf </a>\
    <a href='3.html'> extract_listurl_init too long </a>";
  lu = extract_listurl(maparg,testlisturl,strlen(testlisturl),"http://www.gekang.com/",0);
  printf("\n%s",lu);
  extract_listurl_free(maparg);
  return 1;
  

  INIT_BUFFER(text_buffer, MAX_TEXT_LEN);
  html_content = loadfile("test/test.html", (long *)&content_len);
  if(!html_content) {
    printf("load html file failure.\n");
    FREE_BUFFER(text_buffer);
    return -1;
  }
  extract_text_init(spec_tag_list, tag_list_size, &spec_tag_ht, tag_attr_list, attr_list_size, &tag_attr_ht);
  extract_text(html_content, content_len, &text_buffer, spec_tag_ht, tag_attr_ht,1,CODETYPE_UTF8,0);// utf-8
  fwrite("TEXT: ", 1, 6, stdout);
  fwrite(text_buffer.p, 1, text_buffer.pos, stdout);
  fwrite("\r\n", 1, 2, stdout);
  extract_text_free(spec_tag_ht, tag_attr_ht);
  xfree(html_content);
  FREE_BUFFER(text_buffer);
  return 0;
}
#endif

