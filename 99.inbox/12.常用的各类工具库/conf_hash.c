/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmalloc.h"
#include "xstring.h"
#include "utils.h"
#include "comn.h"
#include "phphash.h"

struct CONF_HASH {
  LPHASHTABLE ht;
  int itemnum;
};

void *conf_hash_new(const char *confpath)
{
	FILE *fp;
	char buffer[1024],*value;
  LPHASHTABLE ht;
  struct CONF_HASH *ch;
  int num;

  ch = (struct CONF_HASH *)xcalloc(1,sizeof(struct CONF_HASH));
  ht = (LPHASHTABLE)xcalloc(1,sizeof(HASHTABLE));
	if(hash_create(ht,463,NULL) < 0) {
    xfree(ht);
    xfree(ch);
    return NULL;
	}

	if(!(fp = fopen(confpath,"rb"))) {
    hash_destroy(ht);
    xfree(ht);
    xfree(ch);
    return NULL;
	}

  num = 0;
	while(fgets(buffer,sizeof(buffer),fp)) {
		trim(buffer);
		if(!buffer[0]) continue;
    if(buffer[0] == '#') continue;
		
		value = strrchr(buffer,' ');
		if(!value) goto freemem;
		*value++ = 0;
		trim(buffer);
		trim(value);
		if(!buffer[0] || !value[0]) goto freemem;

    strtolower_ascii(buffer);
	  hash_entry(ht,buffer,strlen(buffer),xstrdup(value));
    ++num;
	}

  if(num == 0) goto freemem;
	fclose(fp);
  ch->ht = ht;
  ch->itemnum = num;
	return ch;

freemem:
  fclose(fp);
  hash_destroy(ht);
  xfree(ht);
  xfree(ch);
  return NULL;
}

const char *conf_hash_get(void *CONFHASH,const char *key)
{
  struct CONF_HASH *ch = (struct CONF_HASH *)CONFHASH;
  char *value;

  if(!key || !*key)
    return NULL;

  if(hash_find(ch->ht,(char *)key,strlen(key),(void **)&value) == 0)
    return value;
  else
    return NULL;
}


void conf_hash_del(void *CONFHASH)
{
  struct CONF_HASH *ch = (struct CONF_HASH *)CONFHASH;
  LPHASHTABLE ht = ch->ht;
	LPHASHBUCKET p;
  
  p = ht->head;
  while (p) {
    xfree(p->data);
    p = p->next;
  }
		 
	hash_destroy(ht);	
  xfree(ht);
  xfree(ch);
}

