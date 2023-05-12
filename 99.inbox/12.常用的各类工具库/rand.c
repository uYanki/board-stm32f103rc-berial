#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

/*
   现在的随机函数发生器大都采用的是线性同余法，包括函数rand()和srand。
*/

typedef struct {
    int s1;
    int s2;
    int seeded;
} lcg;

#define MODMULT(a, b, c, m, s)   \
    q = s / a;                   \
    s = b * (s - a * q) - c * q; \
    if (s < 0) s += m

/*
  combined linear congruential generator
* combinedLCG() returns a pseudo random number in the range of (0, 1).
* The function combines two CGs with periods of
* 2^31 - 85 and 2^31 - 249. The period of this function
* is equal to the product of both primes.
* 组合线性同余发生器返回范围为 (0, 1) 的一个伪随机数。
  本函数组合了周期为 2^31 - 85 和 2^31 - 249 的两个同余发生器。
  本函数的周期等于这两个素数的乘积。
*/

static void lcg_seed(lcg* LCG)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == 0) {
        LCG->s1 = tv.tv_sec ^ (tv.tv_usec << 11);
    } else {
        LCG->s1 = 1;
    }

    LCG->s2 = getpid();

    /* Add entropy to s2 by calling gettimeofday() again */
    if (gettimeofday(&tv, NULL) == 0) {
        LCG->s2 ^= (tv.tv_usec << 11);
    }

    LCG->seeded = 1;
}

double combined_lcg(void)
{
    int q;
    int z;
    lcg LCG;

    lcg_seed(&LCG);

    MODMULT(53668, 40014, 12211, 2147483563L, LCG.s1);
    MODMULT(52774, 40692, 3791, 2147483399L, LCG.s2);

    z = LCG.s1 - LCG.s2;
    if (z < 1) {
        z += 2147483562;
    }

    return z * 4.656613e-10;
}

// /dev/urandom: 随机性较差，但会立即返回
// /dev/random:  随机性较好，但会让进程等待
// @nread指明要读取的随机数个数,返回实际读取的随机数个数
int entropy_rand(char* entropy_file, char* rbuf, int nread)
{
    int fd, n, pos;

    fd = open(entropy_file, O_RDONLY);
    if (fd < 0)
        return -1;

    pos = 0;
    while (nread > 0) {
        n = read(fd, rbuf + pos, nread);
        if (n <= 0)
            break;
        nread -= n;
        pos += n;
    }

    close(fd);
    return pos;
}

//////////////////////////////////////////////////////////////////////////
// RANDOM NUMBERS GENERATOR
//////////////////////////////////////////////////////////////////////////

/// MWC (Multiply-With-Carry) RNG, invented by George Marsaglia
static uint32_t g_dRngState[5] = {0x95d3474bUL, 0x035cf1f7UL, 0xfd43995fUL, 0x5dfc55fbUL, 0x334a9229UL};

/// generate another dword,注意调用前要先设置种子，不然每次启动时都产生相同的随机数
uint32_t _Rand()
{
    uint64_t uSum;
    uSum =
        (uint64_t)g_dRngState[0] * (uint64_t)5115 +
        (uint64_t)g_dRngState[1] * (uint64_t)1776 +
        (uint64_t)g_dRngState[2] * (uint64_t)1492 +
        (uint64_t)g_dRngState[3] * (uint64_t)2111111111UL +
        (uint64_t)g_dRngState[4];
    g_dRngState[3] = g_dRngState[2];
    g_dRngState[2] = g_dRngState[1];
    g_dRngState[1] = g_dRngState[0];
    g_dRngState[4] = (uint32_t)(uSum >> 32);
    g_dRngState[0] = (uint32_t)uSum;
    return g_dRngState[0];
}

/// seed
void _Srand(uint32_t uSeed)
{
    int i;
    for (i = 0; i < 5; i++) {
        uSeed          = uSeed * 29943829 - 1;
        g_dRngState[i] = uSeed;
    }
    for (i = 0; i < 19; i++)
        _Rand();
}

#define USE_WINDOWS 0  /// whether to compile for Windows

/// auto-seed RNG based on time and PID
void _AutoSrand()
{
    // get timestamp
#if !USE_WINDOWS
    struct timeval tv;
    gettimeofday(&tv, NULL);
#else
#define getpid() GetCurrentProcessId()

    struct
    {
        time_t tv_sec;
        DWORD  tv_usec;
    } tv;

    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    uint64_t ts = (uint64_t(ft.dwHighDateTime) << 32) + uint64_t(ft.dwLowDateTime) - 116444736000000000ULL;  // Jan 1, 1970 magic
    ts /= 10;                                                                                                // to microseconds
    tv.tv_sec  = (DWORD)(ts / 1000000);
    tv.tv_usec = (DWORD)(ts % 1000000);
#endif

    // twist and shout
    _Srand(_Rand() ^ (uint32_t)(tv.tv_sec) ^ ((uint32_t)(tv.tv_usec) + (uint32_t)(getpid())));
}

#ifdef TEST_RAND

int main()
{
    double   r;
    char     rb[1024], hex[4096];
    uint32_t ur;
    int      n;

    r = combined_lcg();
    printf("%f\n", r);

    n = entropy_rand("/dev/urandom", rb, 1000);
    savefile("test/urandom", rb, n);
    bin2hex(rb, n, hex);
    printf("%s\n", hex);

    n = entropy_rand("/dev/random", rb, 8);
    savefile("test/random", rb, n);
    bin2hex(rb, n, hex);
    printf("%s\n", hex);

    ur = _Rand();
    printf("%u\n", ur);
    _AutoSrand();
    ur = _Rand();
    printf("%u\n", ur);

    return 0;
}

#endif
