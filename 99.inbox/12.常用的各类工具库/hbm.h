
#ifndef HBM_H
#define HBM_H

#ifdef __cplusplus
extern "C" {
#endif

uint8_t * hbm_patternmatch(const uint8_t *text,int n,
                                            const uint8_t *pat,int m,
                                            int no_case,int codetype);

#ifdef __cplusplus
}
#endif

#endif
