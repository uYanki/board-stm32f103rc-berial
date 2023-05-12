/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "xmalloc.h"
#include "xstring.h"
#include "mwm.h"
#include "hbm.h"
#include "dir.h"
#include "utils.h"
#include "crypt.h"
#include "comn.h"
#include "record.h"

#define BF_BUFFER_SIZE (32*1024*1024) //default: 32M
#define BF_BUFFER_MIN  (2*1024*1024)  //min: 2M

static int match(void *id, int index, void *data);

struct _file
{
  FILE *fp;
  int file_size;  //待读的文件内容大小
  int file_pos;   //已经读取的位置
  int file_rem;

  char *buffer;
  int bufsize;
  
  int codetype;
  char *recbeg;
  char *recend;

  int buf_data_size;//指buffer有多少数据，包括读取和未读取的
  int buf_data_pos;
  int buf_data_rem;//remainder，指buffer中还有多少数据未读取

  int buffer_data_offset;//buffer中数据在文件中的偏移量= file_pos - buffer_data_size
};

void *record_fromfile_init(const char *path,const char *recbeg, const char *recend, int bufsize,int codetype)
{
  struct _file *p;
  FILE *fp;
  int file_size;
  int file_pos;

  fp = fopen(path,"rb");
  if(!fp)
    return NULL;

  file_size = (int)getfilesize(fp);
  file_pos = 0;

  if(bufsize <= 0)
    bufsize = BF_BUFFER_SIZE;
  else if(bufsize < BF_BUFFER_MIN)
    bufsize = BF_BUFFER_MIN;
  
  p = (struct _file *)xcalloc(1, sizeof(struct _file));

  p->fp = fp;
  p->file_size = file_size;
  p->file_pos = file_pos;
  p->file_rem = p->file_size;

  p->buffer = xcalloc(1, bufsize + 1);
  p->bufsize = bufsize;
  p->codetype = codetype;
  p->recbeg = xstrdup(recbeg);
  p->recend = xstrdup(recend);
  
  p->buf_data_size = 0;
  p->buf_data_pos = 0;
  p->buf_data_rem = 0;

  p->buffer_data_offset = 0;

  return p;
}

void record_fromfile_free(void *datafile)
{
  struct _file *p = (struct _file *)datafile;

  fclose(p->fp);
  xfree(p->buffer);
  xfree(p->recbeg);
  xfree(p->recend);
  xfree(p);
}

