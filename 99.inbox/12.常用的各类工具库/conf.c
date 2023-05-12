/* Copyright (c) 2007 Liu xing zhi. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include "conf.h"
#include "xstring.h"
#include "xmalloc.h"

/* conf 文件比较大的话，建议定义一下 WITH_HASH  */
/* uncomment the next line to enable hashing */

/* #define WITH_HASH */

/* #define _WIN32 */
#if defined(_WIN32) && !defined(__BORLANDC__)
#ifndef strcasecmp
#define strcasecmp stricmp
#endif
#endif

/* const variable define */
#ifdef WITH_HASH
#define CONF_HASH_SECTION_SIZE  1024   /* size of the section hash table */
#define CONF_HASH_KEY_SIZE  1024  /* size of the key hash table of each section */
#endif

#define CONF_SIZE_INCREMENT  10  /* increment of memory allocation for section/key */
#define CONF_LINE_MAX_LEN    1024

/* user data type define */
typedef struct
{
  char *key_name;
  char *key_value;
}conf_key_t;

typedef struct
{
  char *section_name;
  int  key_count;
  int  current_size; /* section 中当前已分配内存的key数 */
  conf_key_t  *keys;/* 指向key数组*/
#ifdef WITH_HASH
  conf_key_t * hash[CONF_HASH_KEY_SIZE];/* 指向key的指针数组 */
#endif
}conf_section_t;

typedef struct {
  conf_section_t *psection;/* 用于线性查找的数据，指向section数组 */
  int section_count;
  int current_size;  /* 当前已分配内存的section数 */
#ifdef WITH_HASH
  conf_section_t  **m_Hash; /* 用于Hash查找的数据，指向section的指针数组, 指针数组中的指针指向一个节*/
#endif
}cfgdata_t;

/* function prototype  */
#ifdef WITH_HASH
static uint16_t  get_hash_value(const char *sz);
#endif
static void free_all_the_stuff(cfgdata_t *cd);
static int valid(cfgdata_t *cd);

void *init_conf(void)
{
  cfgdata_t *cd;
  
  cd = (cfgdata_t *)xcalloc(1,sizeof(cfgdata_t));
  cd->psection = NULL;
  cd->section_count = 0;
  cd->current_size = 0;
#ifdef WITH_HASH
  cd->m_Hash = (conf_section_t **)xcalloc(CONF_HASH_SECTION_SIZE, sizeof(conf_section_t *));
#endif
  return cd;
}

// 支持配置跨行
static char *getconfline(FILE *fp,char *linebuf,int bufsize)
{
  int len,pos,span;
  char str[CONF_LINE_MAX_LEN+1];

  len = 0;
  pos = 0;
  str[0] = 0;
  linebuf[0] = 0;
  span = 0;
  
  while (fgets(str, CONF_LINE_MAX_LEN + 1, fp))
  {
    len = strlen(str);
    if(str[len-1] == '\n') str[--len] = 0;
    if(str[len-1] == '\r') str[--len] = 0;
    if(!str[0] && span) break;
    if(!str[0]) continue;
    if(str[len-1] == '\\') {
      span = 1;
      str[len-1] = '\0';
      len--;
      if(pos+len >= bufsize) 
        goto EXIT;
      memcpy(linebuf+pos,str,len);
      pos += len;
      linebuf[pos] = 0;
    } else {
      if(pos+len >= bufsize)
        goto EXIT;
      memcpy(linebuf+pos,str,len);
      pos += len;
      linebuf[pos] = 0;
      break;
    }
  }

  if(linebuf[0])
    return linebuf;
  else
    return NULL;
  
EXIT:
   printf("configuration line too long.\n");
   exit(1);
}

