// gcc -O3 -DSFMT_MEXP=19937 -o exe/a.o dep/SFMT/SFMT.c src/main.c -Wall -lm
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>
#include <SFMT.h>
#include <imdefs.h>
#include <imtdlib.h>
#include <imfnlib.h>
#include <imrng.h>

// #define NARG 2
#define PROGN "ising-model-c"

typedef struct modes_dict
{                   /* table entry: */
    char *__name__; /* defined name */
    void (*__mode__)();
} md_t;

char buf[1024];
char *MODES[] = {"--print_c", "--acf"}; //, "--gen_c"
sfmt_t sfmt;
uint32_t *seed_rand;
FILE *f_log;

int main(int argc, char *argv[])
{
    int MODE;
    md_t run[2];
    run[0].__mode__ = __print_configf;
    run[0].__name__ = MODES[0];
    run[1].__mode__ = __compute_acf;
    run[1].__name__ = MODES[1];
    /*///////////////////////////////////////////////////////// open log file */
    sprintf(buf, DIRlog PROGN _UCFG "%s" EXTLOG, argv[1] + strlen(DIRcfg) + 1);
    __MAKElog(argc, PROGN, argv);
    /*//////////////////////////// check correct number of argument is parsed */
    // __CHECKargc(argc, NARG);
    /*////////////////////////////////////// seed the random number generator */
    __setSFMT_seed_rand();
    /* execute program according to mode */
    for (int a = 2; a < argc; a++)
        if ((MODE = strIn_(MODES, ARRAY_SIZE(MODES), argv[a])))
            run[MODE - 1].__mode__(argv[1]);
    // if (strIn_(MODES, ARRAY_SIZE(MODES), argv[2]) == 0)
    //     __print_configf(argv[2], "stoutprint");
    // if (strcmp(argv[1], "--acf") == 0)
    //     __compute_acf(argv[2]);
    fclose(f_log);
}

// else if (strcmp(argv[1], "--gen_config") == 0){
//     d = __fscanf_configfile(argv[2]);
//     __gen_config_(d);}
// else if (strcmp(argv[1], "--measure") == 0)
//     measure(argv[2]);
// else
// {
//     printf("Mode %s not recognized\n", argv[1]);
//     exit(EXIT_FAILURE);
// }

// FILE *fp;
// double T;
// side_t Lx, Ly;
// sysz_t N;
// lttc_t *s;
// nnl_t *nn;
// double tmpdbl = 0.;
// //
// seed_rand = (uint32_t [LENSRND]) {SEED, SIID, LSEED, RSEED};
// sfmt_init_by_array(&sfmt, seed_rand, LENSRND);
// //
// Lx = atoi(argv[1]);
// Ly = atoi(argv[2]);
// T = atof(argv[3]);
// //
// N = (Lx * Ly);
// s = malloc(sizeof(*s) * N);
// nn = malloc(sizeof(*nn) * N);

// __init_hotstart_uniform(N, s);
// sprintf(buf, "res/N=%" PRIu32 "/", N);
// mkdir(buf, ACCESSPERMS);
// //
// __compute_nn_array(Lx, Ly, nn);
// for (sysz_t t = 0; t < TMCMC; t++)
// {
//     printf("\rt = %d", t);
//     tmpdbl += updMC(T, N, s, nn);
//     if (!(t % 100))
//     {
//         sprintf(buf, "res/N=%" PRIu32 "/cfg_Lx=%" PRIu32 "_Ly=%" PRIu32 "_T=%.2lf_t=%" PRIu32".bin", N, Lx, Ly, T, t);
//         fp = fopen(buf, "wb");
//         fwritecfg(&fp, N, s);
//         fclose(fp);
//     }
// }
// printf("\n");

// free(s);
// free(nn);