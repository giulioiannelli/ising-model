#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <imdefs.h>
#include <imdtlib.h>
#include <imfnlib.h>
#include <imclib.h>
#include <imrng.h>

/** allocate the lattice and fill the nearest neighbour struct
 * @param s (hlttc_t **) double pointer to linear chain
 */
extern void __alloc_fill_s(hlttc_t **s, imcd_t d)
{
    (*s) = malloc(sizeof(**s) * d.N);
    __challoc(*s);
}

/**
 * compute and return the variation in the energy of the system when spin at
 * site u flips
 * @param u (sysz_t) updated site
 * @param s (hlttc_t *) the lattice array
 * @return difference of enegry, if negative accept the proposed move
 */
extern double dE(sysz_t u, sysz_t N, hlttc_t *s)
{
    double dE;
    sysz_t ln, rn;
    ln = (u - 1 + N) % N;
    rn = (u + 1 + N) % N;
    dE = 2 * J * s[u] * (s[ln] + s[rn]);
    return dE;
}

/** Metropolis-Hastings algorithm sequential synchronous update scheme
 * @param b (double) inverse temperature
 * @param N (sysz_t) system size
 * @param s (hlttc_t *) system array
 * @return
 */
extern void __upd_MEHA_ss(double b, sysz_t N, hlttc_t *s)
{
    double dEtmp;
    for (sysz_t i = 0; i < N; i++)
    {
        dEtmp = dE(i, N, s);
        if (RNG_dbl() < exp(-dEtmp * b))
            s[i] = -s[i];
    }
}
/** Metropolis-Hastings algorithm sequential asynchronous update scheme
 * @param b (double) inverse temperature
 * @param N (sysz_t) system size
 * @param s (hlttc_t *) system array
 * @return
 */
extern void __upd_MEHA_sa(double b, sysz_t N, hlttc_t *s)
{
    sysz_t u;
    double dEtmp;
    for (sysz_t i = 0; i < N; i++)
    {
        u = RNG_u64() % N;
        dEtmp = dE(u, N, s);
        if (RNG_dbl() < exp(-dEtmp * b))
            s[u] = -s[u];
    }
}

/**
 * ...
 * @param
 * @return
 */
extern void __measure_OBS(sysz_t t, sysz_t N, hlttc_t *s, obs_t O)
{
    O.magn[t] = magn(N, s);
}

/** mode selection in init and update modes 
 * @param init_mode (char *) string defining the initialization of the model
 * @param __ptrinit__ (vtmpf_t **) *pointer to init function
 * @param upd_mode (char *) string defining the update scheme of the model
 * @param __ptrupd__ (vtmpf_t **) *pointer to init function
 * @return 
 */
extern void __setfunc__init__upd__(char *init_mode, vtmpf_t **__ptrinit__,
    char *upd_mode, vtmpf_t **__ptrupd__)
{
    if (strcmp(init_mode, _M_HSU) == 0)
        *__ptrinit__ = __init_hotstart_uniform;
    else if (strcmp(init_mode, _M_CSU) == 0)
        *__ptrinit__ = __init_coldstart;
    else
        *__ptrinit__ = NULL;
    if (strcmp(upd_mode, _M_MEHA_SS) == 0)
        *__ptrupd__ = __upd_MEHA_ss;
    else if (strcmp(upd_mode, _M_MEHA_SA) == 0)
        *__ptrupd__ = __upd_MEHA_sa;
    else // if (strcmp(d.MODE_init, "algo_wolff") == 0)
        *__ptrupd__ = NULL;
}

/** generate a square lattice Ising configuration for measuring observables 
 * needed for auto-correlation function and integrated auto-correlation time
 * @param d (smdtc_t) the single realization struct
 * @param O (obs_t) the observable struct for computing magnetization
 * @return (void) none
 */
extern void __gen__obs_acfN(imcd_t d, obs_t O)
{
    hlttc_t *s;
    vtmpf_t *__init__, *__upd__;
    vtmpf_t *__measure__ = __measure_OBS;
    /**/
    __alloc_fill_s(&s, d);
    __setfunc__init__upd__(d._m_ini, &__init__, d._m_upd, &__upd__);
    /**/
    __init__(d.N, s);
    for (sysz_t t = 1; t < d.N; t++)
    {
        __measure__(t - 1, d.N, s, O);
        __upd__(d.b, d.N, s);
    }
    free(s);
}

/** scan from configuration file the single realization struct
 * @param d (imcd_t) the single realization struct
 * @param config_fn (char *) string with the configuration file name
 * @return (void) none
 */