int load_conf(void *cfgdata,const char *filename)
{
  FILE *fp;
  char str[CONF_LINE_MAX_LEN*8+1];
  char section[CONF_LINE_MAX_LEN+1];
  int length;
  char *p;
  cfgdata_t *cd = (cfgdata_t *)cfgdata;

  section[0] = '\0';
  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    return -1;
  }

  /* if we already have a file loaded, free it first */
  if (valid(cd))
  {
    free_all_the_stuff(cd);
  }

  while (getconfline(fp,str,CONF_LINE_MAX_LEN*8+1)) // fgets(str, CONF_LINE_MAX_LEN + 1, fp)
  {
    trim(str); /* trim all the blanks or linefeeds(换行) */

    /* skip all comment lines or empty lines */
    if (!str[0] || str[0] == ';' || str[0] == '/' || str[0] == '#')
    {
      continue;
    }

    length = strlen(str);

    /*  check if this is a section line (e.g., [SECTION]) */
    if (str[0] == '[' && str[length - 1] == ']')
    {
      strcpy(section, &str[1]);
      section[length - 2] = 0; /* remove the ] */

      trim(section); /* trim section name after removing [] */
    }
    else
    {
      /* 配置项是名值对，中间用一个或多个空格或垂直制表符间隔 */
      if (((p = strchr(str, ' ')) != NULL) || ((p = strchr(str, '\t')) != NULL) || ((p = strchr(str, '=')) != NULL))
      {
        *(p++) = '\0';
        trim(str); /* str指向key name */
        trim(p);   /* p指向key value */
        set_key_value(cd,section, str, p);
      }
    }
  }

  fclose(fp); /*  load completed; close the file */
  return 0;
}

// 由于使用了strtok()不能用于多线程环境.多线程环境需要修改为strtok_r()
int parse_conf(void *cfgdata,char *conf_str)
{
  char str[CONF_LINE_MAX_LEN+1];
  char section[CONF_LINE_MAX_LEN+1];
  int length;
  char *p;
  char *line;
  cfgdata_t *cd = (cfgdata_t *)cfgdata;

  section[0] = '\0';
  /* if we already have a file loaded, free it first */
  if (valid(cd))
  {
    free_all_the_stuff(cd);
  }

  line = strtok(conf_str, "\n");
  while (line)
  {
    if (strlen(line) <= CONF_LINE_MAX_LEN)
      strcpy(str, line);
    else
      return -1;
    trim(str); /* trim all the blanks or linefeeds(换行) */

    /* skip all comment lines or empty lines */
    if (!str[0] || str[0] == ';' || str[0] == '/' || str[0] == '#')
    {
      line = strtok(NULL, "\n");
      continue;
    }

    length = strlen(str);

    /* check if this is a section line (e.g., [SECTION]) */
    if (str[0] == '[' && str[length - 1] == ']')
    {
      strcpy(section, &str[1]);
      section[length - 2] = 0; /* remove the ] */

      trim(section); /* trim section name after removing [] */
    }
    else
    {
      /* 配置项是名值对，中间用一个或多个空格或垂直制表符间隔 */
      if (((p = strchr(str, '=')) != NULL) || ((p = strchr(str, ' ')) != NULL) || ((p = strchr(str, '\t')) != NULL))
      {
        *(p++) = '\0';
        trim(str); /* str指向key name */
        trim(p);   /* p指向key value */
        set_key_value(cd,section, str, p);
      }
    }

    line = strtok(NULL, "\n");
  }

  return 0;
}

int save_conf(void *cfgdata,const char *filename)
{
  int i,j;
  cfgdata_t *cd = (cfgdata_t *)cfgdata;
  
  FILE *fp = fopen(filename, "w");
  if (fp == NULL)
  {
    return -1;
  }

  for (i = 0; i < cd->section_count; i++)
  {
    fprintf(fp, "[%s]\n", cd->psection[i].section_name); /* write the section name */

    /* write all the keys... */
    for (j = 0; j < cd->psection[i].key_count; j++)
    {
      fprintf(fp, "%s %s\n", cd->psection[i].keys[j].key_name, cd->psection[i].keys[j].key_value);
    }

    fprintf(fp, "\n"); /* add a line feed */
  }

  fclose(fp); /* save completed; close the file */
  return 0;
}

/* 释放配置模块中的内存资源*/
void free_conf(void *cfgdata)
{
  cfgdata_t *cd = (cfgdata_t *)cfgdata;
  
  free_all_the_stuff(cd);
#ifdef WITH_HASH
  free(cd->m_Hash);
#endif
  xfree(cd);
}

