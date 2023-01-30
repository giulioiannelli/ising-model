#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <time.h>
#include <SFMT.h>

// gcc -O3 -DSFMT_MEXP=19937 -o exe/a.o dep/SFMT/SFMT.c src/main.c -Wall -lm
#define TMCMC (10 * N)
#define TWO 2
#define LENSRND 4
#define SEED (uint32_t)0xBE11AC1A0
#define SIID (uint32_t)0xC1A0BE11A
#define LSEED (uint32_t)(time(0) ^ getpid())
#define RSEED (uint32_t)((time(0) << time(0)) ^ getpid())

#define RNG_u64 SFMTrng_u64
#define RNG_dbl SFMTrng_dbl

#define J 1

typedef int8_t lttc_t;
typedef uint16_t side_t;
typedef uint32_t sysz_t;
typedef struct nearestneigh
{
    uint32_t N;
    uint32_t S;
    uint32_t E;
    uint32_t W;
} nnl_t;

uint64_t SFMTrng_u64();
double SFMTrng_dbl();
void inits_randu(sysz_t, lttc_t *);
void printcfg(sysz_t, side_t, lttc_t *);
// void fprintcfg(FILE **, sysz_t, side_t, lttc_t *);
nnl_t compute_nn(sysz_t, side_t, side_t);
void compute_nnarr(side_t, side_t, nnl_t *);
double m(sysz_t, lttc_t *);
double dE(sysz_t, sysz_t, lttc_t *, nnl_t);
sysz_t upd(double, sysz_t, lttc_t *, nnl_t *);

uint32_t seed_rand[] = {SEED, SIID, 0, 0};
sfmt_t sfmt;
char buf[512];

int main(int argc, char *argv[])
{
    FILE *fp;
    double T;
    side_t Lx, Ly;
    sysz_t N;
    lttc_t *s;
    nnl_t *nn;
    // uint32_t tmpu32;
    double tmpdbl;
    //
    seed_rand[2] = LSEED;
    seed_rand[3] = RSEED;
    sfmt_init_by_array(&sfmt, seed_rand, LENSRND);
    //
    Lx = atoi(argv[1]);
    Ly = atoi(argv[2]);
    T = atof(argv[3]);
    
    //
    N = (Lx * Ly);
    s = malloc(sizeof(*s) * N);
    nn = malloc(sizeof(*nn) * N);
    inits_randu(N, s);
    sprintf(buf, "res/N=%" PRIu32 "/", N);
    mkdir(buf, ACCESSPERMS);
    //
    compute_nnarr(Lx, Ly, nn);
    tmpdbl = upd(T, N, s, nn);
    // printf("magn = %lf\n", m(N, s)/N);
    for (sysz_t t = 0; t < TMCMC; t++)
    {
        // printf("t = %d", t);
        // fflush(stdout);
        tmpdbl = upd(T, N, s, nn);
        // printf("acc = %lf, magn = %lf\n", tmpdbl/N, m(N, s)/N);
        // fprintcfg(fp, N, Lx, s);
        if (!(t % Lx))
        {
            sprintf(buf, "res/N=%" PRIu32 "/cfg_Lx=%" PRIu32 "_Ly=%" PRIu32 "_T=%.2lf_t=%" PRIu32".bin", N, Lx, Ly, T, t);
            fp = fopen(buf, "wb");
            fwrite(s, sizeof(*s), N, fp);
            fclose(fp);
        }
    }
    
    free(s);
    free(nn);

}
/**
 * generate and return 64-bit pseudorandom number.
 * init_gen_rand or init_by_array must be called before this function.
 * @param sfmt SFMT internal state
 * @return 64-bit pseudorandom number
 */
uint64_t SFMTrng_u64()
{
    return sfmt_genrand_uint64(&sfmt);
}
/**
 * generate and return a random number on [0,1) with 53-bit resolution
 * @param sfmt SFMT internal state
 * @return double on [0,1) with 53-bit resolution
 */
double SFMTrng_dbl()
{
    return sfmt_genrand_res53(&sfmt);
}
/**
 * initialize the lattice with Rademacher variables, i.e. {-1, +1}
 * with p = 1/2
 * @param N size of the lattice
 * @param s the lattice array
 * @return none
 */
void inits_randu(sysz_t N, lttc_t *s)
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
void printcfg(sysz_t N, side_t Lx, lttc_t *s)
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
// void fprintcfg(FILE **f, sysz_t N, side_t Lx, lttc_t *s)
// {
//     for (sysz_t i = 0; i < N; i++)
//     {
//         // printf("%+" PRIi8 " ", s[i]);
//         fprintf(&f, "%" PRIi8 " ", (s[i]+1)/2);
//         if (!((i + 1) % Lx))
//             pfrintf(&f, "\n");
//     }
// }
/**
 * compute the nearest neighbor given a specific site of the lattice
 * @param Lx horizontal side size of the lattice
 * @param Ly vertical side size of the lattice
 * @param s the lattice site
 * @param nn the nearest neighbor array
 * @return none
 */
nnl_t compute_nn(sysz_t i, side_t Lx, side_t Ly)
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
void compute_nnarr(side_t Lx, side_t Ly, nnl_t *nn)
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
double m(sysz_t N, lttc_t *s)
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
double dE(sysz_t u, sysz_t N, lttc_t *s, nnl_t nn)
{
    double dE, sum_nn;
    sum_nn = s[nn.N] + s[nn.S] + s[nn.W] + s[nn.E];
    dE = 2 * J * s[u] * sum_nn;
    return dE;
}
sysz_t upd(double T, sysz_t N, lttc_t *s, nnl_t *nn)
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