//返回一个记录和记录的大小，注意，记录最后不是以'\0'结尾。
const char * record_fromfile_get(void *datafile, int *record_len)
{
  int n, rec_len,bufsize,codetype;
  int rec_end_pat_len; 
  char *rec_beg, *rec_end;
  struct _file *in_argp = (struct _file *)datafile;
  const char *rec_beg_pat,*rec_end_pat;

  if (!in_argp)
    return NULL;

  if (in_argp->file_rem == 0 && in_argp->buf_data_rem == 0) //end of file
    return NULL;

  bufsize = in_argp->bufsize;
  codetype = in_argp->codetype;
  rec_beg_pat = in_argp->recbeg;
  rec_end_pat = in_argp->recend;

  if (in_argp->buf_data_rem == 0) // buffer is empty
  {
    n = fread(in_argp->buffer, 1, bufsize, in_argp->fp);
    if (n != bufsize && !feof(in_argp->fp))
      return NULL;
    *(in_argp->buffer + n)  = '\0';

    in_argp->file_rem -= n;
    in_argp->file_pos += n;
    in_argp->buf_data_size = n;
    in_argp->buf_data_pos = 0;
    in_argp->buf_data_rem = n;

    in_argp->buffer_data_offset = in_argp->file_pos - in_argp->buf_data_size;

  }

  rec_end_pat_len = strlen(rec_end_pat);
  rec_beg = in_argp->buffer + in_argp->buf_data_pos;
  rec_end = (char *)hbm_patternmatch((uint8_t *)rec_beg, in_argp->buf_data_rem, (uint8_t *)rec_end_pat, rec_end_pat_len, 0,codetype);
  if (!rec_end) //buffer data not a record,need read remainder data from file
  {
  read_again:
    memcpy(in_argp->buffer, rec_beg, in_argp->buf_data_rem); // 注意: 最好用memmove，防止源地址和目的地址覆盖
    n = fread(in_argp->buffer + in_argp->buf_data_rem, 1, bufsize - in_argp->buf_data_rem, in_argp->fp);
    if (n != (bufsize - in_argp->buf_data_rem) && !feof(in_argp->fp))
      return NULL;
    *(in_argp->buffer + in_argp->buf_data_rem + n)  = '\0';

    in_argp->file_rem -= n;
    in_argp->file_pos += n;
    in_argp->buf_data_size = in_argp->buf_data_rem + n;
    in_argp->buf_data_pos = 0;
    in_argp->buf_data_rem = in_argp->buf_data_rem + n;
    in_argp->buffer_data_offset  = in_argp->file_pos - in_argp->buf_data_size;


    rec_beg = in_argp->buffer + in_argp->buf_data_pos;
    rec_end = (char *)hbm_patternmatch((uint8_t *)rec_beg, in_argp->buf_data_rem, (uint8_t *)rec_end_pat, rec_end_pat_len, 0,codetype);
    if (!rec_end)
    {
      if (in_argp->file_rem == 0)
        return NULL;
      else
        goto read_again;
    }
  }
  rec_end += rec_end_pat_len;
  rec_len =  rec_end - rec_beg;

  in_argp->buf_data_pos += rec_len;
  in_argp->buf_data_rem -= rec_len;

  *record_len = rec_len;
  return rec_beg;
}

///////////////////////////////////////////////////////////
struct _dir
{
  //config
  char *path;
  char *prefix;
  int prefixnum;
  char **prefixpp;
  char *suffix;
  int suffixnum;
  char **suffixpp;
  int isremove;
  char *rename;

  int codetype;
  int bufsize;
  char *recbeg;
  char *recend;

  //internal
  void *dp; //dir pointer
  void *df; //data file
  char curfile[MAX_PATH_LEN+1];

  //event handler
  void (*bof)(void *,void *);  // when begin of file,call function.
  void (*eof)(void *,void *);  // when end of file,call function.
  void *bofarg;
  void *eofarg;

  //statis
  int filenum;
  int recnum;
  int filerecnum; 

  //error
  int errno;
  char errmsg[256];
};

void *record_fromdir_init(const char *path,const char *prefix, const char *suffix,
                                  const char *delim, int isremove,const char*rename,
                                  const char *recbeg, const char *recend, int bufsize,int codetype)
{
  struct _dir *dd;
  char *t,**tt;
  int i,num;

  if(!path || !recbeg || !recend)
    return NULL;

  if((prefix || suffix) && !delim)
    return NULL;

  dd = (struct _dir *)xcalloc(1,sizeof(struct _dir));
  
  dd->path = xstrdup(path);
  if(dd->path[strlen(path)-1] == '/')
    dd->path[strlen(path)-1] = 0;

  dd->dp = _opendir(dd->path);
  if(!dd->dp)
  {
    xfree(dd->path);
    xfree(dd);
    return NULL;
  }

  if(prefix)
  {
    t = xstrdup(prefix);
    num = strfragnum(prefix,delim);
    tt = (char **)xcalloc(num,sizeof(char *));
    split(t,delim,tt,num);
    for(i=0;i<num;i++)
    {
      trim(tt[i]);
      if(*tt[i] == 0)
      {
        xfree(dd->path);
        _closedir(dd->dp);
        xfree(tt);
        xfree(t);
        xfree(dd);
        return NULL;
      }
    }

    dd->prefix = t;
    dd->prefixnum = num;
    dd->prefixpp = tt;
  }

  if(suffix)
  {
    t = xstrdup(suffix);
    num = strfragnum(suffix,delim);
    tt = (char **)xcalloc(num,sizeof(char *));
    split(t,delim,tt,num);
    for(i=0;i<num;i++)
    {
      trim(tt[i]);
      if(*tt[i] == 0)
      {
        xfree(dd->path);
        _closedir(dd->dp);
        xfree(tt);
        xfree(t);
        xfree(dd->prefix);
        xfree(dd->prefixpp);
        xfree(dd);
        return NULL;
      }
    }

    dd->suffix = t;
    dd->suffixnum = num;
    dd->suffixpp = tt;
  }

  dd->isremove = isremove;
  dd->codetype = codetype;
  dd->bufsize = bufsize;
  dd->recbeg = xstrdup(recbeg);
  dd->recend = xstrdup(recend);
  dd->rename = xstrdup(rename);

  return dd;
}

