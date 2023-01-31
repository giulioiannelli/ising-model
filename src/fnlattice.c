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
// extern void printcfg(sysz_t N, side_t L1, lttc_t *s)
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
// extern void compute_nnarr(side_t L1, side_t L2, nnl_t *nn)
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
// extern void updME(double beta, sysz_t N, lttc_t *s, nnl_t *nn)
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
//     fscanf(fconf, "%lf", &dtc.beta_m);
//     fscanf(fconf, "%lf", &dtc.beta_M);
//     fscanf(fconf, "%lf", &dtc.beta_stp);
//     fscanf(fconf, "%s", dtc.MODE_init);
//     fscanf(fconf, "%s", dtc.MODE_upd);
//     fscanf(fconf, "%" SCNu64, &dtc.MODE_save);
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
//     challoc(s);
//     nn = malloc(sizeof(*nn) * N);
//     challoc(nn);
//     compute_nnarr(L1, L2, nn);
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
//         __upd__ = updME;
//     else //if (strcmp(dtc.MODE_init, "algo_wolff") == 0)
//         __upd__ = NULL;
//     /*///////////////////////////////////////////////////////// set save mode */
//     if (dtc.MODE_save)
//         save_time = dtc.MODE_save;
//     else
//         save_time = dtc.tMCMC;
//     /*///////////////////////////////////////////////////////// set save mode */
//     for (double b = dtc.beta_m; b < dtc.beta_M; b += dtc.beta_stp)
//     {
//         sprintf(dirsave2, "%sbeta=" STR_FMT_b _S, dirsave, b);
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
/**
 * generate Ising 2D lattice configuration(s) following instructions provided in
 * configuration file specified by string config_fn
 * @param config_fn configuration file name
 * @return difference of enegry, if negative accept the proposed move
 */
// extern void __compute_ACFtauInt(char *config_fn)
// {
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
 * initialize the lattice with Rademacher variables, i.e. {-1, +1}
 * with p = 1/2
 * @param N size of the lattice
 * @param s the lattice array
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
 * @param N size of the lattice
 * @param s the lattice array
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
 * @param N size of the lattice
 * @param L1 horizontal side size of the lattice
 * @param s the lattice array
 * @return none
 */
extern void printcfg(sysz_t N, side_t L1, lttc_t *s)
{
    for (sysz_t i = 0; i < N; i++)
    {
        // printf("%+" PRIi8 " ", s[i]);
        printf("%" PRIi8 " ", (s[i] + 1) / 2);
        if (!((i + 1) % L1))
            printf("\n");
    }
}
/**
 * print on file the lattice on the screen
 * @param f fle onto which to print
 * @param N size of the lattice
 * @param L1 horizontal side size of the lattice
 * @param s the lattice array
 * @return none
 */
extern void fprintcfg(FILE **f, sysz_t N, side_t L1, lttc_t *s)
{
    for (sysz_t i = 0; i < N; i++)
    {
        // printf("%+" PRIi8 " ", s[i]);
        fprintf(*f, "%" PRIi8 " ", (s[i] + 1) / 2);
        if (!((i + 1) % L1))
            fprintf(*f, "\n");
    }
}
/**
 * write on file the lattice on the screen in binary (int8) format
 * @param f fle onto which to print
 * @param N size of the lattice
 * @param s the lattice array
 * @return none
 */
extern void fwritecfg(FILE **f, sysz_t N, lttc_t *s)
{
    fwrite(s, sizeof(*s), N, *f);
}

/**
 * compute the nearest neighbor given a specific site of the lattice
 * @param L1 horizontal side size of the lattice
 * @param L2 vertical side size of the lattice
 * @param s the lattice site
 * @param nn the nearest neighbor array
 * @return none
 */
