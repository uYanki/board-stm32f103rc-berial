#ifndef CONF_H
#define CONF_H

enum
{
  CONF_INT = 1,
  CONF_STR,
  CONF_FLOAT,
  CONF_MULTI,
  CONF_DIR,
  CONF_PATH
};

/*
 * 配置项分为两类: 必选，可选。开发者需要知道哪些是必选的，哪些是可选的。
 * 配置项取值来源只有两个: 要么由用户指定，要么采用默认值。
 * 必选配置项必须有值,可选配置项则可有可无。
 * isnull仅仅指的是这个配置项是否一定要求用户指定取值。只用于没有默认值的必选选项。
 * defval指的是如果用户没有指定，则采用默认值。
*/


typedef struct
{
  char *section;
  char *key;
  int  type;
  int isnull; //指明该配置项的值是否必须由用户指定，用于没有默认值的必选选项。
  char *defval;
  void *p1;  //int:int min;multi:char ***mulval;string:char **vals; path: char*home;
  void *p2;  //int:int max;multi:char *sepstr;  string:int valnum;  path: char*dir;
  void *p3;  //int:NULL;   multi:int  *mvnum;   string:int *ivals;  path: NULL
  void *r;
}conf_t;


#ifdef __cplusplus
extern "C"
{
#endif
  void *init_conf(void);
  int load_conf(void *cfgdata,const char *filename);
  int parse_conf(void *cfgdata,char *conf_str);
  int save_conf(void *cfgdata,const char *filename);
  void get_conf(void *cfgdata,conf_t *conf,int confnum);
  void free_conf(void *cfgdata);

  const char *get_key_value(void *cfgdata,const char *section, const char *key_name, const char *def);
  int set_key_value(void *cfgdata,const char *section, const char *key_name, const char *set);

  
  int getconfint(void *cfgdata,const char *section, const char *key, int isnull, int notfound, int min, int max);
  double getconfloat(void *cfgdata,const char *section, const char *key, int isnull, double notfound, double min, double max);
  char *getconfstring(void *cfgdata,const char *section, const char *key, int isnull, char *notfound,
                            char **values, int *ivalues, int valnum, int *retival);
  char *getconfmulti(void *cfgdata,const char *section, const char *key, int isnull, char *notfound,
                     const char *sepstr, char ***values, int *valnum);
  char *getconfdir(void *cfgdata,const char *section, const char *key, int isnull, char *notfound);
  char *getconfpath(void *cfgdata,const char *section, const char *key, int isnull, char *notfound,char *home,char *dir);

#ifdef __cplusplus
}
#endif

#endif

