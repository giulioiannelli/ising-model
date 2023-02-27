#include <stdio.h>
#include <stdbool.h>
#include <imdtlib.h>
#include <imfnlib.h>

#ifndef __IMCLIB_H_INC__
#define __IMCLIB_H_INC__

extern void __alloc_fill_s(hlttc_t **s, imcd_t d);

extern void __fscanf_Nb_configfile(imcd_t *d, char *config_fn);
extern void __sprintf_obsN(char *_dirat, imcd_t d);
extern void __sprintf_obsNb(char *_dirat, imcd_t d);

extern double dE(sysz_t u, sysz_t N, hlttc_t *s);

extern void __upd_MEHA_ss(double b, sysz_t N, hlttc_t *s);
extern void __upd_MEHA_sa(double b, sysz_t N, hlttc_t *s);
extern void __setfunc__init__upd__(char *init_mode, vtmpf_t **__ptrinit__,
    char *upd_mode, vtmpf_t **__ptrupd__);
extern void __gen__obs_acfN(imcd_t d, obs_t O);
extern void __mkdir_obsN(char *_dirat, imcd_t d);

extern void __makeTauint(imcd_t d);
extern void __ifNETauint_makeit(imcd_t d);
extern void __readTauint(imcd_t *d);
extern void __Tauint(imcd_t *d);

extern void __wbrite_nconf_d(imcd_t d);

extern void __gen_kconf(char *config_fn);

#endif