void record_fromdir_bof(void *datadir,void *bof,void *arg)
{
  struct _dir *dd = (struct _dir *)datadir;

  dd->bof = bof;
  dd->bofarg = arg;
}

void record_fromdir_eof(void *datadir,void *eof,void *arg)
{
  struct _dir *dd = (struct _dir *)datadir;

  dd->eof = eof;
  dd->eofarg = arg;
}

void record_fromdir_free(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;
  
  xfree(dd->path);
  xfree(dd->prefix);
  xfree(dd->prefixpp);
  xfree(dd->suffix);
  xfree(dd->suffixpp);
  xfree(dd->recbeg);
  xfree(dd->recend);
  xfree(dd->rename);

  _closedir(dd->dp);
  
  if(dd->df)
    record_fromfile_free(dd->df);

  xfree(dd);
}

void record_fromdir_rewind(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;

  _rewinddir(dd->dp);
}

void * record_fromdir_reopen(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;

  _closedir(dd->dp);
  dd->dp = _opendir(dd->path);
  if(!dd->dp)
    return NULL;

  return dd->dp;
}

//返回一个记录和记录的大小，注意，记录最后不是以'\0'结尾。
const char *record_fromdir_get(void *datadir,int *reclen)
{
  struct _dir *dd = (struct _dir *)datadir;
  const char *filename,*record;
  char path[MAX_PATH_LEN+1];

  if(!dd->df)
  {
_NEXTFILE:
    filename = _readdirft(dd->dp,dd->prefixpp,dd->prefixnum,dd->suffixpp,dd->suffixnum);
    if(!filename)
    {
      dd->errno = EEOD;
      return NULL;
    }

    snprintf(dd->curfile,MAX_PATH_LEN+1,"%s/%s",dd->path,filename);
    dd->df = record_fromfile_init(dd->curfile,dd->recbeg,dd->recend,dd->bufsize,dd->codetype);
    if(!dd->df)
    {
      dd->errno = ENOFILE;
      snprintf(dd->errmsg,256,"can't open file: %s\n",dd->curfile);
      return NULL;
    }

    dd->filenum++;
    dd->filerecnum = 0;
    if(dd->bof)
      dd->bof(datadir,dd->bofarg);
  }

  record = record_fromfile_get(dd->df,reclen);
  if(!record)
  {
    record_fromfile_free(dd->df);
    dd->df = NULL;
    
    if(dd->isremove)
    {
      if(remove(dd->curfile) < 0)
      {
        dd->errno = EREMOVE;
        snprintf(dd->errmsg,256,"remove failure: %s\n",dd->curfile);
        return NULL;
      }
    }
    else if(dd->rename)
    {
      snprintf(path,MAX_PATH_LEN+1,"%s%s",dd->curfile,dd->rename);
      if(rename(dd->curfile,path) < 0)
      {
        dd->errno = ERENAME;
        snprintf(dd->errmsg,256,"rename failure: %s -> %s\n",dd->curfile,path);
        return NULL;
      }
    }

    if(dd->eof)
      dd->eof(datadir,dd->eofarg);
    
    goto _NEXTFILE;
  }

  dd->recnum++;
  dd->filerecnum++;
  return record;
}

