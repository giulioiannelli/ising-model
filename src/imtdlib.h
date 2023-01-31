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
typedef struct config_one
{
    bool MODE_measure;
    char MODE_init[128], MODE_upd[128];
    double beta;
    sysz_t tMCMC, MODE_save;
    side_t L1, L2;
} dtc1_t;
typedef struct data_config
{
    bool MODE_measure;
    char MODE_init[128], MODE_upd[128];
    double beta_m, beta_M, beta_stp;
    uint16_t NAVG;
    sysz_t N_M, tMCMC, MODE_save;
    side_t L1, L2, Ls;
} dtc_t;

typedef struct observables
{
    double *magn;
} obs_t;

extern FILE *f_log;

#endif  /* __IMDTLIB_H_INC__ */