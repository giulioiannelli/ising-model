#include <SFMT.h>
#include "imrng.h"

/**
 * generate and return 64-bit pseudorandom number.
 * init_gen_rand or init_by_array must be called before this function.
 * @param sfmt SFMT internal state
 * @return 64-bit pseudorandom number
 */
uint64_t SFMTrng_u64()
{
    return sfmt_genrand_uint64(&sfmt);
}
/**
 * generate and return a random number on [0,1) with 53-bit resolution
 * @param sfmt SFMT internal state
 * @return double on [0,1) with 53-bit resolution
 */
double SFMTrng_dbl()
{
    return sfmt_genrand_res53(&sfmt);
}