#ifndef __AVR__
#define PROGMEM
#define memcpy_P         memcpy
#define __LPM(x)         *x
#define pgm_read_word(x) *x
#else
#include <avr/pgmspace.h>
#define USEPRECALC
#endif

#ifndef USEPRECALC
// malloc-ed by initframe, free manually
extern uint8_t* strinbuf;  // string iput buffer
extern uint8_t* qrframe;
// setup the base frame structure - can be reused
void            initframe(void);
// free the basic frame malloced structures
void            freeframe(void);
// these resturn maximum string size to send in
uint32_t        initeccsize(uint8_t ecc, uint32_t size);
uint32_t        initecc(uint8_t level, uint8_t version);
#else  // precalc-ed arrays
extern uint8_t strinbuf[];
extern uint8_t qrframe[];
#endif

extern uint8_t WD, WDB;
#include "qrbits.h"

#ifdef __cplusplus
extern "C" {
#endif
// strinbuf in, qrframe out
void qrencode(void);
#ifdef __cplusplus
}  // extern "C"
#endif
