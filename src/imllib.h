#include <stdio.h>
#include <stdbool.h>
#include <imdtlib.h>
#include <imfnlib.h>

#ifndef __IMLLIB_H_INC__
#define __IMLLIB_H_INC__

extern void __fscanf_dtc(FILE **, dtc_t *); /* scan config files */
extern void __fscanf_smdtc(FILE **, smdtc_t *);
/* print config files */
extern void __printf_dtc(dtc_t);
extern void __printf_smdtc(smdtc_t);
/* read and store config */
extern void __read_dtc(char *, dtc_t *);
extern void __read_smdtc(char *, smdtc_t *);







extern void __set_localdtc(smdtc_t *d1, dtc_t *d);
extern void __upd_localdtc(smdtc_t *d1, double b);

extern void __mkdir_MOD_syszL1L2(const char *mode, char *_dirsz, smdtc_t d);
extern void __mkdir_syszb(char *_dirb, double beta, char*_dirsz);
extern uint32_t countNsteps(dtc_t dtc);
extern uint32_t countbetasteps(dtc_t dtc);

extern nnl_t compute_nn(sysz_t i, side_t L1, side_t L2);
extern void __compute_nn_array(side_t L1, side_t L2, nnl_t *nn);



extern double dE(sysz_t u, hlttc_t *s, nnl_t nn);


extern void __readTauint_tmp(smdtc_t *d1, dtc_t d, side_t L1, side_t L2);

extern void __alloc_fill_snn(hlttc_t **s, nnl_t **nn, smdtc_t d);
extern void __setsave_time(smdtc_t d, sysz_t *st);
extern void __setmeasureOBS(smdtc_t d, vtmpf_t **mf);

extern void __mkdir_obsN(char *_dirat, smdtc_t d);





extern void __ifNETauint_makeit(smdtc_t d);
extern void __Tauint(smdtc_t d);
extern void __readTauint(smdtc_t *d);
extern void __makeTauint(smdtc_t d);
extern void __gen__obs_acfN(smdtc_t d, obs_t O);



extern void __upd_MEHA_ss(double beta, sysz_t N, hlttc_t *s, nnl_t *nn);
extern void __upd_MEHA_sa(double beta, sysz_t N, hlttc_t *s, nnl_t *nn);
extern void __gen_config_(smdtc_t dtc, obs_t O);
extern void __wbrite_nconf_d(smdtc_t d);
extern void __compute_acf(char *config_fn);
extern void __genUNcorr_CONFIG(char *config_fn);



extern void __find_effKCFGS(smdtc_t *d);
extern void __get_P_TYPE(smdtc_t d, char *ptype);

extern void __make_buf_cgfpth(smdtc_t d, char *_dirsz, char *P_TYPE);

extern void __rename_cfgpth(smdtc_t d, char *_dirsz, char *P_TYPE);


extern void __setfunc__init__upd__(char *init_mode, vtmpf_t **__ptrinit__,
                                   char *upd_mode, vtmpf_t **__ptrupd__);

/* main functions */
extern void __print_conf(char *);
extern void __gen_kconf(char *);


#endif