extern void __fscanf_Nb_configfile(imcd_t *d, char *config_fn)
{
    FILE *fc;
    char row[STR1024];
    char *tok, *ep;
    __F_OPEN(&fc, config_fn, "r+");
    __2get_row_fgets(&fc, row);
    tok = strtok(row, ",");
    d->K = strtou32(tok);
    tok = strtok(NULL, ",");
    d->N = strtou16(tok);
    tok = strtok(NULL, ",");
    d->Navg = strtou16(tok);
    tok = strtok(NULL, ",");
    d->b = strtod(tok, &ep);
    tok = strtok(NULL, ",");
    strcpy(d->_m_ini, tok);
    tok = strtok(NULL, ",");
    strcpy(d->_m_upd, tok);
    d->tMC = d->N;
    fclose(fc);
}

/** create string path to observable folder with configurational parameters
 * @param _dirat (char *) the string onto which sprint the path
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __sprintf_obsN(char *_dirat, imcd_t d)
{
    sprintf(_dirat, DIRcha __NIS__ _H "%s" _H "%s" _S,
        d.N, d._m_ini, d._m_upd);
}
/** create string path to observable folder with configurational parameters
 * @param _dirat (char *) the string onto which sprint the path
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __sprintf_obsNb(char *_dirat, imcd_t d)
{
    sprintf(_dirat, DIRcha __NIS__ _H "%s" _H "%s" _S __BTIS__ _S,
        d.N, d._m_ini, d._m_upd, d.b);
}
/** create string path to observable folder with configurational parameters and
 * make directory if it does not exists
 * @param _dirat (char *) the string onto which sprint the path
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __mkdir_obsN(char *_dirat, imcd_t d)
{
    __sprintf_obsN(_dirat, d);
    mkdir(_dirat, ACCESSPERMS);
    __sprintf_obsNb(_dirat, d);
    mkdir(_dirat, ACCESSPERMS);
}
/** compute integrated correlation time with configuration set by (smdtc_t) 
 * struct 
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __makeTauint(imcd_t d)
{
    char _diracfti[STR512];
    obs_t O;
    double Tauint, *acf, *acf_C, *ti;
    /*////////////////////////////////////////////////////////////////////////*/
    __sprintf_obsNb(_diracfti, d);
    __allocate_acfrel(d.tMC, d.Navg, &acf, &acf_C, &ti);
    O.magn = malloc(sizeof(*O.magn) * d.N);
    __challoc(O.magn);
    /**/
    for (uint32_t av = 0; av < d.Navg; av++)
    {
        __gen__obs_acfN(d, O);
        __acf(d.tMC, &acf, O.magn);
        __acf_cum(d.tMC, acf_C, acf);
        __taui_av(d.tMC, &*(ti + av), acf);
    }
    Tauint = avg(d.Navg, ti);
    __acfcum_norm(d.tMC, d.Navg, acf_C);
    /**/
    __fwrite_acf(_diracfti, d.tMC, d.Navg, acf_C);
    __fwrite_ti(_diracfti, d.Navg, Tauint);
    /**/
    free(acf);
    free(acf_C);
    free(O.magn);
    free(ti);
}

/** set the tauint variable inside the single configuration file
 * @param d (imcd_t) the single realization struct
 * @return (void) none
 */
extern void __ifNETauint_makeit(imcd_t d)
{
    char _dirsti[STR512];
    __mkdir_obsN(_dirsti, d);
    sprintf(buf, "%stauint_avg=" FMT_na EXTBIN, _dirsti, d.Navg);
    if (F_NEXIST(buf))
        __makeTauint(d);
}
/** read integrated correlation time with configuration set by (imcd_t) struct
 * and store inside it
 * @param d (imcd_t) the single realization struct
 * @return (void) none
 */
extern void __readTauint(imcd_t *d)
{
    FILE *fti;
    char _diracfti[STR512];
    __mkdir_obsN(_diracfti, *d);
    sprintf(buf, "%stauint_avg=" FMT_na EXTBIN, _diracfti, d->Navg);
    __F_OPEN(&fti, buf, "rb");
    if (fread(&d->ti, sizeof(double), ONE, fti) != ONE)
    {
        ;// to do debug of problem ifreading does not goes well
    }
    d->_m_sav = ceil(d->ti) + 1;
    fclose(fti);
}
/** set the tauint variable inside the single configuration file
 * @param d (imcd_t) the single realization struct
 * @return (void) none
 */
extern void __Tauint(imcd_t d)
{
    __ifNETauint_makeit(d);
    __readTauint(&d);
}


extern void __setsave_time(imcd_t d, sysz_t *st)
{
    if (d._m_sav)
        *st = d._m_sav;
    else
        *st = d.tMC + 1;
}

