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
    run[1].__mode__ = __compute_ACF;
    run[1].__name__ = MODES[1];
    /*///////////////////////////////////////////////////////// open log file */
    __MAKElog(argc, PROGN, argv);
    /*////////////////////////////////////// seed the random number generator */
    __setSFMT_seed_rand();
    /* execute program according to mode */
    for (int a = 2; a < argc; a++)
        if ((MODE = str_in(MODES, ARRAY_SIZE(MODES), argv[a])))
            run[MODE - 1].__mode__(argv[1]);
    fclose(f_log);
}
