#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "imdefs.h"
#include "imtdlib.h"
#include "imfnlib.h"
#include "imrng.h"
/**
 * initialize the lattice with Rademacher variables, i.e. {-1, +1}
 * with p = 1/2
 * @param N size of the lattice
 * @param s the lattice array
 * @return none
 */
extern void inits_randu(sysz_t N, lttc_t *s)
{
    for (sysz_t i = 0; i < N; i++)
        s[i] = TWO * (RNG_u64() % TWO) - 1;
}
/**
 * print the lattice on the screen
 * @param N size of the lattice
 * @param Lx horizontal side size of the lattice
 * @param s the lattice array
 * @return none
 */
extern void printcfg(sysz_t N, side_t Lx, lttc_t *s)
{
    for (sysz_t i = 0; i < N; i++)
    {
        // printf("%+" PRIi8 " ", s[i]);
        printf("%" PRIi8 " ", (s[i]+1)/2);
        if (!((i + 1) % Lx))
            printf("\n");
    }
}
/**
 * print on file the lattice on the screen
 * @param f fle onto which to print
 * @param N size of the lattice
 * @param Lx horizontal side size of the lattice
 * @param s the lattice array
 * @return none
 */
extern void fprintcfg(FILE **f, sysz_t N, side_t Lx, lttc_t *s)
{
    for (sysz_t i = 0; i < N; i++)
    {
        // printf("%+" PRIi8 " ", s[i]);
        fprintf(*f, "%" PRIi8 " ", (s[i]+1)/2);
        if (!((i + 1) % Lx))
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
 * @param Lx horizontal side size of the lattice
 * @param Ly vertical side size of the lattice
 * @param s the lattice site
 * @param nn the nearest neighbor array
 * @return none
 */
extern nnl_t compute_nn(sysz_t i, side_t Lx, side_t Ly)
{
    nnl_t nn;
    // north
    if (i > Lx)
        nn.N = i - Lx;
    else
        nn.N = i + Lx * (Ly - 1);
    // south
    if (i < Lx * (Ly - 1))
        nn.S = i + Lx;
    else
        nn.S = i % Lx;
    // west
    if (i % Lx)
        nn.W = i - 1;
    else
        nn.W = i + (Lx - 1);
    // east
    if ((+1) % Lx)
        nn.E = i + 1;
    else
        nn.E = i - (Lx - 1);
    return nn;
}
/**
 * compute the nearest neighbor array
 * @param Lx horizontal side size of the lattice
 * @param Ly vertical side size of the lattice
 * @param nn the nearest neighbors array
 * @return none
 */
extern void compute_nnarr(side_t Lx, side_t Ly, nnl_t *nn)
{
    sysz_t N = Lx * Ly;
    for (sysz_t i = 0; i < N; i++)
    {
        nn[i] = compute_nn(i, Lx, Ly);
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
 * compute and return the variation in the energy of the system when spin at
 * site u flips
 * @param u updated site
 * @param N size of the lattice
 * @param s the lattice array
 * @param nn the nearest neighbors array
 * @return difference of enegry, if negative accept the proposed move
 */
extern double dE(sysz_t u, sysz_t N, lttc_t *s, nnl_t nn)
{
    double dE, sum_nn;
    sum_nn = s[nn.N] + s[nn.S] + s[nn.W] + s[nn.E];
    dE = 2 * J * s[u] * sum_nn;
    return dE;
}
extern sysz_t updMC(double T, sysz_t N, lttc_t *s, nnl_t *nn)
{
    sysz_t acc = 0;
    double dEtmp;
    for (sysz_t u = 0; u < N; u++)
    {
        dEtmp = dE(u, N, s, nn[u]);
        if (dEtmp <= 0)
        {
            s[u] = -s[u];
            acc++;
        }
        else if (exp(-dEtmp / T) > RNG_dbl())
        {
            s[u] = -s[u];
            acc++;
        }
    }
    return acc;
}