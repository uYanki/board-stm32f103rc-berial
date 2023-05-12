/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// 两个信号处理函数原型:
// void fun1 (int );
// void fun2 (int signum, siginfo_t *info, void *myact);

static void sigactexit(int signum, siginfo_t* info, void* myact)
{
    exit(1);
}

// setup signal.
int signalsetup(int signum, void* sigact)
{
    struct sigaction snu;

    sigemptyset(&snu.sa_mask);
    snu.sa_flags     = SA_RESTART;
    snu.sa_sigaction = sigact;
    if (sigaction(signum, &snu, NULL))
        return -1;

    return 0;
}

// setup exit signal
int signalsetupexit(int signum)
{
    return signalsetup(signum, sigactexit);
}

// block signal
int signalblock(int signum)
{
    sigset_t newmask;

    sigemptyset(&newmask);
    sigaddset(&newmask, signum);
    if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0)
        return -1;

    return 0;
}

// unblock signal
int signalunblock(int signum)
{
    sigset_t pendmask, mask;

    if (sigpending(&pendmask) < 0)
        return -1;

    if (sigismember(&pendmask, signum) == 0)
        return -1;

    sigemptyset(&mask);
    sigaddset(&mask, signum);
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0)
        return -1;

    return 0;
}