extern nnl_t compute_nn(sysz_t i, side_t L1, side_t L2)
{
    nnl_t nn;
    // north
    if (i > L1)
        nn.N = i - L1;
    else
        nn.N = i + L1 * (L2 - 1);
    // south
    if (i < L1 * (side_t)(L2 - 1))
        nn.S = i + L1;
    else
        nn.S = i % L1;
    // west
    if (i % L1)
        nn.W = i - 1;
    else
        nn.W = i + (L1 - 1);
    // east
    if ((+1) % L1)
        nn.E = i + 1;
    else
        nn.E = i - (L1 - 1);
    return nn;
}
/**
 * compute the nearest neighbor array
 * @param L1 horizontal side size of the lattice
 * @param L2 vertical side size of the lattice
 * @param nn the nearest neighbors array
 * @return none
 */
extern void compute_nnarr(side_t L1, side_t L2, nnl_t *nn)
{
    sysz_t N = L1 * L2;
    for (sysz_t i = 0; i < N; i++)
    {
        nn[i] = compute_nn(i, L1, L2);
    }
}
/**
 * compute and return the magnetization of the system
 * @param N size of the lattice
 * @param s the lattice array
 * @return none
 */
extern double m(sysz_t N, lttc_t *s)
{
    double m = 0.;
    for (sysz_t i = 0; i < N; i++)
        m += s[i];
    return m;
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
    return sum/tMC;
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
        sum2 += m[t]*m[t];
    return sum2/tMC;
}
/**
 * ...
 * @param 
 * @param 
 * @return 
 */
double m_corr_t(sysz_t tMC, sysz_t l, double *m)
{
    double sum = 0.;
    for (uint32_t t = 0; t < tMC - l; t++)
        sum += m[t] * m[t + l];
    return sum / (tMC - l);
}
/**
 * compute and return the variation in the energy of the system when spin at
 * site u flips
 * @param u updated site
 * @param s the lattice array
 * @param nn the nearest neighbors array
 * @return difference of enegry, if negative accept the proposed move
 */
extern double dE(sysz_t u, lttc_t *s, nnl_t nn)
{
    double dE, sum_nn;
    sum_nn = s[nn.N] + s[nn.S] + s[nn.W] + s[nn.E];
    dE = 2 * J * s[u] * sum_nn;
    return dE;
}
extern void updME(double beta, sysz_t N, lttc_t *s, nnl_t *nn)
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
// extern void updWO(double T, sysz_t N, lttc_t *s, nnl_t *nn)
// {
//     //    int j, nn[Z];

//     //    s[i] = - s0;                    /* flip the spin immediateL2 */
//     //    neighbor(i, nn);                /* find nearest neighbor of i */
//     //    for(j = 0; j < Z; ++j)          /* flip the neighbor if ...  */
//     //       if(s0 == s[nn[j]] && RNG_dbl() < p)
//     //          flip(nn[j], s0);
// }
extern sysz_t __countNsteps(dtc_t dtc)
{
    sysz_t N = dtc.L1 * dtc.L2;
    sysz_t Nsteps = 0;
    side_t L1 = dtc.L1;
    side_t L2 = dtc.L2, Ls = dtc.Ls;
    for (sysz_t sN = N; sN <= dtc.N_M; sN += L1*Ls+L2*Ls+Ls*Ls)
    {        
        L1 += Ls;
        L2 += Ls;
        Nsteps++;
    }
    return Nsteps;
}
extern sysz_t __countbetasteps(dtc_t dtc)
{
    sysz_t betas;
    for (double b = dtc.beta_m; b < dtc.beta_M; b += dtc.beta_stp)
        betas++;
    return betas;
}
extern void __dont_measure(obs_t O)
{
    ;
}
extern void measure(sysz_t t, sysz_t N, lttc_t *s, obs_t O)
{
    O.magn[t] = m(N, s);
}
/**
 * ...
 * @param config_fn configuration file name
 * @return difference of enegry, if negative accept the proposed move
 */