extern void __mkdir_MOD(const char *mode, char *_dirsz, imcd_t d)
{
    if (strcmp(mode, "--acf") == 0)
        sprintf(_dirsz, DIRcha __NIS__ _H "%s" _H "%s" _S, d.N, d._m_ini, d._m_upd);
    else if (strcmp(mode, "--gen_config") == 0)
        sprintf(_dirsz, DIRvbc __NIS__ _H "%s" _H "%s" _S, d.N, d._m_ini, d._m_upd);
    else if (strcmp(mode, MODE_KGENCN) == 0)
        sprintf(_dirsz, DIRunc "%s" _H "%s" _S,d._m_ini, d._m_upd);
    mkdir(_dirsz, ACCESSPERMS);
}

extern void __get_P_TYPE(imcd_t d, char *ptype)
{
    if (strcmp(d._m_ini, _M_HSU) == 0)
    {
        if (strcmp(d._m_upd, _M_MEHA_SA) == 0)
            sprintf(ptype, ISING1DHSSA);
        else if (strcmp(d._m_upd, _M_MEHA_SS) == 0)
            sprintf(ptype, ISING1DHSSS);
    }
    else if (strcmp(d._m_ini, _M_CSU) == 0)
    {
        if (strcmp(d._m_upd, _M_MEHA_SA) == 0)
            sprintf(ptype, ISING2DCSSA);
        else if (strcmp(d._m_upd, _M_MEHA_SS) == 0)
            sprintf(ptype, ISING2DCSSS);
    }
}

extern void __make_buffind(imcd_t d, char *ptype)
{
    sprintf(buf, "find " DIRunc " -maxdepth 1 -name %s" _U __NIS__ _U __TIS__ _U "*",
        ptype, d.N, 1. / d.b);
}

extern void __make_buf_cgfpth(imcd_t d, char *_dirsz, char *P_TYPE)
{
    sprintf(buf, "%s%s" _U __NIS__ _U __TIS__ _U _FMT_K_ EXTBIN, 
            _dirsz, P_TYPE, d.N, 1. / d.b, d.eK);
}

extern void __find_effKCFGS(imcd_t *d)
{
    FILE *pipe;
    char P_TYPE[STR256];
    __get_P_TYPE(*d, P_TYPE);
    __make_buffind(*d, P_TYPE);
    __P_OPEN(&pipe, buf, "r");

    if (fgets(buf, sizeof(buf), pipe) == NULL)
    {
        ;
    }
    buf[strlen(buf) - 1] = '\0';
    d->eK = d->K;
    d->mK = d->K;
    if (!(strncmp(buf, DIRunc, strlen(DIRunc))))
    {
        sprintf(buf1, DIRunc "%s" _U __NIS__ _U __TIS__ _U "[", P_TYPE, d->N, 1. / d->b);
        strncpy(buf2, buf + strlen(buf1), strlen(buf) - strlen(buf1) - 5);
        d->eK = strtou32(buf2);
        d->mK = softplus_u32(d->K - d->eK);
    }
    pclose(pipe);
}

extern void __rename_cfgpth(imcd_t d, char *_dirsz, char *P_TYPE)
{
    if (d.mK)
    {
        sprintf(buf2, "%s%s" _U __NIS__ _U __TIS__ _U _FMT_K_ EXTBIN,
                _dirsz, P_TYPE, d.N, 1. / d.b, d.K);
        if ((rename(buf, buf2)) == 0)
        {
            fprintf(f_log, PIFRSUC);
        }
        else
        {
            fprintf(f_log, MSGFAIL PFUNREN);
        }
    }
}
extern void __wbrite_nconf_d(imcd_t d)
{
    FILE *fo;
    char _dirsz[STR256];
    char P_TYPE[STR256];
    hlttc_t *s;
    sysz_t St;
    vtmpf_t *__init__, *__upd__;
    /**/
    d.tMC = d.K * d._m_sav;
    __alloc_fill_s(&s, d);
    __setfunc__init__upd__(d._m_ini, &__init__, d._m_upd, &__upd__);
    __setsave_time(d, &St);
    /**/
    __mkdir_MOD(MODE_KGENCN, _dirsz, d);
    __find_effKCFGS(&d);
    __get_P_TYPE(d, P_TYPE);
    __make_buf_cgfpth(d, _dirsz, P_TYPE);
    __F_OPEN_SEEKEND(&fo, buf, "a+b");
    /**/
    for (sysz_t i = 0; i < d.mK; i++)
    {
        __init__(d.N, s);
        for (sysz_t t = 0; t < St; t++)
            __upd__(d.b, d.N, s);
        __fwrite_cfg(&fo, d.N, s);
    }
    free(s);
    fclose(fo);
    /**/
    __rename_cfgpth(d, _dirsz, P_TYPE);
}