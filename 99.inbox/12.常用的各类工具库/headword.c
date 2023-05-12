/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recognizer.h"
#include "charset.h"

//提取中心词
void *headword_init(const char *dicdir, int codetype)
{
  void * Dic;
  Dic = recognizer_init(NULL, dicdir, RECOGNIZER_INIT_TYPE_LOAD, 0, codetype);
  return Dic;
}

int headword_extract(void *Dic, const char *content, char *wordbuf, int bufsize,int codetype)
{
  char words[MAX_SEGMENT_OUTPUT+1];
  int len, num, buflen, cnt,clen;
  char *p, *q, *pc;

  if (!Dic || !content  || !wordbuf || !bufsize)
    return -1;

  if(codetype != CODETYPE_GBK && codetype != CODETYPE_UTF8)
    return -1;

  len = strlen(content);
  if (len > MAX_SEGMENT_STRING)
    return -1;

  if (!len)
  {
    wordbuf[0] = 0;
    return 0;
  }

  if ((num = recognizer_words(Dic, content, words)) < 0)
    return -1;

  if (!num)
  {
    wordbuf[0] = 0;
    return 0;
  }

  buflen = 0;
  cnt = 0;
  p = words;
  q = p;
  while (*q)
  {
    if(codetype == CODETYPE_GBK)
      clen = GBKCHLEN((uint8_t)*q);
    else if(codetype == CODETYPE_UTF8)
      clen = UTF8CHLEN((uint8_t)*q);
    
    if (clen > 1) 
    {
      if (*(q + clen) == '\0')
      {
        break;
      }
      else
      {
        q += clen;
        continue;
      }
    }

    if (*q == ' ' || *(q + 1) == '\0') //注意最后一个地址元素后面没有分隔符
    {
      if (*q == ' ')
        *q = 0;

      pc = strrchr(p, WORD_TYPE_SEPARATOR);
      if (pc)
      {
        *pc = 0;
        len = pc - p;
        if (buflen + len + 1 >= bufsize)
          return -1;
        else
        {
          memcpy(wordbuf + buflen, p, len);
          buflen += len;
          wordbuf[buflen] = ' ';
          buflen++;
          cnt++;
        }

        q++;
        p = q;
      }
      else
      {
        q++;
        p = q;
      }
    }
    else
    {
      q++;
    }
  }

  if (buflen)
    wordbuf[buflen-1] = 0;
  else
    wordbuf[buflen] = 0;

  return cnt;
}

void headword_free(void *Dic)
{
  recognizer_free(Dic);
}

#ifdef TEST_HEADWORD
#include "xstring.h"

int main(int argc,char *argv[])
{
  void *dic;
  char str[1024],words[1024];

  if(argc != 3)
  {
    printf("arguments set error.\n");
    return -1;
  }
  
  dic = headword_init(argv[1],atoi(argv[2]));
  if(!dic)
  {
    printf("headword_init() error.\n");
    return -1;
  }

  while(fgets(str,1024,stdin))
  {
    trim(str);
    if(strcasecmp(str,"exit") == 0)
      break;
    
    if(headword_extract(dic,str,words,1024,atoi(argv[2])) > 0)
    {
      printf("words: %s\n",words);
    }
    else
      break;
  }

  headword_free(dic);
  return 0;
}

#endif

