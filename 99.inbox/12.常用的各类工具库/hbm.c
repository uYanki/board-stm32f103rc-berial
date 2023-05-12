#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xstring.h"
#include "xmalloc.h"

/* #define COPY_PATTERN */

/*
*
*  Boyer-Moore-Horsepool for small pattern groups
*
*/

typedef struct
{

  const uint8_t *P;
  int            M;
  short          bcShift[256];

}
HBM_STRUCT;

static HBM_STRUCT * hbm_prepx(HBM_STRUCT *p, const uint8_t * pat, int m);
static HBM_STRUCT * hbm_prep(const uint8_t * pat, int m);
static uint8_t * hbm_match(HBM_STRUCT * px, const uint8_t * text, int n);
static void hbm_free(HBM_STRUCT *p);


static HBM_STRUCT * hbm_prepx(HBM_STRUCT *p, const uint8_t * pat, int m)
{
  int     k;

  if (!m) return NULL;
  if (!p) return NULL;

#ifdef COPYPATTERN
  p->P = (uint8_t*)xmalloc(m + 1);
  if (!p->P) return NULL;
  memcpy(p->P, pat, m);
#else
  p->P = pat;
#endif

  p->M = m;

  /* Compute normal Boyer-Moore Bad Character Shift */
  for (k = 0; k < 256; k++) p->bcShift[k] = m;
  for (k = 0; k < m; k++)   p->bcShift[pat[k]] = m - k - 1;

  return p;
}

/*
*
*/
static HBM_STRUCT * hbm_prep(const uint8_t * pat, int m)
{
  HBM_STRUCT    *p;

  p = (HBM_STRUCT*)xmalloc(sizeof(HBM_STRUCT));
  if (!p) return NULL;

  return hbm_prepx(p, pat, m);
}


static void hbm_free(HBM_STRUCT *p)
{
  if (p)
  {
#ifdef COPYPATTERN
    if (p->P) xfree(p->P);
#endif
    xfree(p);
  }
}

/*
*   Boyer-Moore Horspool
*   Does NOT use Sentinel Byte(s)
*   Scan and Match Loops are unrolled and separated
*   Optimized for 1 byte patterns as well
*/
static uint8_t * hbm_match(HBM_STRUCT * px, const uint8_t * text, int n)
{
  const uint8_t *pat, *t, *et, *q;
  int m1, k;
  short *bcShift;

  m1     = px->M - 1;
  pat    = px->P;
  bcShift = px->bcShift;

  t  = text + m1;
  et = text + n;

  /* Handle 1 Byte patterns - it's a faster loop */
  if (!m1)
  {
    for (;t < et; t++)
      if (*t == *pat) return (uint8_t *)t;
    return NULL;
  }

  /* Handle MultiByte Patterns */
  while (t < et)
  {
    /* Scan Loop - Bad Character Shift */
    do
    {
      t += bcShift[*t];
      if (t >= et)return NULL;
      ;

      t += (k = bcShift[*t]);
      if (t >= et)return NULL;

    }
    while (k);

    /* Unrolled Match Loop */
    k = m1;
    q = t - m1;
    while (k >= 4)
    {
      if (pat[k] != q[k])goto NoMatch;
      k--;
      if (pat[k] != q[k])goto NoMatch;
      k--;
      if (pat[k] != q[k])goto NoMatch;
      k--;
      if (pat[k] != q[k])goto NoMatch;
      k--;
    }
    /* Finish Match Loop */
    while (k >= 0)
    {
      if (pat[k] != q[k])goto NoMatch;
      k--;
    }
    /* If matched - return 1st char of pattern in text */
    return (uint8_t *)q;

  NoMatch:

    /* Shift by 1, this replaces the good suffix shift */
    t++;
  }

  return NULL;
}

#define TEXT_LOWER  (100*1024)
#define PAT_LOWER    128

uint8_t * hbm_patternmatch(const uint8_t *text, int n, 
                                            const uint8_t *pat, int m, 
                                            int no_case,int codetype)
{
  HBM_STRUCT *px;
  uint8_t *puc = NULL;
  uint8_t text_l[TEXT_LOWER+1], pat_l[PAT_LOWER+1];
  uint8_t *text_p, *pat_p;
  int tdy, pdy;

  tdy = 0;
  pdy = 0;

  if (no_case)
  {
    if (n > TEXT_LOWER)
    {
      text_p = (uint8_t *)xcalloc(1, n + 1);
      tdy = 1;
    }
    else
    {
      text_p = text_l;
    }

    memcpy(text_p, text, n);
    *(text_p + n) = '\0';
    strtolower((char *)text_p, n, (char *)text_p,codetype);

    if (m > PAT_LOWER)
    {
      pat_p = (uint8_t *)xcalloc(1, m + 1);
      pdy = 1;
    }
    else
    {
      pat_p = pat_l;
    }

    memcpy(pat_p, pat, m);
    *(pat_p + m) = '\0';
    strtolower((char *)pat_p, m, (char *)pat_p,codetype);

    px = hbm_prep(pat_p, m);
    if (!px)
      return NULL;
    puc = hbm_match(px, text_p, n);
    hbm_free(px);
    if (tdy)
      xfree(text_p);
    if (pdy)
      xfree(pat_p);
    if (puc)
      puc = (uint8_t *)(text + (puc - text_p));

    return puc;
  }

  px = hbm_prep(pat, m);
  if (!px)
    return NULL;
  puc = hbm_match(px, text, n);
  hbm_free(px);
  return puc;
}

#ifdef TEST_HBM
int main()
{
  uint8_t *puc;

  puc = hbm_patternmatch("刘兴之liuXingzhi", 16, "liu", 3, 0,2);
  printf("%s\n", puc);
  puc = hbm_patternmatch("niHao刘兴之liuXingzhi", 21, "xIng", 4, 1,2);
  printf("%s\n", puc);
  puc = hbm_patternmatch("niHao刘兴之liuXingzhi", 21, "xIng", 4, 0,2);
  printf("%s\n", puc);
  puc = hbm_patternmatch("niHao刘兴之liuXingzhi", 21, "兴之", 4, 1,2);
  printf("%s\n", puc);
}
#endif
