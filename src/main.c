// gcc -O3 -DSFMT_MEXP=19937 -o exe/a.o dep/SFMT/SFMT.c src/main.c -Wall -lm
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <time.h>
#include <SFMT.h>
#include <imdefs.h>
#include <imtdlib.h>
#include <imfnlib.h>
#include <imrng.h>

#define TMCMC (10 * N)

char buf[512];
sfmt_t sfmt;
uint32_t *seed_rand;

int main(int argc, char *argv[])
{
    if (argc != 3 + 1)
    {
        printf("ERROR");
        exit(EXIT_FAILURE);
    }
    FILE *fp;
    double T;
    side_t Lx, Ly;
    sysz_t N;
    lttc_t *s;
    nnl_t *nn;
    // uint32_t tmpu32;
    double tmpdbl;
    //
    seed_rand = (uint32_t [LENSRND]) {SEED, SIID, LSEED, RSEED};
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
    for (sysz_t t = 0; t < TMCMC; t++)
    {
        printf("\rt = %d", t);
        tmpdbl = updMC(T, N, s, nn);
        if (!(t % Lx))
        {
            sprintf(buf, "res/N=%" PRIu32 "/cfg_Lx=%" PRIu32 "_Ly=%" PRIu32 "_T=%.2lf_t=%" PRIu32".bin", N, Lx, Ly, T, t);
            fp = fopen(buf, "wb");
            fwritecfg(&fp, N, s);
            fclose(fp);
        }
    }
    printf("\n");
    
    free(s);
    free(nn);

}

