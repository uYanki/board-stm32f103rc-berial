/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef CRYPT_H
#define CRYPT_H

#define CRYPT_PWD_LEN16  16
#define CRYPT_PWD_LEN24  24
#define CRYPT_PWD_LEN32  32
#define CRYPT_PWD_FILE   "/tmp/crypt_pwd"

#ifdef __cplusplus
extern "C"
{
#endif

  void *crypt_init(const char *pwdfile, int pwdmax);
  void crypt_free(void *ctx);
  char * crypt_encode(void *ctx, const char *input, int inlen, int *outlen);
  char * crypt_decode(void *ctx, const char *input, int inlen, int *outlen);


#ifdef __cplusplus
}
#endif

#endif
