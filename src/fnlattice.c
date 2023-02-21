#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <imdefs.h>
#include <imdtlib.h>
#include <imfnlib.h>
#include <imllib.h>
#include <imrng.h>


const char *DT_DTC_I2D[]={MODE_ACFCOR__, MODE_GENCON__};
const char *DT_SMDTC_I2D[]={MODE_KGENCN__};

/**
 * compute the nearest neighbor given a specific site of the lattice
 * @param i (sysz_t) the lattice site
 * @param L1 (side_t) first dimension size of the lattice
 * @param L1 (side_t) second dimension size of the lattice
 * @return (nnl_t) nearest neighbors struct
 */
extern nnl_t compute_nn(sysz_t i, side_t L1, side_t L2)
{
    nnl_t nn;
    if (i >= L1)
        nn.N = i - L1;
    else
        nn.N = i + L1 * (L2 - 1);
    if (i < L1 * (side_t)(L2 - 1))
        nn.S = i + L1;
    else
        nn.S = i % L1;
    if (i % L1)
        nn.W = i - 1;
    else
        nn.W = i + (L1 - 1);
    if ((i+1) % L1)
        nn.E = i + 1;
    else
        nn.E = i - (L1 - 1);
    return nn;
}
/**
 * compute the nearest neighbor array
 * @param L1 (side_t) first dimension size of the lattice
 * @param L2 (side_t) second dimension size of the lattice
 * @param nn (nnl_t *) the nearest neighbors array
 * @return none
 */
extern void __compute_nn_array(side_t L1, side_t L2, nnl_t *nn)
{
    for (sysz_t i = 0; i < L1 * L2; i++)
        nn[i] = compute_nn(i, L1, L2);
}
/*///////////////////////////////////////////////////// I/O LATTICE FUNCTIONS */
//
/**
 * print the lattice on the screen
 * @param N (sysz_t) size of the lattice
 * @param L1 (side_t) first dimension size of the lattice
 * @param s (hlttc_t *) the lattice array
 * @return none
 */
extern void __print_CONFIG(sysz_t N, side_t L1, hlttc_t *s)
{
    for (sysz_t i = 0; i < N; i++)
    {
        PRIconf(s[i]);
        if (!((i + 1) % L1))
            printf("\n");
    }
}
/**
 * print on file the lattice on the screen
 * @param fn (char *) string with the filename
 * @param N (sysz_t) size of the lattice
 * @param L1 (side_t) first dimension size of the lattice
 * @param s (hlttc_t *) the lattice array
 * @return none
 */
extern void __fprint_CONFIG(char *fn, sysz_t N, side_t L1, hlttc_t *s)
{
    FILE *fout;
    __F_OPEN(&fout, fn, "w+");
    for (sysz_t i = 0; i < N; i++)
    {
        FPRIconf(s[i], fout);
        if (!((i + 1) % L1))
            fprintf(fout, "\n");
    }
    fclose(fout);
}
/**
 * write on file the lattice on the screen in binary (int8) format
 * @param fn (char *) string with the filename
 * @param t (sysz_t) MC time
 * @param N (sysz_t) size of the lattice
 * @param s (hlttc_t *) the lattice array
 * @return none
 */
extern void __fwrite_CONFIG(char *fn, sysz_t t, sysz_t N, hlttc_t *s)
{
    FILE *fout;
    sprintf(buf, "%sSCONF_t=%" PRIu32 EXTBIN, fn, t);
    __F_OPEN(&fout, buf, "wb");
    fwrite(s, sizeof(*s), N, fout);
    fclose(fout);
}



/*////////////////////////////////////////////////////////// I/O CONFIG FILES */
/*
*/

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
    {
        *__ptrinit__ = NULL;
        // e mettici un maledetto messaggio di errore
    }
    if (strcmp(upd_mode, _M_MEHA_SS) == 0)
        *__ptrupd__ = __upd_MEHA_ss;
    else if (strcmp(upd_mode, _M_MEHA_SA) == 0)
        *__ptrupd__ = __upd_MEHA_sa;
    else // if (strcmp(d.MODE_init, "algo_wolff") == 0)
    {
        *__ptrupd__ = NULL;
        // e mettici un maledetto messaggio di errore
    }
}

/** acquire from config file the d struct with allthe settings for the program
 * @param fc (FILE **) configuration file pointer defereced
 * @param d (dtc_t *) the data struct for config files
 * @return (void) none
 */
