/* CHECKED */
#include <inttypes.h>
#include <stdbool.h>

#ifndef __IMDTLIB_H_INC__
#define __IMDTLIB_H_INC__

typedef int8_t lttc_t;
typedef uint16_t side_t;
typedef uint16_t avg_t;
typedef uint32_t sysz_t;
typedef struct modes_dict
{
    char *__name__;
    void (*__mode__)();
} md_t;
typedef struct observables
{
    double *magn;
    double *acf;
} obs_t;
typedef struct nearest_neigh_on_lattice
{
    uint32_t N;
    uint32_t S;
    uint32_t E;
    uint32_t W;
} nnl_t;
typedef struct config_one
{
    side_t L1;
    side_t L2;
    sysz_t tMC;
    sysz_t _m_sav;
    double b;
    char _m_init[128];
    char _m_upd[128];
    bool _m_mea;
} smdtc_t;
typedef struct data_config
{
    sysz_t tMC;
    sysz_t N_M;
    sysz_t _m_sav;
    avg_t Navg;
    side_t L1;
    side_t L2;
    side_t Ls;
    double b_m;
    double b_M;
    double b_s;
    char _m_init[128];
    char _m_upd[128];
    bool _m_mea;
} dtc_t;



#endif  /* __IMDTLIB_H_INC__ */