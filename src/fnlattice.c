#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <imdefs.h>
#include <imtdlib.h>
#include <imfnlib.h>
#include <imrng.h>
/**
 * initialize the lattice with a hot start (uniform distribution), i.e. spins
 * are Rademacher variables, {-1, +1}: p = 1/2
 * @param N (sysz_t) size of the lattice
 * @param s (lttc_t *) the lattice array
 * @return none
 */
extern void __init_hotstart_uniform(sysz_t N, lttc_t *s)
{
    for (sysz_t i = 0; i < N; i++)
        s[i] = TWO * (RNG_u64() % TWO) - 1;
}
/**
 * initialize the lattice with a cold start, i.e. all the spins equal to +1 (-1)
 * with p = 1/2
 * @param N (sysz_t) size of the lattice
 * @param s (lttc_t *) the lattice array
 * @return none
 */
extern void __init_coldstart(sysz_t N, lttc_t *s)
{
    s[0] = TWO * (RNG_u64() % TWO) - 1;
    for (sysz_t i = 1; i < N; i++)
        s[i] = s[0];
}
/**
 * print the lattice on the screen
 * @param N (sysz_t) size of the lattice
 * @param L1 (side_t) first dimension size of the lattice
 * @param s (lttc_t *) the lattice array
 * @return none
 */
extern void __print_CONFIG(sysz_t N, side_t L1, lttc_t *s)
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
 * @param s (lttc_t *) the lattice array
 * @return none
 */
extern void __fprint_CONFIG(char *fn, sysz_t N, side_t L1, lttc_t *s)
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
 * @param s (lttc_t *) the lattice array
 * @return none
 */
extern void __fwrite_CONFIG(char *fn, sysz_t t, sysz_t N, lttc_t *s)
{
    FILE *fout;
    sprintf(buf, "%sSCONF_t=%" PRIu32 EXTBIN, fn, t);
    __F_OPEN(&fout, buf, "wb");
    fwrite(s, sizeof(*s), N, fout);
    fclose(fout);
}
/**
 * write on file the ACF (auto-correlation function) of the system in binary
 * (double) format
 * @param fn (char *) string with the filename
 * @param tMC (sysz_t) length of correlation interval considered
 * @param acf (double *) the autocorrelation function array
 * @return none
 */
extern void __fwrite_ACF(char *fn, uint16_t na, sysz_t tMC, double *acf)
{
    FILE *fout;
    sprintf(buf, "%sACF_avg=%" PRIu16 EXTBIN, fn, na);
    __F_OPEN(&fout, buf, "wb");
    fwrite(acf, sizeof(*acf), tMC, fout);
    fclose(fout);
}
/**
 */
extern void __fwrite_ti(char *fn, uint16_t na, double ti)
{
    FILE *fout;
    sprintf(buf, "%stauint_avg=" FMT_na EXTBIN, fn, na);
    __F_OPEN(&fout, buf, "wb");
    fwrite(&ti, sizeof(ti), 1, fout);
    fclose(fout);
}
/**
 * acquire from config file the d struct with allthe settings for the program
 * @param config_fn (char *) configuration file name
 * @return (dtc_t) the struct with configurational settings
 */