const char *record_fromdir_curfile(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;

  return dd->curfile;
}

int record_fromdir_filenum(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;

  return dd->filenum;
}

int record_fromdir_recnum(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;

  return dd->recnum;
}

int record_fromdir_filerecnum(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;

  return dd->filerecnum;
}


int record_fromdir_errno(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;

  return dd->errno;
}

const char *record_fromdir_errmsg(void *datadir)
{
  struct _dir *dd = (struct _dir *)datadir;

  return dd->errmsg;
}


///////////////////////////////////////////////////////////
//记录解析...

struct _field
{
  char *begtag; //<tag>
  char *endtag; //</tag>
  char *name;
  char *value;  //need't to free. if has op,need free. 注意该value末尾不是'\0'
  int vlen;
  int zop;
  int cop;
};

struct _recmode
{
  struct _field *field;
  int field_num;
  int codetype;
  void *aesctx;
};

struct match_arg
{
  char *rec_beg;
  char *rec_end;
  struct _recmode *recmode;
};

// fieldname string: name,name.cz,name.xd,... 注意: tag name是大小写敏感的。
void *record_parse_init(const char *fieldnames,int codetype,
                                const char *pwdfile, int pwdmax)
{
  int i;
  struct _recmode *recmode;
  char *fnames,**fn;
  char *t;
  
  recmode = (struct _recmode *)xcalloc(1,sizeof(struct _recmode));
  recmode->codetype = codetype;
  recmode->field_num = strfragnum(fieldnames,",");
  if(recmode->field_num == 0)
  {
    xfree(recmode);
    return NULL;
  }
  fnames = xstrdup(fieldnames);
  fn = (char **)xcalloc(recmode->field_num,sizeof(char *));
  split(fnames,",",fn,recmode->field_num);
  for(i=0;i<recmode->field_num;i++)
  {
    trim(fn[i]);
    if(*fn[i] == 0)
    {
      xfree(fn);
      xfree(fnames);
      xfree(recmode);
      return NULL;
    }
  }

  if(pwdfile)
  {
    recmode->aesctx = crypt_init(pwdfile,pwdmax);
    if(!recmode->aesctx)
    {
      xfree(fn);
      xfree(fnames);
      xfree(recmode);
      return NULL;
    }
  }
  
  recmode->field = (struct _field *)xcalloc(recmode->field_num,sizeof(struct _field));
  for(i=0;i<recmode->field_num;i++)
  {
    t = strchr(fn[i],'.');
    if(t)
    {
      *t = 0;
      t++;
      if(!*t) goto _freemem;
      if(*t)
      {
        if(*t == FIELD_CRYPT) recmode->field[i].cop = FIELD_CRYPT;
        else if(*t == FIELD_DECRYPT) recmode->field[i].cop = FIELD_DECRYPT;
        else if(*t == FIELD_ZIP) recmode->field[i].zop = FIELD_ZIP;
        else if(*t == FIELD_UNZIP) recmode->field[i].zop = FIELD_UNZIP;
        else goto _freemem;
      }
      
      t++;
      if(*t)
      {
        if(*t == FIELD_CRYPT) recmode->field[i].cop = FIELD_CRYPT;
        else if(*t == FIELD_DECRYPT) recmode->field[i].cop = FIELD_DECRYPT;
        else if(*t == FIELD_ZIP) recmode->field[i].zop = FIELD_ZIP;
        else if(*t == FIELD_UNZIP) recmode->field[i].zop = FIELD_UNZIP;
        else goto _freemem;
      }

      if((recmode->field[i].cop == FIELD_CRYPT || recmode->field[i].cop == FIELD_DECRYPT) && !recmode->aesctx)
         goto _freemem;

      if(recmode->field[i].cop && recmode->field[i].zop)
      {
        if((recmode->field[i].cop == FIELD_CRYPT && recmode->field[i].zop == FIELD_UNZIP)
         ||(recmode->field[i].cop == FIELD_DECRYPT && recmode->field[i].zop == FIELD_ZIP))
          goto _freemem;
      }
    }
    
    recmode->field[i].begtag = (char *)xcalloc(1,strlen(fn[i])+2+1);
    recmode->field[i].endtag = (char *)xcalloc(1,strlen(fn[i])+3+1);
    sprintf(recmode->field[i].begtag,"<%s>",fn[i]);
    sprintf(recmode->field[i].endtag,"</%s>",fn[i]);
    recmode->field[i].name = xstrdup(fn[i]);
    recmode->field[i].value = NULL;
    recmode->field[i].vlen = 0;
  }

  xfree(fn);
  xfree(fnames);
  
  return recmode;
  
_freemem:
  xfree(fn);
  xfree(fnames);
  for(i=0;i<recmode->field_num;i++)
  {
    xfree(recmode->field[i].begtag);
    xfree(recmode->field[i].endtag);
    xfree(recmode->field[i].name);
  }

  xfree(recmode->field);
  if(recmode->aesctx)
    crypt_free(recmode->aesctx);
  xfree(recmode);
  return NULL;
}

