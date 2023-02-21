/* CHECKED */
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <imdefs.h>

#ifndef __IMDTLIB_H_INC__
#define __IMDTLIB_H_INC__

typedef void (vtmpf_t)();
typedef int8_t hlttc_t;
typedef uint16_t side_t;
typedef uint16_t avg_t;
typedef uint32_t sysz_t;
/* mode program execute struct type */
typedef struct modes_dict
{
    char *__name__;
    void (*__mode__)();
} md_t;
/* struct for storing observable */
typedef struct observables
{
    double *magn;
    double *acf;
} obs_t;
/* struct for storing nearest neighbours */
typedef struct nearest_neigh_on_lattice
{
    uint32_t N;
    uint32_t S;
    uint32_t E;
    uint32_t W;
} nnl_t;
typedef struct config_one_c
{
    uint32_t eK;
    uint32_t mK;
    uint32_t K;
    sysz_t N;
    sysz_t tMC;
    avg_t Navg;
    sysz_t _m_sav;
    double b;
    double ti;
    char _m_init[128];
    char _m_upd[128];
    bool _m_mea;
} imcd_t;
typedef struct config_one_l
{
    uint32_t eK;
    uint32_t mK;
    uint32_t K;
    side_t L1;
    side_t L2;
    sysz_t N;
    sysz_t tMC;
    avg_t Navg;
    sysz_t _m_sav;
    double b;
    double ti;
    char _m_init[128];
    char _m_upd[128];
    bool _m_mea;
} smdtc_t;
typedef struct data_config
{
    sysz_t tMC;
    sysz_t N_M;
    sysz_t _m_sav;
    uint16_t nconf;
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


typedef struct dataStruct_Nandbeta
{
    uint32_t K;
    sysz_t N;
    sysz_t _m_sav;
    avg_t Navg;
    double b;
    char _m_init[128];
    char _m_upd[128];
} dsNb_t;


extern FILE *f_log;
extern char buf[STR1024], buf1[STR1024], buf2[STR1024];
extern const char *MODES[];

#endif  /* __IMDTLIB_H_INC__ */