extern void __fscanf_configfile(dtc_t *d, char *config_fn)
{
    // FILE *fconf;
    // int _mdmea;
    // __F_OPEN(&fconf, config_fn, "r+");
    // if (fscanf(fconf, "%" SCNu32 "%" SCNu32 "%" SCNu32,
    //            &d->tMC, &d->N_M, &d->_m_sav) != 3)
    //     printf("Error in config file\n");
    // if (fscanf(fconf, "%" SCNu16 "%" SCNu16 "%" SCNu16 "%" SCNu16,
    //            &d->Navg, &d->L1, &d->L2, &d->Ls) != 4)
    //     printf("Error in config file\n");
    // if (fscanf(fconf, "%lf%lf%lf", &d->b_m, &d->b_M, &d->b_s) != 3)
    //     printf("Error in config file\n");
    // if (fscanf(fconf, "%s", d->_m_init) != 1)
    //     printf("Error in config file\n");
    // if (fscanf(fconf, "%s", d->_m_upd) != 1)
    //     printf("Error in config file\n");
    // if (fscanf(fconf, "%d", &_mdmea) != 1)
    //     printf("Error in config file\n");
    // else
    //     d->_m_mea = _mdmea;
    // fclose(fconf);
    FILE *fconf;
    char row[1024];
    char *tok, *endptr;
    __F_OPEN(&fconf, config_fn, "r+");
    if (fgets(row, 1024, fconf) == NULL)
    {
        ;
    }
    if (fgets(row, 1024, fconf) == NULL)
    {
        ;
    }
    tok = strtok(row, ",");
    d->tMC = strtou32(tok);
    tok = strtok(NULL, ",");
    d->N_M = strtou32(tok);
    tok = strtok(NULL, ",");
    d->_m_sav = strtou32(tok);
    tok = strtok(NULL, ",");
    d->Navg = strtou16(tok);
    tok = strtok(NULL, ",");
    d->L1 = strtou16(tok);
    tok = strtok(NULL, ",");
    d->L2 = strtou16(tok);
    tok = strtok(NULL, ",");
    d->Ls = strtou16(tok);
    tok = strtok(NULL, ",");
    d->b_m = strtod(tok, &endptr);
    tok = strtok(NULL, ",");
    d->b_M = strtod(tok, &endptr);
    tok = strtok(NULL, ",");
    d->b_s = strtod(tok, &endptr);
    tok = strtok(NULL, ",");
    strcpy(d->_m_init, tok);
    tok = strtok(NULL, ",");
    strcpy(d->_m_upd, tok);
    tok = strtok(NULL, ",");
    d->_m_mea = (int) strtol(tok, (char **)NULL, 10);
    fclose(fconf);
}
extern void __set_localdtc(smdtc_t *d1, dtc_t *d)
{
    d1->tMC = d->tMC;
    d1->_m_mea = d->_m_mea;
    d1->_m_sav = d->_m_sav;
    strcpy(d1->_m_init, d->_m_init);
    strcpy(d1->_m_upd, d->_m_upd);
}
extern void __upd_localdtc(smdtc_t *d1, double b, side_t L1, side_t L2)
{           
    d1->b = b;
    d1->L1 = L1;
    d1->L2 = L2;
}
extern void __printf_configfile(dtc_t d, char *config_fn)
{
    __fscanf_configfile(&d, config_fn);
    printf(_V _LARW __ "CONFIGURATION FILE CONTENTS" _N);
    printf(_T "tMC" _T "%" SCNu32"\n", d.tMC);
    printf(_T "N_M\t%" SCNu32"\n", d.N_M);
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
extern void __print_configf(char *config_fn)
{
    dtc_t d;
    __fscanf_configfile(&d, config_fn);
    __printf_configfile(d, config_fn);
}

/**
 * ...
 * @param
 * @return
 */
extern void __mkdir_syszN(char *_dirsz, side_t L1, side_t L2, sysz_t N, smdtc_t d)
{
    if (L1 == L2)
    {
        if (strcmp(_dirsz, "--acf") == 0)
            sprintf(_dirsz, DIRobs "%s" _H "%s" __NIS__ _S, d._m_init, d._m_upd, N);
        else if (strcmp(_dirsz, "--gen_config") == 0)
            sprintf(_dirsz, DIRvbc "%s" _H "%s" __NIS__ _S, d._m_init, d._m_upd, N);
    }
    else
    {
        if (strcmp(_dirsz, "--acf") == 0)
            sprintf(_dirsz, DIRobs "%s" _H "%s" __L1IS_L2IS__ _S, d._m_init, d._m_upd, L1, L2);
        else if (strcmp(_dirsz, "--gen_config") == 0)
            sprintf(_dirsz, DIRvbc "%s" _H "%s" __L1IS_L2IS__ _S, d._m_init, d._m_upd, L1, L2);
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
 * compute the nearest neighbor given a specific site of the lattice
 * @param L1 (side_t) first dimension size of the lattice
 * @param L1 (side_t) second dimension size of the lattice
 * @param i (sysz_t) the lattice site
 * @return (nnl_t) nearest neighbors struct
 */
extern nnl_t compute_nn(sysz_t i, side_t L1, side_t L2)
{
    nnl_t nn;
    if (i >= L1) // north
        nn.N = i - L1;
    else
        nn.N = i + L1 * (L2 - 1);
    if (i < L1 * (side_t)(L2 - 1)) // south
        nn.S = i + L1;
    else
        nn.S = i % L1;
    if (i % L1) // west
        nn.W = i - 1;
    else
        nn.W = i + (L1 - 1);
    if ((i+1) % L1) // east
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
extern void compute_nn_array(side_t L1, side_t L2, nnl_t *nn)
{
    sysz_t N = L1 * L2;
    for (sysz_t i = 0; i < N; i++)
        nn[i] = compute_nn(i, L1, L2);
}
/**
 * compute and return the magnetization of the system
 * @param N (sysz_t) size of the lattice
 * @param s (lttc_t *) the lattice array
 * @return none
 */
extern double m(sysz_t N, lttc_t *s)
{
    double m = 0.;
    for (sysz_t i = 0; i < N; i++)
        m += s[i];
    return m/N;
}
/**
 * compute and return the average magnetization of the system over time
 * @param tMC (sysz_t) the montecarlo simulation time
 * @param m (double *) the magnetization array
 * @return the sum of the array over the length
 */
extern double m_avg(sysz_t tMC, double *m)
{
    double sum = 0.;
    for (sysz_t t = 0; t < tMC; t++)
        sum += m[t];
    return sum / tMC;
}
/**
 * compute and return the average magnetization of the system over time
 * @param tMC (sysz_t) the montecarlo simulation time
 * @param m (double *) the magnetization array
 * @return the sum of the array over the length
 */
extern double m_avg2(sysz_t tMC, double *m)
{
    double sum2 = 0.;
    for (sysz_t t = 0; t < tMC; t++)
        sum2 += m[t] * m[t];
    return sum2 / tMC;
}
/**
 * ...
 * @param tMC (sysz_t) the montecarlo simulation time
 * @param l (sysz_t) the temporal extension to which compute ACF
 * @param m (double *) the magnetization array
 * @return
 */
extern double m_corr_t(sysz_t tMC, sysz_t l, double *m)
{
    double sum = 0.;
    for (uint32_t t = 0; t < tMC - l; t++)
        sum += m[t] * m[t + l];
    return sum / (tMC - l);
}
/**
 * compute and return the variation in the energy of the system when spin at
 * site u flips
 * @param u (sysz_t) updated site
 * @param s (lttc_t *) the lattice array
 * @param nn (nnl_t) the nearest neighbors of the site
 * @return difference of enegry, if negative accept the proposed move
 */
extern double dE(sysz_t u, lttc_t *s, nnl_t nn)
{
    double dE, sum_nn;
    sum_nn = s[nn.N] + s[nn.S] + s[nn.W] + s[nn.E];
    dE = 2 * J * s[u] * sum_nn;
    return dE;
}
/**
 * compute autocorrelation time of configuration to be used for generating
 * independent configurations
 * @param config_fn configuration file name
 * @return difference of enegry, if negative accept the proposed move
 */
extern void __ACFcomputation(double **corr, smdtc_t d1, obs_t O)
{
    // devo generare configurazioni al variare di N e beta e poi salvare
    // il contenuto, quindi devo creare files di configurazione, i files di
    // il simtime deve scalare con dist(T, T_c) e la taglia del sistema
    sysz_t tMC, l;
    double mavg, mavg2, m2avg;
    tMC = d1.tMC * d1.L1 * d1.L2;
    mavg = m_avg(tMC, O.magn);
    m2avg = m_avg2(tMC, O.magn);
    mavg2 = mavg * mavg;
    *(*corr + (l = 0)) = 1;

    while (l < tMC - 1)
        *(*corr + l) = (m_corr_t(tMC, l++, O.magn) - mavg2) / (m2avg - mavg2);
}
/**
 * ...
 * @param
 * @return
 */
extern void __dont_measure(void)
{
    ;
}
/**
 * ...
 * @param
 * @return
 */
extern void __measure_OBS(sysz_t t, sysz_t N, lttc_t *s, obs_t O)
{
    O.magn[t] = m(N, s);
}
/**
 * ...
 * @param
 * @return
 */
extern void __upd_MEseq__scheme(double beta, sysz_t N, lttc_t *s, nnl_t *nn)
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
extern void __upd_MEasy__scheme(double beta, sysz_t N, lttc_t *s, nnl_t *nn)
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
/**
 * generate Ising 2D lattice configuration(s) following instructions provided in
 * configuration file specified by string config_fn
 * @param config_fn configuration file name
 * @return difference of enegry, if negative accept the proposed move
 */
extern void __gen_config_(smdtc_t d, obs_t O)
{
    char _dirsz[256], _dirb[512];
    double beta;
    uint32_t save_time;
    side_t L1, L2;
    sysz_t N, tMC;
    lttc_t *s;
    nnl_t *nn;
    void (*__init__)(), (*__upd__)(), (*__measure__)();
    // /*///////////////////////////////////////////// open and read config_file */
    // d = __fscanf_configfile(config_fn);
    /*//////////////////////////////////////////////////// allocate variables */
    L1 = d.L1;
    L2 = d.L2;
    beta = d.b;
    N = (L1 * L2);
    tMC = d.tMC * N;
    s = malloc(sizeof(*s) * N);
    __challoc(s);
    nn = malloc(sizeof(*nn) * N);
    __challoc(nn);
    compute_nn_array(L1, L2, nn);
    /*///////////////////////////////////////////// create folder for results */
    sprintf(_dirsz, "--gen_config");
    __mkdir_syszN(_dirsz, L1, L2, N, d);
    /*///////////////////////////////////////////////////////// set init mode */
    if (strcmp(d._m_init, "hs_unif") == 0)
        __init__ = __init_hotstart_uniform;
    else if (strcmp(d._m_init, "cs_unif") == 0)
        __init__ = __init_coldstart;
    else
        __init__ = NULL;
    /*///////////////////////////////// set update mode (wolff or metropolis) */
    if (strcmp(d._m_upd, "algo_metro_s") == 0)
        __upd__ = __upd_MEseq__scheme;
    else if (strcmp(d._m_upd, "algo_metro_a") == 0)
        __upd__ = __upd_MEasy__scheme;
    else // if (strcmp(d.MODE_init, "algo_wolff") == 0)
        __upd__ = NULL;
    /*///////////////////////////////////////////////////////// set save mode */
    if (d._m_sav)
        save_time = (uint32_t)(1./d._m_sav * N);
    else
        save_time = tMC + 1;
    /*////////////////////////////////////////////////////// set measure mode */
    if (strcmp(d._m_upd, "algo_metro") == 0)
        __measure__ = __measure_OBS;
    else // if (strcmp(d.MODE_init, "algo_wolff") == 0)
        __measure__ = __dont_measure;
    /*///////////////////////////////////////////// create folder for results */
    __mkdir_syszb(_dirb, beta, _dirsz);
    __init__(N, s);
    for (sysz_t t = 1; t < tMC + 1; t++) // printf("\rt = %d", t);
    {
        
        __measure__(t - 1, N, s, O);
        __upd__(beta, N, s, nn);
        if (!(t % save_time))
            __fwrite_CONFIG(_dirb, t, N, s);
    }
    free(s);
    free(nn);
}
extern void __compute_ACF(char *config_fn)
{
    char _dirsz[256], _dirb[512];
    side_t L1, L2, Ls;
    sysz_t N, tMC;
    double tauint;
    double *ACFcorr, *ACFcorr_cum, *ti;
    smdtc_t d1;
    dtc_t d;
    obs_t O;
    /*///////////////////////////////// open and read config_file + variables */
    __fscanf_configfile(&d, config_fn);
    __set_localdtc(&d1, &d);
    L1 = d.L1;
    L2 = d.L2;
    Ls = d.Ls;
    N = (L1 * L2);
    /*////////////////////////////////////////////////////////////////////////*/
    while (N <= d.N_M)
    {
        sprintf(_dirsz, "--acf");
        __mkdir_syszN(_dirsz, L1, L2, N, d1);
        for (double b = d.b_m; b < d.b_M; b += d.b_s)
        {
            __mkdir_syszb(_dirb, b, _dirsz);
            __upd_localdtc(&d1, b, L1, L2);
            tMC = d1.tMC * N;
            ti = malloc(sizeof(*ti) * d.Navg);
            O.magn = malloc(sizeof(*O.magn) * tMC);
            ACFcorr = calloc(tMC, sizeof(*ACFcorr));
            ACFcorr_cum = calloc(tMC, sizeof(*ACFcorr_cum));
            for (uint32_t av = 0; av < d.Navg; av++)
            {
                __gen_config_(d1, O);
                __ACFcomputation(&ACFcorr, d1, O);
                //
                for (sysz_t t = 0; t < tMC; t++)
                    ACFcorr_cum[t] += ACFcorr[t];
                sysz_t tt = 0;
                ti[av] = 1. / 2;
                while (ACFcorr[tt++] > 1/ M_E && tt < tMC)
                    ti[av] += ACFcorr[tt];
            }
            tauint = avg(d.Navg, ti);
            for (sysz_t t = 0; t < tMC; t++)
                ACFcorr_cum[t] /= d.Navg;
            __fwrite_ACF(_dirb, d.Navg, tMC, ACFcorr_cum);
            __fwrite_ti(_dirb, d.Navg, tauint);
            free(ACFcorr);
            free(ACFcorr_cum);
            free(O.magn);
            free(ti);
        }
        L1 += Ls;
        L2 += Ls;
        N = (L1 * L2);
    }
}

//TRASH
// extern void updWO(double T, sysz_t N, lttc_t *s, nnl_t *nn)
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
// extern void __init_hotstart_uniform(sysz_t N, lttc_t *s)
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
// extern void __init_coldstart(sysz_t N, lttc_t *s)
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
// extern void __print_CONFIG(sysz_t N, side_t L1, lttc_t *s)
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
// extern void fprintcfg(FILE **f, sysz_t N, side_t L1, lttc_t *s)
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
// extern void fwritecfg(FILE **f, sysz_t N, lttc_t *s)
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
// extern void compute_nn_array(side_t L1, side_t L2, nnl_t *nn)
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
// extern double m(sysz_t N, lttc_t *s)
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
// extern double dE(sysz_t u, lttc_t *s, nnl_t nn)
// {
//     double dE, sum_nn;
//     sum_nn = s[nn.N] + s[nn.S] + s[nn.W] + s[nn.E];
//     dE = 2 * J * s[u] * sum_nn;
//     return dE;
// }
// extern void __upd_ME__scheme(double beta, sysz_t N, lttc_t *s, nnl_t *nn)
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
// // extern void updWO(double T, sysz_t N, lttc_t *s, nnl_t *nn)
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
//     lttc_t *s;
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
//     compute_nn_array(L1, L2, nn);
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