void record_parse_free(void *rec_mode)
{
  int i;
  struct _recmode *recmode = (struct _recmode *)rec_mode;

  for(i=0;i<recmode->field_num;i++)
  {
    xfree(recmode->field[i].begtag);
    xfree(recmode->field[i].endtag);
    xfree(recmode->field[i].name);
  }

  xfree(recmode->field);
  if(recmode->aesctx)
    crypt_free(recmode->aesctx);
  xfree(recmode);
}

// 注意返回值不一定是字符串，即末尾不一定为'\0',是否需要释放由free决定。
char *record_parse_getbyname(void *rec_mode,const char *fieldname,int *fieldlen,int *free)
{
  int i,len,flen;
  char *p,*field;
  struct _recmode *recmode = (struct _recmode *)rec_mode;
  
  for(i=0;i<recmode->field_num;i++)
  {
    if(strcmp(recmode->field[i].name,fieldname) == 0)
    {
      field = recmode->field[i].value;
      flen = recmode->field[i].vlen;
      *free = 0;

      if(!field || !flen)
        return NULL;
      
      if(recmode->field[i].zop == FIELD_ZIP)
      {
        p = (char *)compressx(field,flen,&len);
        if(!p)
          return NULL;
        field = p;
        flen = len;
        *free = 1;
      }

      if(recmode->field[i].cop == FIELD_CRYPT)
      {
        p = crypt_encode(recmode->aesctx,field,flen,&len);
        if(*free)
          xfree(field);
        if(!p) 
          return NULL;
        field = p;
        flen = len;
        *free = 1;
      }

      if(recmode->field[i].cop == FIELD_DECRYPT)
      {
        p = crypt_decode(recmode->aesctx,field,flen,&len);
        if(*free)
          xfree(field);
        if(!p)
          return NULL;
        field = p;
        flen = len;
        *free = 1;
      }

      if(recmode->field[i].zop == FIELD_UNZIP)
      {
        p = uncompressx(field,flen,&len);
        if(*free)
          xfree(field);
        if(!p)
          return NULL;
        field = p;
        flen = len;
        *free = 1;
      }
      
      *fieldlen = flen;
      return field;
    }
  }

  return NULL;
}

