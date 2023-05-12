/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"
#include "utils.h"
#include "xmalloc.h"
#include "xstring.h"

void *crypt_init(const char *pwdfile, int pwdmax)
{
  aes_context *aesctx;
  char *pwd;
  int pwdlen;
  char key[33];//加密密码,最多32字符，256bits

  if (!pwdfile || (pwdmax != 16 && pwdmax != 24 && pwdmax != 32))
    return NULL;

  pwd = loadfile(pwdfile, NULL);
  if (!pwd)
    return NULL;
  
  trim(pwd);
  pwdlen = strlen(pwd);
  if (pwdlen > pwdmax)
  {
    xfree(pwd);
    return NULL;
  }
  memset(key, 0, 33);
  strcpy(key, pwd);
  xfree(pwd);

  aesctx = (aes_context *)xcalloc(1, sizeof(aes_context));
  if (aes_set_key(aesctx, (uint8 *)key, pwdmax*8))
  {
    xfree(aesctx);
    return NULL;
  }

  return aesctx;
}

void crypt_free(void *ctx)
{
  aes_context *aesctx = (aes_context *)ctx;

  xfree(aesctx);
}

// @return: need free.
char * crypt_encode(void *ctx, const char *input, int inlen, int *outlen)
{
  aes_context *aesctx;
  int proclen, rmdlen;
  char *cryrec;
  int cryreclen;
  char block[16];

  aesctx = (aes_context *)ctx;

  if (!ctx || !input || !inlen || !outlen)
    return NULL;

  cryrec = (char *)xcalloc(1, inlen + 16);
  proclen = 0;
  rmdlen = inlen;

  while (rmdlen >= 16)
  {
    aes_encrypt(aesctx, (uint8_t *)input + proclen, (uint8_t *)cryrec + proclen);
    proclen += 16;
    rmdlen -= 16;
  }

  //采用pkcs5填充方式
  //对不够16字节的部分进行加密，填充剩余字节数
  //对于正好是16的整数倍的，最后16个字节全部填充为16
  if (rmdlen >= 0 && rmdlen < 16)
  {
    memcpy(block, input + proclen, rmdlen);
    memset(block + rmdlen, 16 - rmdlen, 16 - rmdlen);
    aes_encrypt(aesctx, (uint8_t *)block, (uint8_t *)cryrec + proclen);
    proclen += 16;
    rmdlen = 0;
  }

  cryreclen = proclen;
  *outlen = cryreclen;
  return cryrec;
}

// @return: need free.
char * crypt_decode(void *ctx, const char *input, int inlen, int *outlen)
{
  aes_context *aesctx;
  int proclen, rmdlen;
  char *decryrec;

  aesctx = (aes_context *)ctx;

  if (!ctx || !input || !inlen || !outlen)
    return NULL;

  decryrec = (char *)xcalloc(1, inlen + 1);
  proclen = 0;
  rmdlen = inlen;

  while (rmdlen >= 16)
  {
    aes_decrypt(aesctx, (uint8_t *)input + proclen, (uint8_t *)decryrec + proclen);
    proclen += 16;
    rmdlen -= 16;
  }

  rmdlen = 16 - decryrec[proclen-1];
  *outlen = proclen - 16  + rmdlen;
  decryrec[*outlen] = 0;
  return decryrec;
}

#ifdef TEST_CRYPT
#include "crypt.h"
int main()
{
  void *aesctx;
  char *s = "hello,world!";
  char *r,*t;
  int len;
  
  aesctx = crypt_init(CRYPT_PWD_FILE,CRYPT_PWD_LEN16);
  if(!aesctx) {
    printf("call crypt_init() error.\n");
    return 1;
  }

  r = crypt_encode(aesctx,s,strlen(s),&len);
  t = crypt_decode(aesctx,r,len,&len);
  printf("%s\n",t);
  xfree(r);
  xfree(t);

  crypt_free(aesctx);
  return 0;
}

#endif