const char * get_key_value(void *cfgdata,const char *section, const char *key_name, const char *def)
{
  conf_section_t *psect = NULL;
  int i;
#ifdef WITH_HASH
  uint16_t vhash;
#endif
  cfgdata_t *cd = (cfgdata_t *)cfgdata;

#ifdef WITH_HASH
  /* search for the section name... */
  /* first search in the hash table... */
  vhash = get_hash_value(section) % CONF_HASH_SECTION_SIZE;
  if (cd->m_Hash[vhash] != NULL && strcasecmp(section, cd->m_Hash[vhash]->section_name) == 0)
  {
    psect = cd->m_Hash[vhash]; /* found this section in the hash table */
  }
#endif

  if (psect == NULL)
  {
    /* Not found in the hash table, do a normal search... */
    for (i = 0; i < cd->section_count; i++)
    {
      if (strcasecmp(cd->psection[i].section_name, section) == 0)
      {
        psect = &cd->psection[i];
        break;
      }
    }
  }

  if (psect != NULL)
  {
#ifdef WITH_HASH
    /* section found, search for the key in this section... */
    vhash = get_hash_value(key_name) % CONF_HASH_KEY_SIZE;
    if (psect->hash[vhash] != NULL && strcasecmp(key_name, psect->hash[vhash]->key_name) == 0)
    {
      return psect->hash[vhash]->key_value;
    }
#endif
    /* not found in the hash table, do a normal linear search... */
    for (i = 0; i < psect->key_count; i++)
    {
      if (strcasecmp(psect->keys[i].key_name, key_name) == 0)
      {
        return psect->keys[i].key_value;
      }
    }
  }

  return def; /* key is not found; use default value */
}


/* set the value in conf file */
/* 该函数把<setcion_name,key_name,key_value>写到内存中配置数据
   数据结构中，如果section_name,key_name不存在，则会创建一个节、一个键并设置值*/
