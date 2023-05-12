/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#ifndef SIG_H
#define SIG_H

#ifdef __cplusplus
extern "C" {
#endif

int signalsetup(int signum, void* sigact);
int signalsetupexit(int signum);
int signalblock(int signum);
int signalunblock(int signum);

#ifdef __cplusplus
}
#endif

#endif