extern dtc_t __fscanf_configfile(char *config_fn)
{
    FILE *fconf;
    dtc_t dtc;
    int MODE_mea;
    F_OPEN(&fconf, config_fn, "r+");
    fscanf(fconf, "%" SCNu64, &dtc.tMCMC);
    fscanf(fconf, "%" SCNu16, &dtc.NAVG);
    fscanf(fconf, "%" SCNu16, &dtc.L1);
    fscanf(fconf, "%" SCNu16, &dtc.L2);
    fscanf(fconf, "%" SCNu16, &dtc.Ls);
    fscanf(fconf, "%" SCNu16, &dtc.N_M);
    fscanf(fconf, "%lf", &dtc.beta_m);
    fscanf(fconf, "%lf", &dtc.beta_M);
    fscanf(fconf, "%lf", &dtc.beta_stp);
    fscanf(fconf, "%s", dtc.MODE_init);
    fscanf(fconf, "%s", dtc.MODE_upd);
    fscanf(fconf, "%" SCNu64, &dtc.MODE_save);
    fscanf(fconf, "%d", &MODE_mea);
    dtc.MODE_measure = MODE_mea;
    fclose(fconf);
    return dtc;
}
/**
 * generate Ising 2D lattice configuration(s) following instructions provided in
 * configuration file specified by string config_fn
 * @param config_fn configuration file name
 * @return difference of enegry, if negative accept the proposed move
 */
