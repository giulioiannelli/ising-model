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

#define PROGN "ising-model-c"

char buf[1024];
sfmt_t sfmt;
uint32_t *seed_rand;
FILE *f_log;

const char *MODES[] = {"--print_c", "--acf", "--check_sfmt"};

int main(int argc, char *argv[])
{
    int MODE;
    md_t run[3];
    run[0].__mode__ = __print_configf;
    run[0].__name__ = (char *) MODES[0];
    run[1].__mode__ = __compute_ACF;
    run[1].__name__ = (char *) MODES[1];
    run[2].__mode__ = __check_RNG;
    run[2].__name__ = (char *) MODES[2];
    /*///////////////////////////////////////////////////////// open log file */
    __MAKElog(argc, argv);
    /*////////////////////////////////////// seed the random number generator */
    __setSFMT_seed_rand();
    /* execute program according to mode */
    for (int a = 2; a < argc; a++)
        if ((MODE = strIn_(argv[a], MODES)))
            run[MODE - 1].__mode__(argv[1]);
    fclose(f_log);
}
