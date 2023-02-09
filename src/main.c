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

int MODE;
md_t run[NMODES];
char buf[1024];
sfmt_t sfmt;
uint32_t *seed_rand;
FILE *f_log;

const char *MODES[] = {"--print_c", "--check_sfmt", "--acf", "--gen_unconf"};
void (*FUNCS[])() = {__print_configf, __check_RNG, __compute_ACF, __genUNcorr_CONFIG};

int main(int argc, char *argv[])
{
    //git working
    for (int i = 0; i < NMODES; i++)
    {
        run[i].__mode__ = FUNCS[i];
        run[i].__name__ = (char *) MODES[i];
    }
    /*///////////////////////////////////////////////////////// open log file */
    __MAKElog(argc, argv);
    /*////////////////////////////////////////////////////////// seed the RNG */
    __setSFMT_seed_rand();
    /*////////////////////////////////////////////////// execute program mode */
    for (int a = TWO; a < argc; a++)
        if ((MODE = strIn_(argv[a], MODES)))
            run[MODE - 1].__mode__(argv[1]);
    fclose(f_log);
}