int  set_key_value(void *cfgdata,const char *section, const char *key_name, const char *set)
{
  conf_section_t *psect = NULL;
  conf_key_t *pkey = NULL;
  int i;
#ifdef WITH_HASH
  uint16_t vhash;
#endif
  cfgdata_t *cd = (cfgdata_t *)cfgdata;

#ifdef WITH_HASH
  /* search if this section already exists... */
  /* first search in the hash table... */
  vhash = get_hash_value(section) % CONF_HASH_SECTION_SIZE;
  if (cd->m_Hash[vhash] != NULL && strcasecmp(section, cd->m_Hash[vhash]->section_name) == 0)
  {
    psect = cd->m_Hash[vhash]; /* found this section in the hash table */
  }
#endif

  if (psect == NULL)
  {
    /* not found in the hash table, do a normal search... */
    for (i = 0; i < cd->section_count; i++)
    {
      if (strcasecmp(cd->psection[i].section_name, section) == 0)
      {
        psect = &cd->psection[i];
        break;
      }
    }
  }

  /* if this is a new section, try to allocate memory for it */
  if (psect == NULL)
  {
    cd->section_count++;

    /* if we don't have enough room for this new section, try to allocate more memory */
    if (cd->section_count > cd->current_size)
    {
      cd->current_size += CONF_SIZE_INCREMENT;
      if (cd->psection)
      {
        cd->psection = (conf_section_t *)realloc(cd->psection, sizeof(conf_section_t) * cd->current_size);
      }
      else
      {
        cd->psection = (conf_section_t *)malloc(sizeof(conf_section_t) * cd->current_size);
      }

      if (!cd->psection)
      {
        return -1;
      }
    }

    psect = &cd->psection[cd->section_count - 1];
    psect->section_name = strdup(section);
    psect->keys = NULL;
    psect->key_count = 0;
    psect->current_size = 0;
#ifdef WITH_HASH
    memset(psect->hash, 0, sizeof(psect->hash)); /* zero out the hash table */
    /* store this new section in the hash table... */
    vhash = get_hash_value(psect->section_name) % CONF_HASH_SECTION_SIZE;
    cd->m_Hash[vhash] = psect;
#endif
  }

#ifdef WITH_HASH
  /* search if the key is already in the section... */
  vhash = get_hash_value(key_name) % CONF_HASH_KEY_SIZE;
  if (psect->hash[vhash] != NULL && strcasecmp(key_name, psect->hash[vhash]->key_name) == 0)
  {
    pkey = psect->hash[vhash]; /* we have found the key in the hash table */
  }
#endif

  if (pkey == NULL)
  {
    /* key is not found in the hash table, do a normal search... */
    for (i = 0; i < psect->key_count; i++)
    {
      if (strcasecmp(key_name, psect->keys[i].key_name) == 0)
      {
        /* we have found the value */
        pkey = &psect->keys[i];
        break;
      }
    }
  }

  if (pkey != NULL)
  {
    /* this key already exists in the section... */
    free(pkey->key_value);
    pkey->key_value = strdup(set);

    if (pkey->key_value == NULL)
    {
      return -1;
    }
  }
  else
  {
    /*  this is a new key... */
    psect->key_count++;

    /* if we don't have enough room for this new key, try to allocate more memory */
    if (psect->key_count > psect->current_size)
    {
      psect->current_size += CONF_SIZE_INCREMENT;
      if (psect->keys)
      {
        psect->keys = (conf_key_t *)realloc(psect->keys, sizeof(conf_key_t) * psect->current_size);
      }
      else
      {
        psect->keys = (conf_key_t *)malloc(sizeof(conf_key_t) * psect->current_size);
      }

      if (psect->keys == NULL)
      {
        return -1;
      }
    }

    psect->keys[psect->key_count - 1].key_name = strdup(key_name);
    psect->keys[psect->key_count - 1].key_value = strdup(set);

    if (psect->keys[psect->key_count - 1].key_value == NULL || psect->keys[psect->key_count - 1].key_name == NULL)
    {
      return -1;
    }

    trim(psect->keys[psect->key_count - 1].key_value);
    trim(psect->keys[psect->key_count - 1].key_name);

#ifdef WITH_HASH
    /* store this new key in the hash table */
    vhash = get_hash_value(psect->keys[psect->key_count - 1].key_name) % CONF_HASH_KEY_SIZE;
    psect->hash[vhash] = &psect->keys[psect->key_count - 1];
#endif
  }

  return 0;
}


#ifdef WITH_HASH
/**
 * This hash function has been taken from an Article in Dr Dobbs Journal.
 * This is normally a collision-free function, distributing keys evenly.
 * Collision can be avoided by comparing the key itself in last resort.
 */