extern obs_t __gen_config_(dtc1_t dtc)
{
    FILE *fout;
    char dirsave[256], dirsave2[512];
    double beta;
    uint64_t save_time;
    side_t L1, L2;
    sysz_t N;
    lttc_t *s;
    nnl_t *nn;
    obs_t O;
    void (*__init__)(), (*__upd__)(),  (*__measure__)();
    // /*///////////////////////////////////////////// open and read config_file */
    // dtc = __fscanf_configfile(config_fn);
    /*//////////////////////////////////////////////////// allocate variables */
    L1 = dtc.L1;
    L2 = dtc.L2;
    beta = dtc.beta;
    N = (L1 * L2);
    s = malloc(sizeof(*s) * N);
    challoc(s);
    nn = malloc(sizeof(*nn) * N);
    challoc(nn);
    compute_nnarr(L1, L2, nn);
    /*///////////////////////////////////////////// create folder for results */
    if (L1 == L2)
        sprintf(dirsave, DIRvbc "N=%" PRIu32 _S, N);
    else
        sprintf(dirsave, DIRvbc "L1=%" PRIu16 _U "L2=%" PRIu16 _S, L1, L2);
    mkdir(dirsave, ACCESSPERMS);
    /*///////////////////////////////////////////////////////// set init mode */
    if (strcmp(dtc.MODE_init, "hs_unif") == 0)
        __init__ = __init_hotstart_uniform;
    else if (strcmp(dtc.MODE_init, "cs_unif") == 0)
        __init__ = __init_coldstart;
    else
        __init__ = NULL;
    /*///////////////////////////////// set update mode (wolff or metropolis) */
    if (strcmp(dtc.MODE_upd, "algo_metro") == 0)
        __upd__ = updME;
    else //if (strcmp(dtc.MODE_init, "algo_wolff") == 0)
        __upd__ = NULL;
    /*///////////////////////////////////////////////////////// set save mode */
    if (dtc.MODE_save)
        save_time = dtc.MODE_save;
    else
        save_time = dtc.tMCMC;
    /*////////////////////////////////////////////////////// set measure mode */
    if (strcmp(dtc.MODE_measure, "algo_metro") == 0)
        __measure__ = measure;
    else //if (strcmp(dtc.MODE_init, "algo_wolff") == 0)
        __measure__ = __dont_measure;
    /*///////////////////////////////////////////////////////// set save mode */
    sprintf(dirsave2, "%sbeta=" STR_FMT_b _S, dirsave, beta);
    mkdir(dirsave2, ACCESSPERMS);
    __init__(N, s);
    for (sysz_t t = 1; t < dtc.tMCMC + 1; t++)
    {
        // printf("\rt = %d", t);
        __measure__(t-1, N, s, O);
        __upd__(beta, N, s, nn);
        if (!(t % save_time))
        {
            sprintf(buf, "%sSCONF_t=%" PRIu32 EXTBIN, dirsave2, t);
            F_OPEN(&fout, buf, "wb");
            fwritecfg(&fout, N, s);
            fclose(fout);
        }
    }
    // printf("\n");
    free(s);
    free(nn);
    return O;
}
extern void __compute_ACF(char *config_fn)
{
    FILE *fout;
    char dirsave[256], dirsave2[512];
    side_t L1, L2, Ls;
    sysz_t N, Ns;
    double betas, beta_m, beta_M, beta_stp;
    double *ACFcorr, *ACFcorr_tmp;
    dtc1_t dtc1;
    dtc_t dtc;
    obs_t O;
    /*///////////////////////////////// open and read config_file + variables */
    dtc = __fscanf_configfile(config_fn);
    L1 = dtc.L1;
    L2 = dtc.L2;
    Ls = dtc.Ls;
    beta_m = dtc.beta_m;
    beta_M = dtc.beta_M;
    beta_stp = dtc.beta_stp;
    N = (L1 * L2);
    Ns = __countNsteps(dtc);
    betas = __countbetasteps(dtc);
    dtc.MODE_save = dtc.tMCMC + 1;
    dtc1.tMCMC = dtc.tMCMC;
    dtc.MODE_measure = dtc.MODE_measure;
    strcpy(dtc1.MODE_init, dtc.MODE_init);
    strcpy(dtc1.MODE_upd, dtc.MODE_upd);
    /*/////////////////////////////////////////////////////////////////////// */
    for (sysz_t sN = N; sN <= dtc.N_M; sN += L1*Ls+L2*Ls+Ls*Ls)
    {
        if (L1 == L2)
            sprintf(dirsave, DIRobs "N=%" PRIu32 _S, sN);
        else
            sprintf(dirsave, DIRobs "L1=%" PRIu16 _U "L2=%" PRIu16 _S, L1, L2);
        mkdir(dirsave, ACCESSPERMS);
        for (double b = dtc.beta_m; b < dtc.beta_M; b += dtc.beta_stp)
        {
            sprintf(dirsave2, "%sbeta=" STR_FMT_b _S, dirsave, b);
            mkdir(dirsave2, ACCESSPERMS);
            dtc1.beta = b;
            dtc1.L1 = L1;
            dtc1.L2 = L2;
            ACFcorr = calloc(dtc.tMCMC, sizeof(*ACFcorr));
            for (uint32_t av = 0; av < dtc.NAVG; av++)
            {
                O = __gen_config_(dtc1);
                ACFcorr_tmp = __ACFcomputation(dtc1, O);
                for (uint16_t i = 0; i < dtc.NAVG; i++)
                    ACFcorr[i] += ACFcorr_tmp[i];
                free(ACFcorr_tmp);
            }
            for (sysz_t i = 0; i < dtc.NAVG; i++)
                    ACFcorr[i] /= dtc.NAVG;
            free(ACFcorr);
        }
        L1 += Ls;
        L2 += Ls;
    }
}

/**
 * compute autocorrelation time of configuration to be used for generating
 * independent configurations
 * @param config_fn configuration file name
 * @return difference of enegry, if negative accept the proposed move
 */
extern double *__ACFcomputation(dtc1_t dtc1, obs_t O)
{
    // devo generare configurazioni al variare di N e beta e poi salvare 
    // il contenuto, quindi devo creare files di configurazione, i files di 
    // il simtime deve scalare con dist(T, T_c) e la taglia del sistema
    sysz_t tMC, l;
    double *corr;
    double mavg, mavg2, m2avg;
    tMC = dtc1.tMCMC;
    mavg = m_avg(tMC, O.magn);
    mavg2 = mavg * mavg;
    m2avg = m_avg2(tMC, O.magn);
    corr = malloc(sizeof(*corr) * tMC);
    challoc(corr);
    l = 0;
    corr[l] = 1;
    while (l < tMC - 1)
        corr[l] = (m_corr_t(tMC, l, O.magn) - mavg2) / (m2avg - mavg2);
    return corr;
}

