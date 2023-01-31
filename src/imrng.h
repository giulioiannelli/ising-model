#include <time.h>
#include <inttypes.h>
#include <unistd.h>
#include <SFMT.h>
#include <imdefs.h>

#ifndef __IMRNG_H_INC__
#define __IMRNG_H_INC__

#define SEED (uint32_t)0xBE11AC1A0
#define SIID (uint32_t)0xC1A0BE11A
#define LSEED (uint32_t)(time(0) ^ getpid())
#define RSEED (uint32_t)((time(0) << time(0)) ^ getpid())

#define RNG_u64 SFMTrng_u64
#define RNG_dbl SFMTrng_dbl

extern uint64_t SFMTrng_u64(void);
extern double SFMTrng_dbl(void);

extern sfmt_t sfmt;
extern uint32_t *seed_rand;

extern void __setSFMT_seed_rand(void);

#endif /* __IMRNG_H_INC__ */