static uint16_t get_hash_value(const char *sz)
{
  uint16_t hash = 0;
  char a;

  while (*sz)
  {
    /* convert all the characters to be upper case, so that it will be case insensitive */
    a = *sz;
    if (a >= 'a' && a <= 'z')
    {
      a -= 'a' - 'A';
    }

    hash += (uint16_t)a;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    sz++;
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}
#endif

static void free_all_the_stuff(cfgdata_t *cd)
{
  int i, j;
  
  if (cd->psection)
  {
    /*  free all the memory allocated for this conf file */
    for (i = 0; i < cd->section_count; i++)
    {
      /* delete all the keys in this section... */
      for (j = 0; j < cd->psection[i].key_count; j++)
      {
        free(cd->psection[i].keys[j].key_value);
        free(cd->psection[i].keys[j].key_name);
      }

      free(cd->psection[i].keys);
      free(cd->psection[i].section_name);
    }
    free(cd->psection);
    cd->psection = NULL;
  }

#ifdef WITH_HASH
  /* null out the hash table */
  memset(cd->m_Hash, 0, sizeof(conf_section_t *) * CONF_HASH_SECTION_SIZE);
#endif
}

static int valid(cfgdata_t *cd)
{  
  return (cd->psection != NULL);
}

// min=max=0 表示大小没有限制。notfound也是默认值
int getconfint(void *cfgdata,const char *section, const char *key, int isnull, int notfound, int min, int max)
{
  const char *cp;
  int v;

  cp = get_key_value(cfgdata,section, key, NULL);
  if (!cp && !isnull)
  {
    printf("Config Item: [%s] %s: not set\n", section, key);
    exit(1);
  }
  else if (!cp && isnull)
    return notfound;
  else
  {
    v = atoi(cp);
    if (max && (v < min || v > max))
    {
      printf("Config Item: [%s] %s: set error\n", section, key);
      exit(1);
    }

    return v;
  }
}

double getconfloat(void *cfgdata,const char *section, const char *key, int isnull, double notfound, double min, double max)
{
  const char *cp;
  double v;

  cp = get_key_value(cfgdata,section, key, NULL);
  if (!cp && !isnull)
  {
    printf("Config Item: [%s] %s: not set\n", section, key);
    exit(1);
  }
  else if (!cp && isnull)
    return notfound;
  else
  {
    v = atof(cp);
    if (max && (v < min || v > max))
    {
      printf("Config Item: [%s] %s: set error\n", section, key);
      exit(1);
    }

    return v;
  }
}

// @return: need free.
char *getconfstring(void *cfgdata,const char *section, const char *key, int isnull, char *notfound,
                          char **values, int *ivalues, int valnum, int *retival)
{
  const char *cp;
  int i;

  cp = get_key_value(cfgdata,section, key, NULL);
  if (!cp && !isnull)
  {
    printf("Config Item: [%s] %s: not set\n", section, key);
    exit(1);
  }
  else if (!cp && isnull)
  {
    if(notfound) { 
      cp = notfound; 
      goto process;
    } else
      return notfound;
  }
  else
  {
  process:
    if (valnum == 0)
      return xstrdup(cp);

    for (i = 0;i < valnum;++i)
    {
      if (strcasecmp(cp, values[i]) == 0)
        break;
    }

    if (i == valnum)
    {
      printf("Config Item: [%s] %s: set error\n", section, key);
      exit(1);
    }

    if (ivalues && retival)
    {
      *retival = ivalues[i];
      return NULL;
    }
    else if (ivalues && !retival)
    {
      printf("return value variable pointer isn't set.\n");
      exit(1);
    }
    else
      return xstrdup(cp);
  }
}

// @return: need free.
char *getconfmulti(void *cfgdata,const char *section, const char *key, int isnull, char *notfound,
                         const char *sepstr, char ***values, int *valnum)
{
  const char *cp;
  char *v, **vv;
  int i, n, len;

  cp = get_key_value(cfgdata,section, key, NULL);
  if (!cp && !isnull)
  {
    printf("Config Item: [%s] %s: not set\n", section, key);
    exit(1);
  }
  else if (!cp && isnull) 
  {
    if(notfound) {
      cp = notfound;
      goto process;
    }
    else {
      *values = NULL;
      *valnum = 0;
      return notfound;
    }
  }
  else
  {
  process:
    len = strlen(cp);
    v = xmalloc(len + 1);
    strcpy(v,cp);
    strsqueeze(v);
    //trimid(cp, v, 1); 注意: 只能用于ascii和gbk
    n = strfragnum(v, sepstr);
    vv = (char **)xcalloc(n, sizeof(char *));
    split(v, sepstr, vv, n);
    for (i = 0;i < n;++i)
    {
      trim(vv[i]);
      if (*vv[i] == 0)
      {
        printf("Config Item: [%s] %s: set error\n", section, key);
        xfree(v);
        xfree(vv);
        exit(1);
      }
    }
    *values = vv;
    *valnum = n;
    return v;
  }
}

// @return: need free.
char *getconfdir(void *cfgdata,const char *section, const char *key, int isnull, char *notfound)
{
  const char *cp;
  char *v;

  cp = get_key_value(cfgdata,section, key, NULL);
  if (!cp && !isnull)
  {
    printf("Config Item: [%s] %s: not set\n", section, key);
    exit(1);
  }
  else if (!cp && isnull)
  {
    if(notfound) {
      cp = notfound;
      goto process;
    }
    else
     return notfound;
  }
  else
  {
  process:
    v = xstrdup(cp);
    if(v[strlen(v)-1] == '/')
       v[strlen(v)-1] = 0;
    
    return v;
  }
}

// @return: need free.
char *getconfpath(void *cfgdata,const char *section, const char *key, int isnull, char *notfound,char *home,char *dir)
{
  const char *cp;
  char *v,path[1024];

  cp = get_key_value(cfgdata,section, key, NULL);
  if (!cp && !isnull)
  {
    printf("Config Item: [%s] %s: not set\n", section, key);
    exit(1);
  }
  else if (!cp && isnull) {
    if(notfound) {
      cp = notfound;
      goto process;
    }
    else
      return notfound;
  }
  else
  {
  process:
    if(cp[0] != '/')
    {
      path[0] = 0;
      if (home) { strcpy(path, home); strcat(path,"/");}
      if (dir)  { strcat(path, dir); strcat(path,"/");}
      strcat(path, cp);
      v = xstrdup(path);
    }
    else
    {
      v = xstrdup(cp);
    }
    
    return v;
  }
}

void get_conf(void *cfgdata,conf_t *conf,int confnum)
{
  int i;

  for(i=0;i<confnum;i++)
  {
    switch(conf[i].type)
    {
      case CONF_INT:
        *(int *)conf[i].r = getconfint(cfgdata,conf[i].section,conf[i].key,conf[i].isnull,conf[i].defval?atoi(conf[i].defval):0,
                                       (int)conf[i].p1,(int)conf[i].p2);
        break;
      case CONF_STR:
        if((int)conf[i].p3) // 返回整数
          getconfstring(cfgdata,conf[i].section,conf[i].key,conf[i].isnull,conf[i].defval,(char **)conf[i].p1,(int *)conf[i].p3,(int)conf[i].p2,(int *)conf[i].r);
        else  // 返回字符串
          *(char **)conf[i].r = getconfstring(cfgdata,conf[i].section,conf[i].key,conf[i].isnull,conf[i].defval,(char **)conf[i].p1,NULL,(int)conf[i].p2,NULL);
        break;
      case CONF_FLOAT:
        *(double *)conf[i].r = getconfloat(cfgdata,conf[i].section,conf[i].key,conf[i].isnull,conf[i].defval?atof(conf[i].defval):0.0,
                               (double)(int)conf[i].p1,(double)(int)conf[i].p2); 
        break;
      case CONF_MULTI:
        *(char **)conf[i].r = getconfmulti(cfgdata,conf[i].section,conf[i].key,conf[i].isnull,conf[i].defval,
                                           (char *)(conf[i].p2),(char ***)(conf[i].p1),(int *)(conf[i].p3));
        break;
      case CONF_DIR:
        *(char **)conf[i].r = getconfdir(cfgdata,conf[i].section,conf[i].key,conf[i].isnull,conf[i].defval);
        break;
      case CONF_PATH:
        *(char **)conf[i].r = getconfpath(cfgdata,conf[i].section,conf[i].key,conf[i].isnull,conf[i].defval,
                                          (char *)conf[i].p1,(char *)conf[i].p2);
        break;
    }
  }
}



#ifdef TEST_CONF
int  main()
{
  char **value;
  int valnum;
  int i;
  void *cfgdata;
  
  cfgdata = init_conf();
  if(load_conf(cfgdata,"../../etc/dp.conf") < 0)
  {
    printf("load configuration file failure.\n");
    return -1;
  }

  const char *weekday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; 

  printf("[test] int: %d\n",getconfint(cfgdata,"test","int",0,1,2,6));
  printf("[test] float: %.2f\n",getconfloat(cfgdata,"test","float",0,1.0,1.0,999.99));
  printf("[test] string: %s\n",getconfstring(cfgdata,"test","string",0,"not fount",weekday,NULL,7,NULL));
  printf("[test] multi-value: \n");
  getconfmulti(cfgdata,"test","multi",0,"not found"," ",&value,&valnum);
  for(i=0;i<valnum;i++)
    printf("%s ",value[i]);

  printf("\n");
  free_conf(cfgdata);
}
#endif

