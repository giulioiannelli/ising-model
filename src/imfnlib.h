#include <stdio.h>
#include <stdbool.h>
#include <imdtlib.h>

#ifndef __IMFNLIB_H_INC__
#define __IMFNLIB_H_INC__

extern double sum_vs(size_t n, double *v);
extern double sum_vs_hlttc_t(size_t n, hlttc_t *v);
extern double sum_vs_2(size_t n, double *v);
extern double avg_hlttc_t(size_t n, hlttc_t *v);
extern double avg_2(size_t n, double *v);


extern bool F_NEXIST(const char *n);




extern void __allocate_acfrel(sysz_t tMC, avg_t Navg, double **acf, double **acf_C, double **ti);




extern void __splash__(void);
extern size_t strIn_(char *, size_t, const char **);
extern double sum_vs(size_t n, double *v);
extern double avg(size_t n, double *v);
extern uint16_t strtou16(const char *s);
extern uint32_t softplus_u32(int32_t);
extern uint32_t strtou32(const char *s);

extern bool __feexist(const char *);
extern bool __fnexist(const char *);
extern void __F_OPEN(FILE **, const char *, const char *);
extern void __F_OPEN_SEEKEND(FILE **, const char *, const char *);
extern void __popen(FILE **p, const char *fn, const char *md);
extern void __challoc(void *);
extern void __CHECKargc(int, int);
extern void __setSFMT_seed_rand(void);
extern void __MAKElog(int, char *[]);
/* */
extern void __init_hotstart_uniform(sysz_t N, hlttc_t *s);
extern void __init_coldstart(sysz_t N, hlttc_t *s);
extern void __print_CONFIG(sysz_t N, side_t L1, hlttc_t *s);
extern void __fprint_CONFIG(char *fn, sysz_t N, side_t L1, hlttc_t *s);
extern void __fwrite_CONFIG(char *fn, sysz_t t, sysz_t N, hlttc_t *s);
extern void __fwrite_cfg(FILE **fout, sysz_t N, hlttc_t *s);
extern void __fwrite_acf(char *fn, sysz_t tMC, avg_t Navg, double *acf);
extern void __fwrite_ti(char *dir, avg_t Navg, double ti);

extern void __set_localdtc(smdtc_t *d1, dtc_t *d);


extern void __get_row_fgets(FILE **fc, char *row);
extern void __2get_row_fgets(FILE **fc, char *row);

extern double magn(sysz_t N, hlttc_t *s);
extern double m_avg(sysz_t tMC, double *m);
extern double m_avg2(sysz_t tMC, double *m);
extern double m_corr_t(sysz_t tMC, sysz_t l, double *m);
extern void __acf(sysz_t tMC, double **acf, double *m);
extern void __acf_cum(sysz_t tMC, double *acfc, double *acf);
extern void __acfcum_norm(sysz_t tMC, avg_t Navg, double *acfc);
extern void __taui_av(sysz_t tMC, double *tiav, double *acf);

extern void __measure_OBS(sysz_t t, sysz_t N, hlttc_t *s, obs_t O);

#endif /* __IMFNLIB_H_INC__ */

