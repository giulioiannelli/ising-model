#include <stdbool.h>
#include <imtdlib.h>

#ifndef __IMFNLIB_H_INC__
#define __IMFNLIB_H_INC__

extern size_t strIn_(char *, const char **);
extern void __splash__(void);
double avg(avg_t, double *);
uint16_t strtou16(const char *s);
uint32_t strtou32(const char *s);

extern bool F_EEXIST(const char *);
extern bool F_NEXIST(const char *);
extern void __F_OPEN(FILE **, const char *, const char *);
extern void __challoc(void *);
extern void __CHECKargc(int, int);
extern void __setSFMT_seed_rand(void);
extern void __MAKElog(int, char *[]);
/* */
extern void __init_hotstart_uniform(sysz_t N, lttc_t *s);
extern void __init_coldstart(sysz_t N, lttc_t *s);
extern void __print_CONFIG(sysz_t N, side_t L1, lttc_t *s);
extern void __fprint_CONFIG(char *fn, sysz_t N, side_t L1, lttc_t *s);
extern void __fwrite_CONFIG(char *fn, sysz_t t, sysz_t N, lttc_t *s);
extern void __fwrite_CONFIGsamefile(FILE **fout, sysz_t N, lttc_t *s);
extern void __fwrite_ACF(char *fn, uint16_t na, sysz_t tMC, double *acf);
extern void __fwrite_ti(char *fn, uint16_t na, double ti);
extern void __fscanf_configfile(dtc_t *dtc, char *config_fn);
extern void __set_localdtc(smdtc_t *d1, dtc_t *d);
extern void __printf_configfile(dtc_t d, char *config_fn);
extern void __print_configf(char *config_fn);
extern void __set_localdtc(smdtc_t *d1, dtc_t *d);
extern void __upd_localdtc(smdtc_t *d1, double b, side_t L1, side_t L2);

extern void __mkdir_syszN(char *_dirsz, side_t L1, side_t L2, smdtc_t d);
extern void __mkdir_syszb(char *_dirb, double beta, char*_dirsz);
extern uint32_t countNsteps(dtc_t dtc);
extern uint32_t countbetasteps(dtc_t dtc);

extern nnl_t compute_nn(sysz_t i, side_t L1, side_t L2);
extern void __compute_nn_array(side_t L1, side_t L2, nnl_t *nn);

extern double m(sysz_t N, lttc_t *s);
extern double m_avg(sysz_t tMC, double *m);
extern double m_avg2(sysz_t tMC, double *m);
extern double m_corr_t(sysz_t tMC, sysz_t l, double *m);
extern double dE(sysz_t u, lttc_t *s, nnl_t nn);
// extern double *__ACFcomputation(smdtc_t d1, obs_t O);
extern void __ACFcomputation(double **corr, smdtc_t d1, obs_t O);

extern void __readTauint(smdtc_t *d1, dtc_t d, side_t L1, side_t L2);

extern void __alloc_fill_snn(lttc_t **s, nnl_t **nn, side_t L1, side_t L2);

extern void __measure_OBS(sysz_t t, sysz_t N, lttc_t *s, obs_t O);
extern void __upd_MEseq__scheme(double beta, sysz_t N, lttc_t *s, nnl_t *nn);
extern void __upd_MEasy__scheme(double beta, sysz_t N, lttc_t *s, nnl_t *nn);
extern void __gen_config_(smdtc_t dtc, obs_t O);
extern void __gen_config2_(smdtc_t d);
extern void __compute_ACF(char *config_fn);
extern void __genUNcorr_CONFIG(char *config_fn);

#endif /* __IMFNLIB_H_INC__ */

