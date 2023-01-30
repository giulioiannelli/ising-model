/* CHECKED */
#include <inttypes.h>

#ifndef __IMDTLIB_H_INC__
#define __IMDTLIB_H_INC__

typedef int8_t lttc_t;
typedef uint16_t side_t;
typedef uint32_t sysz_t;
typedef struct nearestneigh
{
    uint32_t N;
    uint32_t S;
    uint32_t E;
    uint32_t W;
} nnl_t;

#endif  /* __IMDTLIB_H_INC__ */