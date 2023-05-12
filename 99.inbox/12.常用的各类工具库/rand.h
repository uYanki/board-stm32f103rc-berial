#ifndef RAND_H
#define RAND_H

#ifdef __cplusplus
extern "C" {
#endif

double combined_lcg(void);
int    entropy_rand(char* entropy_file, char* rbuf, int nread);

uint32_t _Rand(void);
void     _Srand(uint32_t uSeed);
void     _AutoSrand(void);

#ifdef __cplusplus
}
#endif

#endif