extern void __fscanf_dtc(FILE **fc, dtc_t *d)
{
    char *ep;
    __get_row_fgets(fc, row);
    d->tMC = strtou32(strtok(row, ","));
    d->N_M = strtou32(STR_TOK_COMMA);
    d->K = strtou16(STR_TOK_COMMA);
    d->_m_sav = strtou32(STR_TOK_COMMA);
    d->Navg = strtou16(STR_TOK_COMMA);
    d->L1 = strtou16(STR_TOK_COMMA);
    d->L2 = strtou16(STR_TOK_COMMA);
    d->Ls = strtou16(STR_TOK_COMMA);
    d->b_m = strtod(STR_TOK_COMMA, &ep);
    d->b_M = strtod(STR_TOK_COMMA, &ep);
    d->b_s = strtod(STR_TOK_COMMA, &ep);
    d->_m_mea = (bool) strtol(STR_TOK_COMMA, (char **)NULL, 10);
    strcpy(d->_m_init, STR_TOK_COMMA);
    strcpy(d->_m_upd, STR_TOK_COMMA);
}
/** scan from configuration file the single realization struct
 * @param fc (FILE **) configuration file pointer defereced
 * @param d (smdtc_t *) the data struct for single config files
 * @return (void) none
 */
extern void __fscanf_smdtc(FILE **fc, smdtc_t *d)
{
    char *ep;
    __get_row_fgets(fc, row);
    d->tMC = strtou32(strtok(row, ","));
    d->L1 = strtou16(STR_TOK_COMMA);
    d->L2 = strtou16(STR_TOK_COMMA);
    d->N = d->L1 * d->L2;
    d->tMC = d->N;
    d->Navg = strtou16(STR_TOK_COMMA);
    d->b = strtod(STR_TOK_COMMA, &ep);;
    strcpy(d->_m_init, STR_TOK_COMMA);
    strcpy(d->_m_upd, STR_TOK_COMMA);
}
extern void __printf_dtc(dtc_t d)
{
    printf(_V _LARW __ "CONFIGURATION FILE CONTENTS" _N);
    printf(_T "tMC" _T "%" SCNu32"\n", d.tMC);
    printf(_T "N_M\t%" SCNu32"\n", d.N_M);
    printf(_T "K\t%" SCNu32"\n", d.K);
    printf(_T "_m_sav\t%" SCNu32"\n", d._m_sav);
    printf(_T "Navg\t%" SCNu16"\n", d.Navg);
    printf(_T "L1\t%" SCNu16"\n", d.L1);
    printf(_T "L2\t%" SCNu16"\n", d.L2);
    printf(_T "Ls\t%" SCNu16"\n", d.Ls);
    printf(_T "b_m\t%.3g\n", d.b_m);
    printf(_T "b_M\t%.3g\n", d.b_M);
    printf(_T "b_s\t%.3g\n", d.b_s);
    printf(_T "_m_init\t%s\n", d._m_init);
    printf(_T "_m_upd\t%s\n", d._m_upd);
    printf(_T "_m_mea\t%d\n", d._m_mea);
}
extern void __printf_smdtc(smdtc_t d)
{
    printf(_V _LARW __ "CONFIGURATION FILE CONTENTS" _N);
    printf(_T "tMC" _T "%" SCNu32"\n", d.tMC);
    printf(_T "K\t%" SCNu32"\n", d.K);
    printf(_T "_m_sav\t%" SCNu32"\n", d._m_sav);
    printf(_T "Navg\t%" SCNu16"\n", d.Navg);
    printf(_T "L1\t%" SCNu16"\n", d.L1);
    printf(_T "L2\t%" SCNu16"\n", d.L2);
    printf(_T "_m_init\t%s\n", d._m_init);
    printf(_T "_m_upd\t%s\n", d._m_upd);
    printf(_T "_m_mea\t%d\n", d._m_mea);
}
extern void __read_dtc(char *config_fn, dtc_t *d)
{
    FILE *fconf;
    __F_OPEN(&fconf, config_fn, "r+");
    __get_row_fgets(&fconf, row);
    __fscanf_dtc(&fconf, d);
    fclose(fconf);
}
extern void __read_smdtc(char *config_fn, smdtc_t *d)
{
    FILE *fconf;
    __F_OPEN(&fconf, config_fn, "r+");
    __get_row_fgets(&fconf, row);
    __fscanf_smdtc(&fconf, d);
    fclose(fconf);
}




