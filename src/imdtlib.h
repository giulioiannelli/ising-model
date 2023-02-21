/* CHECKED */
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <imdefs.h>

#ifndef __IMDTLIB_H_INC__
#define __IMDTLIB_H_INC__

typedef void(vtmpf_t)();
typedef int8_t hlttc_t;
typedef uint16_t side_t;
typedef uint16_t avg_t;
typedef uint32_t sysz_t;
/* mode program execute struct type */
typedef struct modes_dict
{
    char *__name__;
    vtmpf_t *__mode__;
} md_t;
/* struct for storing observable */
typedef struct observables
{
    double *magn;
    double *acf;
} obs_t;
/* struct for storing nearest neighbours */
typedef struct nearest_neigh_on_2Dlattice
{
    sysz_t N;
    sysz_t S;
    sysz_t E;
    sysz_t W;
} nnl_t;
typedef struct config_one_c
{
    sysz_t eK;
    sysz_t mK;
    sysz_t K;
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
    sysz_t eK;
    sysz_t mK;
    sysz_t K;
    side_t L1;
    side_t L2;
    sysz_t N;
    sysz_t tMC;
    sysz_t _m_sav;
    avg_t Navg;
    double b;
    double ti;
    bool _m_mea;
    char _m_init[128];
    char _m_upd[128];
} smdtc_t;
typedef struct data_config_l
{
    sysz_t tMC;
    sysz_t N_M;
    sysz_t K;
    sysz_t _m_sav;
    avg_t Navg;
    side_t L1;
    side_t L2;
    side_t Ls;
    double b_m;
    double b_M;
    double b_s;
    bool _m_mea;
    char _m_init[128];
    char _m_upd[128];
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
extern char buf[STR1024], buf1[STR1024], buf2[STR1024], row[STR1024];
extern const char *MODES[];
extern const char *DT_DTC_I2D[], *DT_SMDTC_I2D[];

#endif /* __IMDTLIB_H_INC__ */