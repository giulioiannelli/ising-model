#include <stdbool.h>
#include <imtdlib.h>

#ifndef __IMFNLIB_H_INC__
#define __IMFNLIB_H_INC__

extern bool F_EEXIST(const char *);
extern bool F_NEXIST(const char *);
extern void F_OPEN(FILE **, const char *, const char *);
extern void challoc(void *);
extern void __CHECKargc(int, int);
extern void __setSFMT_seed_rand(void);
extern void __MAKElog(int, const char *, char *[]);
/* */
extern void __init_hotstart_uniform(sysz_t, lttc_t *);
extern void __init_coldstart(sysz_t, lttc_t *);
extern void printcfg(sysz_t, side_t, lttc_t *);
extern void fprintcfg(FILE **, sysz_t, side_t, lttc_t *);
extern void fwritecfg(FILE **, sysz_t, lttc_t *);
extern nnl_t compute_nn(sysz_t, side_t, side_t);
extern void compute_nnarr(side_t, side_t, nnl_t *);
extern double m(sysz_t, lttc_t *);
extern double dE(sysz_t, lttc_t *, nnl_t);
extern void updME(double, sysz_t, lttc_t *, nnl_t *);
extern dtc_t __fscanf_configfile(char *);
// extern void updWO(double, sysz_t, lttc_t *, nnl_t *);
extern void __gen_config_varbeta(char *config_fn);

#endif /* __IMFNLIB_H_INC__ */