extern void __set_localdtc(smdtc_t *d1, dtc_t *d)
{
    d1->L1 = d->L1;
    d1->L2 = d->L2;
    d1->N = d->L1 * d->L2;
    d1->Navg = d->Navg;
    d1->tMC = d->tMC;
    d1->_m_mea = d->_m_mea;
    d1->_m_sav = d->_m_sav;
    strcpy(d1->_m_init, d->_m_init);
    strcpy(d1->_m_upd, d->_m_upd);
}
extern void __upd_localdtc(smdtc_t *d1, double b)
{           
    d1->b = b;
}



/**
 * ...
 * @param
 * @return
 */
extern void __mkdir_MOD_syszL1L2(const char *mode, char *_dirsz, smdtc_t d)
{
    if (d.L1 == d.L2)
    {
        if (strcmp(mode, "--acf") == 0)
            sprintf(_dirsz, DIRobs __NIS__ _H "%s" _H "%s" _S, d.L1*d.L2, d._m_init, d._m_upd);
        else if (strcmp(mode, "--gen_config") == 0)
            sprintf(_dirsz, DIRvbc __NIS__ _H "%s" _H "%s" _S, d.L1*d.L2, d._m_init, d._m_upd);
        else if (strcmp(mode, "--gen_unconf") == 0)
            sprintf(_dirsz, DIRunc "%s" _H "%s" _S, d._m_init, d._m_upd);
        else if (strcmp(mode, "--gen_kconf") == 0)
            sprintf(_dirsz, DIRunc);
    }
    else
    {
        if (strcmp(mode, "--acf") == 0)
            sprintf(_dirsz, DIRobs __L1IS_L2IS__ _H "%s" _H "%s" _S, d.L1, d.L2, d._m_init, d._m_upd);
        else if (strcmp(mode, "--gen_config") == 0)
            sprintf(_dirsz, DIRvbc __L1IS_L2IS__ _H "%s" _H "%s" _S, d.L1, d.L2, d._m_init, d._m_upd);
        else if (strcmp(mode, "--gen_unconf") == 0)
            sprintf(_dirsz, DIRunc "%s" _H "%s" _S,d._m_init, d._m_upd);
    }
    mkdir(_dirsz, ACCESSPERMS);
}

/**
 * ...
 * @param
 * @return
 */
extern void __mkdir_syszb(char *_dirb, double beta, char *_dirsz)
{
    sprintf(_dirb, "%s" __BTIS__ _S, _dirsz, beta);
    mkdir(_dirb, ACCESSPERMS);
}
/**
 * ...
 * @param
 * @return
 */
extern uint32_t countNsteps(dtc_t d)
{
    sysz_t N = d.L1 * d.L2;
    uint32_t Nsteps = 0;
    side_t L1 = d.L1;
    side_t L2 = d.L2, Ls = d.Ls;
    for (sysz_t sN = N; sN <= d.N_M; sN += L1 * Ls + L2 * Ls + Ls * Ls)
    {
        L1 += Ls;
        L2 += Ls;
        Nsteps++;
    }
    return Nsteps;
}
/**
 * ...
 * @param
 * @return
 */
extern uint32_t countbetasteps(dtc_t d)
{
    uint32_t betas = 0;
    for (double b = d.b_m; b < d.b_M; b += d.b_s)
        betas++;
    return betas;
}



/**
 * compute and return the variation in the energy of the system when spin at
 * site u flips
 * @param u (sysz_t) updated site
 * @param s (hlttc_t *) the lattice array
 * @param nn (nnl_t) the nearest neighbors of the site
 * @return difference of enegry, if negative accept the proposed move
 */