// 注意返回值不一定是字符串，即末尾不一定为'\0',是否需要释放由free决定。
char *record_parse_getbyid(void *rec_mode,int fieldid,int *fieldlen,char **fname,int *free)
{
  int flen,len;
  char *field,*p;
  struct _recmode *recmode = (struct _recmode *)rec_mode;

  if(fieldid < 0 || fieldid >= recmode->field_num)
    return NULL;

  field = recmode->field[fieldid].value;
  flen = recmode->field[fieldid].vlen;
  *free = 0;
  *fname = recmode->field[fieldid].name;

  if(!field || !flen)
    return NULL;
  
  if(recmode->field[fieldid].zop == FIELD_ZIP)
  {
    p = (char *)compressx(field,flen,&len);
    if(!p)
      return NULL;
    field = p;
    flen = len;
    *free = 1;
  }

  if(recmode->field[fieldid].cop == FIELD_CRYPT)
  {
    p = crypt_encode(recmode->aesctx,field,flen,&len);
    if(*free)
      xfree(field);
    if(!p) 
      return NULL;
    field = p;
    flen = len;
    *free = 1;
  }

  if(recmode->field[fieldid].cop == FIELD_DECRYPT)
  {
    p = crypt_decode(recmode->aesctx,field,flen,&len);
    if(*free)
      xfree(field);
    if(!p)
      return NULL;
    field = p;
    flen = len;
    *free = 1;
  }

  if(recmode->field[fieldid].zop == FIELD_UNZIP)
  {
    p = uncompressx(field,flen,&len);
    if(*free)
      xfree(field);
    if(!p)
      return NULL;
    field = p;
    flen = len;
    *free = 1;
  }

  *fieldlen = flen;
  return field;
}

int record_parse_getfieldnum(void *rec_mode)
{
  struct _recmode *recmode = (struct _recmode *)rec_mode;

  return recmode->field_num;
}

int record_parse(void *rec_mode,const char *record, int record_len)
{
  void *pv;
  struct match_arg mat_arg;
  int i, rec_len;
  struct _recmode *recmode = (struct _recmode *)rec_mode;

  //清空记录
  for(i=0;i<recmode->field_num;i++)
  {
    recmode->field[i].value = NULL;
    recmode->field[i].vlen = 0;
  }

  rec_len = record_len;
  mat_arg.rec_beg = (char *)record;
  mat_arg.rec_end = (char *)record + rec_len;
  mat_arg.recmode = recmode;

  pv = mwmNew(0);
  for (i = 0;i < recmode->field_num;i++)
  {
    mwmAddPatternEx(pv, (uint8_t *)(recmode->field[i].begtag), strlen(recmode->field[i].begtag), 0, 0, 0, (void *)i, 3000);
  }
  mwmPrepPatterns(pv);
  mwmSearch(pv, (uint8_t *)record, rec_len, match, &mat_arg);
  mwmFree(pv);

  return 0;
}

static int match(void *id, int index, void *data)
{
  char *pbeg, *pend;
  int len, fid;
  struct match_arg *mat_arg_p;

  fid = (int)id;
  mat_arg_p = (struct match_arg *)data;
  len = strlen(mat_arg_p->recmode->field[fid].begtag);
  pbeg = mat_arg_p->rec_beg + index + len;
  pend = (char *)hbm_patternmatch((uint8_t *)pbeg, mat_arg_p->rec_end - pbeg, (uint8_t *)mat_arg_p->recmode->field[fid].endtag, strlen(mat_arg_p->recmode->field[fid].endtag), 0,mat_arg_p->recmode->codetype);
  if (!pend)
  {
    return 0;
  }

  len = pend - pbeg;
  //mat_arg_p->recmode->field[fid].value = xcalloc(1, len + 1);
  //memcpy(mat_arg_p->recmode->field[fid].value, pbeg, len);
  mat_arg_p->recmode->field[fid].vlen = len;
  mat_arg_p->recmode->field[fid].value = pbeg;

  return 0;
}

#ifdef TEST_RECORD
#include "charset.h"
#include "record.h"
int main(int argc,char *argv[])
{

}

#endif

