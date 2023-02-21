#include <stdio.h>
#include <inttypes.h>
#include <SFMT.h>
#include <imrng.h>
/**/
#define PRIrngN 10
/**
 * generate and return 64-bit pseudorandom number.
 * init_gen_rand or init_by_array must be called before this function.
 * @return (uint64_t) 64-bit pseudorandom number
 */
uint64_t SFMTrng_u64(void)
{
    return sfmt_genrand_uint64(&sfmt);
}
/**
 * generate and return a random number on [0,1) with 53-bit resolution
 * init_gen_rand or init_by_array must be called before this function.
 * @return (double) number on [0,1) with 53-bit resolution
 */
double SFMTrng_dbl(void)
{
    return sfmt_genrand_res53(&sfmt);
}
/** TBD
 */
void __check_RNG(void)
{
    for (int i = 0; i < PRIrngN; i++)
    {
        printf("rand_%d: (double) %.3g," _T "(uint64_t) %" PRIu64 "\n",
               i, RNG_dbl(), RNG_u64());
    }
}