extern double dE(sysz_t u, hlttc_t *s, nnl_t nn)
{
    double dE, sum_nn;
    sum_nn = s[nn.N] + s[nn.S] + s[nn.W] + s[nn.E];
    dE = 2 * J * s[u] * sum_nn;
    return dE;
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
/**
 * ...
 * @param
 * @return
 */
extern void __upd_MEHA_ss(double beta, sysz_t N, hlttc_t *s, nnl_t *nn)
{
    double dEtmp;
    for (sysz_t u = 0; u < N; u++)
    {
        dEtmp = dE(u, s, nn[u]);
        if (dEtmp <= 0)
            s[u] = -s[u];
        else if (RNG_dbl() < exp(-dEtmp * beta))
            s[u] = -s[u];
    }
}
/**
 * ...
 * @param
 * @return
 */
extern void __upd_MEHA_sa(double beta, sysz_t N, hlttc_t *s, nnl_t *nn)
{
    sysz_t u;
    double dEtmp;
    for (sysz_t i = 0; i < N; i++)
    {
        u = RNG_u64() % N;
        dEtmp = dE(u, s, nn[u]);
        if (dEtmp <= 0)
            s[u] = -s[u];
        else if (RNG_dbl() < exp(-dEtmp * beta))
            s[u] = -s[u];
    }
}


extern void __setsave_time(smdtc_t d, sysz_t *st)
{
    if (d._m_sav)
        *st = d._m_sav;
    else
        *st = d.tMC + 1;
}
extern void __setmeasureOBS(smdtc_t d, vtmpf_t **mf)
{
    if (d._m_mea)
        *mf = __measure_OBS;
    else 
        *mf = __splash__;
}
/**
 * generate Ising 2D lattice configuration(s) following instructions provided in
 * configuration file specified by string config_fn
 * @param config_fn configuration file name
 * @return difference of enegry, if negative accept the proposed move
 */
extern void __gen_config_(smdtc_t d, obs_t O)
{
    char _dirsz[256], _dirb[512];
    uint32_t save_time;
    hlttc_t *s;
    nnl_t *nn;
    vtmpf_t *__init__, *__upd__, *__measure__;
    /*//////////////////////////////////////////////////// allocate variables */
    d.tMC *= d.N;
    __alloc_fill_snn(&s, &nn, d);
    /*///////////////////////////////////////////// create folder for results */
    __setfunc__init__upd__(d._m_init, &__init__, d._m_upd, &__upd__);
    __setsave_time(d, &save_time);
    __setmeasureOBS(d, &__measure__);
    /*///////////////////////////////////////////// create folder for results */
    __mkdir_MOD_syszL1L2(MODE_GENCON, _dirsz, d);
    __mkdir_syszb(_dirb, d.b, _dirsz);
    __init__(d.N, s);
    for (sysz_t t = 1; t < d.tMC + 1; t++)
    {
        __measure__(t - 1, d.N, s, O);
        __upd__(d.b, d.N, s, nn);
        if (!(t % save_time))
            __fwrite_CONFIG(_dirb, t, d.N, s);
    }
    d.tMC /= d.N;
    free(s);
    free(nn);
}
/** allocate the lattice and fill the nearest neighbour struct
 * @param s (hlttc_t **) double pointer to lattice
 * @param nn (nnl_t **) double pointer to nearest neighbour struct 
 */

extern void __alloc_fill_snn(hlttc_t **s, nnl_t **nn, smdtc_t d)
{
    sysz_t N = d.L1 * d.L2;
    (*s) = malloc(sizeof(**s) * N);
    __challoc(*s);
    (*nn) = malloc(sizeof(**nn) * N);
    __challoc(*nn);
    __compute_nn_array(d.L1, d.L2, *nn);
}



extern void __readTauint_tmp(smdtc_t *d1, dtc_t d, side_t L1, side_t L2)
{
    FILE *fti;
    char _diracfti[STR512];
    double tauint;
    __mkdir_obsN(_diracfti, *d1);
    sprintf(buf, DIRobs __NIS__ _H "%s" _H "%s" _S __BTIS__ _S "tauint_avg=" FMT_na EXTBIN,
            L1*L2, d._m_init, d._m_upd, d1->b, d.Navg);
    __F_OPEN(&fti, buf, "rb");
    if (fread(&tauint, sizeof(double), ONE, fti) != ONE)
    {
        printf("PROBLEM\n");
    }
    d1->_m_sav = ceil(tauint);
    printf("save = %u, tauint = %lf\n", d1->_m_sav, ceil(tauint));
    fclose(fti);
}

extern void __genUNcorr_CONFIG(char *config_fn)
{
    char _dirsz[256];
    dtc_t d;
    smdtc_t d1;
    side_t L1, L2, Ls;
    sysz_t N;
    __read_dtc(config_fn, &d);
    __set_localdtc(&d1, &d);
    L1 = d.L1;
    L2 = d.L2;
    Ls = d.Ls;
    N = (L1 * L2);
    while (N <= d.N_M)
    {
        __mkdir_MOD_syszL1L2(MODE_GENUNC, _dirsz, d1);
        for (double b = d.b_m; b < d.b_M; b += d.b_s)
        {
            __upd_localdtc(&d1, b);
            __readTauint_tmp(&d1, d, L1, L2);
            // read tauint and use it as savetime, tMC = savetime*K
            d1.tMC = d1._m_sav * d.K;
            printf("K = %u tmc = %u, svt = %u\n", d.K, d1.tMC, d1._m_sav);
            for (uint32_t av = 0; av < d.K; av++)//here use not dnavg ma 
                __wbrite_nconf_d(d1);
        }
        L1 += Ls;
        L2 += Ls;
        N = (L1 * L2);
    }
}












extern void __translate_dsNb_smdtc(dsNb_t d, smdtc_t *d1)
{
    d1->L1 = sqrt(d.N);
    d1->L2 = sqrt(d.N);
    d1->tMC = d.N;
    d1->b = d.b;
    d1->_m_sav = d._m_sav;
    strcpy(d1->_m_init, d._m_init);
    strcpy(d1->_m_upd, d._m_upd);
}
extern void __translate_smdtc_dsNb(dsNb_t *d1, smdtc_t d)
{
    d1->N = d.L1 * d.L2;
    d1->b = d.b;
    strcpy(d1->_m_init, d._m_init);
    strcpy(d1->_m_upd, d._m_upd);
}

/** generate a square lattice Ising configuration for measuring observables 
 * needed for auto-correlation function and integrated auto-correlation time
 * @param d (smdtc_t) the single realization struct
 * @param O (obs_t) the observable struct for computing magnetization
 * @return (void) none
 */
extern void __gen__obs_acfN(smdtc_t d, obs_t O)
{
    hlttc_t *s;
    nnl_t *nn;
    vtmpf_t *__init__, *__upd__;
    vtmpf_t *__measure__ = __measure_OBS;
    /**/
    __alloc_fill_snn(&s, &nn, d);
    __setfunc__init__upd__(d._m_init, &__init__, d._m_upd, &__upd__);
    /**/
    __init__(d.N, s);
    for (sysz_t t = 1; t < d.N; t++)
    {
        __measure__(t - 1, d.N, s, O);
        __upd__(d.b, d.N, s, nn);
    }
    free(s);
    free(nn);
}


/** create string path to observable folder with configurational parameters
 * @param _dirat (char *) the string onto which sprint the path
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __sprintf_obsN(char *_dirat, smdtc_t d)
{
    sprintf(_dirat, DIRobs __NIS__ _H "%s" _H "%s" _S,
        d.N, d._m_init, d._m_upd);
}
/** create string path to observable folder with configurational parameters
 * @param _dirat (char *) the string onto which sprint the path
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __sprintf_obsNb(char *_dirat, smdtc_t d)
{
    sprintf(_dirat, DIRobs __NIS__ _H "%s" _H "%s" _S __BTIS__ _S,
        d.N, d._m_init, d._m_upd, d.b);
}

/** create string path to observable folder with configurational parameters and
 * make directory if it does not exists
 * @param _dirat (char *) the string onto which sprint the path
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __mkdir_obsN(char *_dirat, smdtc_t d)
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
extern void __makeTauint(smdtc_t d)
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
/** read integrated correlation time with configuration set by (smdtc_t) struct
 * and store inside it
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __readTauint(smdtc_t *d)
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
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __ifNETauint_makeit(smdtc_t d)
{
    char _dirsti[STR512];
    __mkdir_obsN(_dirsti, d);
    printf("%u\n", d.Navg);
    sprintf(buf, "%stauint_avg=" FMT_na EXTBIN, _dirsti, d.Navg);
    if (F_NEXIST(buf))
        __makeTauint(d);
}
/** set the tauint variable inside the single configuration file
 * @param d (smdtc_t) the single realization struct
 * @return (void) none
 */
extern void __Tauint(smdtc_t d)
{
    __ifNETauint_makeit(d);
    __readTauint(&d);
}

extern void __make_buf_cgfpth(smdtc_t d, char *_dirsz, char *P_TYPE)
{
    sprintf(buf, "%s%s" _U __NIS__ _U __TIS__ _U _FMT_K_ EXTBIN, 
            _dirsz, P_TYPE, d.N, 1. / d.b, d.eK);
}

extern void __rename_cfgpth(smdtc_t d, char *_dirsz, char *P_TYPE)
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
extern void __wbrite_nconf_d(smdtc_t d)
{
    FILE *fo;
    char _dirsz[STR256];
    char P_TYPE[STR256];
    hlttc_t *s;
    nnl_t *nn;
    sysz_t St;
    vtmpf_t *__init__, *__upd__;
    /**/
    d.tMC = d.K * d._m_sav;
    __alloc_fill_snn(&s, &nn, d);
    __setfunc__init__upd__(d._m_init, &__init__, d._m_upd, &__upd__);
    __setsave_time(d, &St);
    /**/
    __mkdir_MOD_syszL1L2(MODE_KGENCN, _dirsz, d);
    __find_effKCFGS(&d);
    __get_P_TYPE(d, P_TYPE);
    __make_buf_cgfpth(d, _dirsz, P_TYPE);
    __F_OPEN_SEEKEND(&fo, buf, "a+b");
    /**/
    for (sysz_t i = 0; i < d.mK; i++)
    {
        __init__(d.N, s);
        for (sysz_t t = 0; t < St; t++)
            __upd__(d.b, d.N, s, nn);
        __fwrite_cfg(&fo, d.N, s);
    }
    free(s);
    free(nn);
    fclose(fo);
    /**/
    __rename_cfgpth(d, _dirsz, P_TYPE);
}


extern void __get_P_TYPE(smdtc_t d, char *ptype)
{
    if (strcmp(d._m_init, _M_HSU) == 0)
    {
        if (strcmp(d._m_upd, _M_MEHA_SA) == 0)
            sprintf(ptype, ISING2DHSSA);
        else if (strcmp(d._m_upd, _M_MEHA_SS) == 0)
            sprintf(ptype, ISING2DHSSS);
    }
    else if (strcmp(d._m_init, _M_CSU) == 0)
    {
        if (strcmp(d._m_upd, _M_MEHA_SA) == 0)
            sprintf(ptype, ISING2DCSSA);
        else if (strcmp(d._m_upd, _M_MEHA_SS) == 0)
            sprintf(ptype, ISING2DCSSS);
    }
}

extern void __find_effKCFGS(smdtc_t *d)
{
    FILE *pipe;
    char P_TYPE[STR256];
    __get_P_TYPE(*d, P_TYPE);
    sprintf(buf, "find " DIRunc " -maxdepth 1 -name %s" _U __NIS__ _U __TIS__ _U "*",
        P_TYPE, d->N, 1. / d->b);
    if ((pipe = popen(buf, "r")) == NULL)
    {
        perror("PIPING ERROR");
        exit(EXIT_FAILURE);
    }
    ;
    if (fgets(buf, sizeof(buf), pipe) == NULL)
    {
        ;
    }
    buf[strlen(buf) - 1] = '\0';
    pclose(pipe);
    d->eK = d->K;
    d->mK = d->K;
    if (!(strncmp(buf, DIRunc, strlen(DIRunc))))
    {
        sprintf(buf1, DIRunc "%s" _U __NIS__ _U __TIS__ _U "[", P_TYPE, d->N, 1. / d->b);
        strncpy(buf2, buf + strlen(buf1), strlen(buf) - strlen(buf1) - 5);
        d->eK = strtou32(buf2);
        d->mK = softplus_u32(d->K - d->eK);
    }
}







//TRASH
// extern void updWO(double T, sysz_t N, hlttc_t *s, nnl_t *nn)
// {
//     //    int j, nn[Z];
//     //    s[i] = - s0;                    /* flip the spin immediateL2 */
//     //    neighbor(i, nn);                /* find nearest neighbor of i */
//     //    for(j = 0; j < Z; ++j)          /* flip the neighbor if ...  */
//     //       if(s0 == s[nn[j]] && RNG_dbl() < p)
//     //          flip(nn[j], s0);
// }
// #include <stdio.h>
// #include <sys/stat.h>
// #include <stdlib.h>
// #include <math.h>
// #include <string.h>
// #include <inttypes.h>
// #include <imdefs.h>
// #include <imtdlib.h>
// #include <imfnlib.h>
// #include <imrng.h>
// /**
//  * initialize the lattice with Rademacher variables, i.e. {-1, +1}
//  * with p = 1/2
//  * @param N size of the lattice
//  * @param s the lattice array
//  * @return none
//  */
// extern void __init_hotstart_uniform(sysz_t N, hlttc_t *s)
// {
//     for (sysz_t i = 0; i < N; i++)
//         s[i] = TWO * (RNG_u64() % TWO) - 1;
// }
// /**
//  * initialize the lattice with a cold start, i.e. all the spins equal to +1 (-1)
//  * with p = 1/2
//  * @param N size of the lattice
//  * @param s the lattice array
//  * @return none
//  */
// extern void __init_coldstart(sysz_t N, hlttc_t *s)
// {
//     s[0] = TWO * (RNG_u64() % TWO) - 1;
//     for (sysz_t i = 1; i < N; i++)
//         s[i] = s[0];
// }
// /**
//  * print the lattice on the screen
//  * @param N size of the lattice
//  * @param L1 horizontal side size of the lattice
//  * @param s the lattice array
//  * @return none
//  */
// extern void __print_CONFIG(sysz_t N, side_t L1, hlttc_t *s)
// {
//     for (sysz_t i = 0; i < N; i++)
//     {
//         // printf("%+" PRIi8 " ", s[i]);
//         printf("%" PRIi8 " ", (s[i] + 1) / 2);
//         if (!((i + 1) % L1))
//             printf("\n");
//     }
// }
// /**
//  * print on file the lattice on the screen
//  * @param f fle onto which to print
//  * @param N size of the lattice
//  * @param L1 horizontal side size of the lattice
//  * @param s the lattice array
//  * @return none
//  */
// extern void fprintcfg(FILE **f, sysz_t N, side_t L1, hlttc_t *s)
// {
//     for (sysz_t i = 0; i < N; i++)
//     {
//         // printf("%+" PRIi8 " ", s[i]);
//         fprintf(*f, "%" PRIi8 " ", (s[i] + 1) / 2);
//         if (!((i + 1) % L1))
//             fprintf(*f, "\n");
//     }
// }
// /**
//  * write on file the lattice on the screen in binary (int8) format
//  * @param f fle onto which to print
//  * @param N size of the lattice
//  * @param s the lattice array
//  * @return none
//  */
// extern void fwritecfg(FILE **f, sysz_t N, hlttc_t *s)
// {
//     fwrite(s, sizeof(*s), N, *f);
// }
// /**
//  * compute the nearest neighbor given a specific site of the lattice
//  * @param L1 horizontal side size of the lattice
//  * @param L2 vertical side size of the lattice
//  * @param s the lattice site
//  * @param nn the nearest neighbor array
//  * @return none
//  */
// extern nnl_t compute_nn(sysz_t i, side_t L1, side_t L2)
// {
//     nnl_t nn;
//     // north
//     if (i > L1)
//         nn.N = i - L1;
//     else
//         nn.N = i + L1 * (L2 - 1);
//     // south
//     if (i < L1 * (side_t)(L2 - 1))
//         nn.S = i + L1;
//     else
//         nn.S = i % L1;
//     // west
//     if (i % L1)
//         nn.W = i - 1;
//     else
//         nn.W = i + (L1 - 1);
//     // east
//     if ((+1) % L1)
//         nn.E = i + 1;
//     else
//         nn.E = i - (L1 - 1);
//     return nn;
// }
// /**
//  * compute the nearest neighbor array
//  * @param L1 horizontal side size of the lattice
//  * @param L2 vertical side size of the lattice
//  * @param nn the nearest neighbors array
//  * @return none
//  */
// extern void __compute_nn_array(side_t L1, side_t L2, nnl_t *nn)
// {
//     sysz_t N = L1 * L2;
//     for (sysz_t i = 0; i < N; i++)
//     {
//         nn[i] = compute_nn(i, L1, L2);
//     }
// }
// /**
//  * compute and return the magnetization of the system
//  * @param N size of the lattice
//  * @param s the lattice array
//  * @return none
//  */
// extern double m(sysz_t N, hlttc_t *s)
// {
//     double m = 0.;
//     for (sysz_t i = 0; i < N; i++)
//         m += s[i];
//     return m;
// }
// /**
//  * compute and return the variation in the energy of the system when spin at
//  * site u flips
//  * @param u updated site
//  * @param s the lattice array
//  * @param nn the nearest neighbors array
//  * @return difference of enegry, if negative accept the proposed move
//  */
// extern double dE(sysz_t u, hlttc_t *s, nnl_t nn)
// {
//     double dE, sum_nn;
//     sum_nn = s[nn.N] + s[nn.S] + s[nn.W] + s[nn.E];
//     dE = 2 * J * s[u] * sum_nn;
//     return dE;
// }
// extern void __upd_ME__scheme(double beta, sysz_t N, hlttc_t *s, nnl_t *nn)
// {
//     double dEtmp;
//     for (sysz_t u = 0; u < N; u++)
//     {
//         dEtmp = dE(u, s, nn[u]);
//         if (dEtmp <= 0)
//             s[u] = -s[u];
//         else if (exp(-dEtmp * beta) > RNG_dbl())
//             s[u] = -s[u];
//     }
// }
// // extern void updWO(double T, sysz_t N, hlttc_t *s, nnl_t *nn)
// // {
// //     //    int j, nn[Z];
// //     //    s[i] = - s0;                    /* flip the spin immediateL2 */
// //     //    neighbor(i, nn);                /* find nearest neighbor of i */
// //     //    for(j = 0; j < Z; ++j)          /* flip the neighbor if ...  */
// //     //       if(s0 == s[nn[j]] && RNG_dbl() < p)
// //     //          flip(nn[j], s0);
// // }
// /**
//  * ...
//  * @param config_fn configuration file name
//  * @return difference of enegry, if negative accept the proposed move
//  */
// extern dtc_t __fscanf_configfile(char *config_fn)
// {
//     FILE *fconf;
//     dtc_t dtc;
//     F_OPEN(&fconf, config_fn, "r+");
//     fscanf(fconf, "%" SCNu64, &dtc.tMCMC);
//     fscanf(fconf, "%" SCNu16, &dtc.L1);
//     fscanf(fconf, "%" SCNu16, &dtc.L2);
//     fscanf(fconf, "%lf", &dtc.b_m);
//     fscanf(fconf, "%lf", &dtc.b_M);
//     fscanf(fconf, "%lf", &dtc.b_s);
//     fscanf(fconf, "%s", dtc.MODE_init);
//     fscanf(fconf, "%s", dtc.MODE_upd);
//     fscanf(fconf, "%" SCNu64, &dtc._m_sav);
//     fclose(fconf);
//     return dtc;
// }
// /**
//  * generate Ising 2D lattice configuration(s) following instructions provided in
//  * configuration file specified by string config_fn
//  * @param config_fn configuration file name
//  * @return difference of enegry, if negative accept the proposed move
//  */
// extern void __gen_config_varbeta(char *config_fn)
// {
//     FILE *fout;
//     char dirsave[256], dirsave2[512];
//     uint64_t save_time;
//     side_t L1, L2;
//     sysz_t N;
//     hlttc_t *s;
//     nnl_t *nn;
//     dtc_t dtc;
//     void (*__init__)(), (*__upd__)();
//     /*////////////////////////////////////// seed the random number generator */
//     __setSFMT_seed_rand();
//     /*///////////////////////////////////////////// open and read config_file */
//     dtc = __fscanf_configfile(config_fn);
//     /*//////////////////////////////////////////////////// allocate variables */
//     L1 = dtc.L1;
//     L2 = dtc.L2;
//     N = (L1 * L2);
//     s = malloc(sizeof(*s) * N);
//     __challoc(s);
//     nn = malloc(sizeof(*nn) * N);
//     __challoc(nn);
//     __compute_nn_array(L1, L2, nn);
//     /*///////////////////////////////////////////// create folder for results */
//     if (L1 == L2)
//         sprintf(dirsave, DIRvbc "N=%" PRIu32 _S, N);
//     else
//         sprintf(dirsave, DIRvbc "L1=%" PRIu16 _U "L2=%" PRIu16 _S, L1, L2);
//     mkdir(dirsave, ACCESSPERMS);
//     /*///////////////////////////////////////////////////////// set init mode */
//     if (strcmp(dtc.MODE_init, "hs_unif") == 0)
//         __init__ = __init_hotstart_uniform;
//     else if (strcmp(dtc.MODE_init, "cs_unif") == 0)
//         __init__ = __init_coldstart;
//     else
//         __init__ = NULL;
//     __init__(N, s);
//     /*///////////////////////////////// set update mode (wolff or metropolis) */
//     if (strcmp(dtc.MODE_upd, "algo_metro") == 0)
//         __upd__ = __upd_ME__scheme;
//     else //if (strcmp(dtc.MODE_init, "algo_wolff") == 0)
//         __upd__ = NULL;
//     /*///////////////////////////////////////////////////////// set save mode */
//     if (dtc._m_sav)
//         save_time = dtc._m_sav;
//     else
//         save_time = dtc.tMCMC;
//     /*///////////////////////////////////////////////////////// set save mode */
//     for (double b = dtc.b_m; b < dtc.b_M; b += dtc.b_s)
//     {
//         sprintf(dirsave2, "%sbeta=" FMT_bt _S, dirsave, b);
//         mkdir(dirsave2, ACCESSPERMS);
//         __init__(N, s);
//         for (sysz_t t = 1; t < dtc.tMCMC + 1; t++)
//         {
//             // printf("\rt = %d", t);
//             __upd__(b, N, s, nn);
//             if (!(t % save_time))
//             {
//                 sprintf(buf, "%sSCONF_t=%" PRIu32 EXTBIN, dirsave2, t);
//                 F_OPEN(&fout, buf, "wb");
//                 fwritecfg(&fout, N, s);
//                 fclose(fout);
//             }
//         }
//         // printf("\n");
//     }
//     free(s);
//     free(nn);
// }