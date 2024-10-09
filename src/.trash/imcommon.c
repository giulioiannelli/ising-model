#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <imdtlib.h>
#include <imfnlib.h>
#include <imclib.h>
#include <imrng.h>

char row[STR1024];
/*////////////////////////////////////////////////////// I/O SYSTEM FUNCTIONS */
//
/** write on file the acf (auto-correlation function) of the system in binary
 * (double) format
 * @param dir (char *) string with the directory
 * @param tMC (sysz_t) length of correlation interval considered
 * @param Navg (avg_t) number of averages computed
 * @param acf (double *) the autocorrelation function array
 * @return none
 */
extern void __fwrite_acf(char *dir, sysz_t tMC, avg_t Navg, double *acf)
{
    FILE *fout;
    sprintf(buf, "%sACF_avg=%" PRIu16 EXTBIN, dir, Navg);
    __F_OPEN(&fout, buf, "wb");
    fwrite(acf, sizeof(*acf), tMC, fout);
    fclose(fout);
}
/** write on file the integrated auto-correlation time of the system in binary
 * (double) format
 * @param dir (char *) string with the directory
 * @param Navg (avg_t) number of averages computed
 * @param ti (double) the tauint time
 * @return none
 */
extern void __fwrite_ti(char *dir, avg_t Navg, double ti)
{
    FILE *fout;
    sprintf(buf, "%stauint_avg=" FMT_na EXTBIN, dir, Navg);
    __F_OPEN(&fout, buf, "wb");
    fwrite(&ti, sizeof(ti), ONE, fout);
    fclose(fout);
}
/** write on file a system configuration, allowing for multiple calls within
 * for writing on the same file
 * @param fout (FILE **) file onto which to print
 * @param N (sysz_t) system size
 * @param s (hlttc_t *) system array
 * @return none
 */
extern void __fwrite_cfg(FILE **fout, sysz_t N, hlttc_t *s)
{
    fwrite(s, sizeof(*s), N, *fout);
}
/*/////////////////////////////////////////////////// MEMORY MANAGE FUNCTIONS */
//
/** allocate memory for autocorrelation functions and tauint
 * @param tMC (sysz_t) size of the model
 * @param Navg (hlttc_t *) the model array
 * @return none
 */
extern void __allocate_acfrel(sysz_t tMC, avg_t Navg, double **acf, double **acf_C, double **ti)
{
    *acf = calloc(tMC, sizeof(**acf));
    __challoc(*acf);
    *acf_C = calloc(tMC, sizeof(**acf_C));
    __challoc(*acf_C);
    *ti = malloc(sizeof(**ti) * Navg);
    __challoc(*ti);
}

/*////////////////////////////////////////////////////// INIT MODEL FUNCTIONS */
//
/** initialize the model with a hot start (uniform distribution), i.e. spins
 * are Rademacher variables, {-1, +1}: p = 1/2
 * @param N (sysz_t) size of the model
 * @param s (hlttc_t *) the model array
 * @return none
 */
extern void __init_hotstart_uniform(sysz_t N, hlttc_t *s)
{
    for (sysz_t i = 0; i < N; i++)
        *(s + i) = RANDTR01_11;
}
/** initialize the model with a cold start, i.e. all the spins equal to +1 
 * (-1) with p = 1/2
 * @param N (sysz_t) size of the model
 * @param s (hlttc_t *) the model array
 * @return none
 */
extern void __init_coldstart(sysz_t N, hlttc_t *s)
{
    *s = RANDTR01_11;
    memset(s, *s, sizeof(*s) * N);
}

/*///////////////////////////////////////////////////// OBSERVABLES FUNCTIONS */
//
/** compute and return the magnetization of the system
 * @param N (sysz_t) size of the model
 * @param s (hlttc_t *) the system array
 * @return none
 */
extern double magn(sysz_t N, hlttc_t *s)
{
    return avg_hlttc_t(N, s);
}
/** compute and return the average magnetization of the system over time
 * @param tMC (sysz_t) the montecarlo simulation time
 * @param m (double *) the magnetization array
 * @return the sum of the array over the length
 */
extern double m_avg(sysz_t tMC, double *m)
{
    return avg(tMC, m);
}
/** compute and return the average magnetization of the system over time
 * @param tMC (sysz_t) the montecarlo simulation time
 * @param m (double *) the magnetization array
 * @return the sum of the array over the length
 */
extern double m_avg2(sysz_t tMC, double *m)
{
    return avg_2(tMC, m);
}
/** compute the time correlation with lag l of the time series of an observable
 * @param tMC (sysz_t) the montecarlo simulation time
 * @param l (sysz_t) the time lag to which compute ACF
 * @param m (double *) the magnetization array
 * @return
 */
extern double m_corr_t(sysz_t tMC, sysz_t l, double *m)
{
    double sum = 0.;
    for (sysz_t t = 0; t < tMC - l; t++)
        sum += m[t] * m[t + l];
    return sum / (tMC - l);
}
/** compute autocorrelation function for a time series of an observable
 * @param acf (double **) the autocorrelation preallocated array
 * @param tMC (sysz_t) the Monte Carlo simulation time
 * @param m (double *) the quantity for which to compute ACF (usually magnetiz.)
 * @return
 */
extern void __acf(sysz_t tMC, double **acf, double *m)
{
    sysz_t l;
    double mavg, mavg2, m2avg;
    mavg = m_avg(tMC, m);
    m2avg = m_avg2(tMC, m);
    mavg2 = mavg * mavg;
    *(*acf + (l = 0)) = 1;
    while (++l < tMC - 1)
        *(*acf + l) = (m_corr_t(tMC, l, m) - mavg2) / (m2avg - mavg2);
}
/** compute cumulative (for averaging) autocorrelation function
 * @param acf (double **) the autocorrelation preallocated array
 * @param tMC (sysz_t) the Monte Carlo simulation time
 * @param m (double *) the quantity for which to compute ACF (usually magnetiz.)
 * @return
 */
extern void __acf_cum(sysz_t tMC, double *acfc, double *acf)
{
    for (sysz_t t = 0; t < tMC; t++)
        *(acfc + t) += *(acf + t);
}
/** normalize cumulative autocorrelation function for the number of averages
 * @param acf (double **) the autocorrelation preallocated array
 * @param tMC (sysz_t) the Monte Carlo simulation time
 * @param m (double *) the quantity for which to compute ACF (usually magnetiz.)
 * @return
 */
extern void __acfcum_norm(sysz_t tMC, avg_t Navg, double *acfc)
{
    for (sysz_t t = 0; t < tMC; t++)
        *(acfc + t) /= Navg;
}
/** compute the averaged integrated autocorrelation time
 * @param tMC (sysz_t) the Monte Carlo simulation time
 * @param tiav (double *) the n-th replica of tauint
 * @param acf (double *) the acf
 * @return
 */
extern void __taui_av(sysz_t tMC, double *tiav, double *acf)
{
    sysz_t tt = 0;
    *tiav = 1. / 2;
    while (acf[tt++] > (1. / M_E) && tt < tMC)
        *tiav += acf[tt];
}