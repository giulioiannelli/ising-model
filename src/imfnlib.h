#include "imtdlib.h"

#ifndef __IMFNLIB_H_INC__
#define __IMFNLIB_H_INC__

extern void inits_randu(sysz_t, lttc_t *);
extern void printcfg(sysz_t, side_t, lttc_t *);
extern void fprintcfg(FILE **, sysz_t, side_t, lttc_t *);
extern void fwritecfg(FILE **, sysz_t, lttc_t *);
extern nnl_t compute_nn(sysz_t, side_t, side_t);
extern void compute_nnarr(side_t, side_t, nnl_t *);
extern double m(sysz_t, lttc_t *);
extern double dE(sysz_t, sysz_t, lttc_t *, nnl_t);
extern sysz_t updMC(double, sysz_t, lttc_t *, nnl_t *);

#endif /* __IMFNLIB_H_INC__ */

