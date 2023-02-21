#include <stdio.h>
#include <imdefs.h>
#include <imdtlib.h>
#include <imfnlib.h>
#include <imllib.h>
#include <imrng.h>

int MODE;
md_t run[NMODES];
char buf[STR1024];
char buf1[STR1024];
char buf2[STR1024];
sfmt_t sfmt;
uint32_t *seed_rand;
FILE *f_log;

const char *MODES[] = {MODE_PRINTC,
                       MODE_CHKRNG,
                       MODE_ACFCOR,
                       MODE_GENUNC,
                       MODE_KGENCN};
vtmpf_t *FUNCS[] = {__print_conf,
                     __check_RNG,
                     __compute_acf,
                     __genUNcorr_CONFIG,
                     __gen_kconf};

int main(int argc, char *argv[])
{
    /*//////////////////////////////////////////// open log file, seed the RNG*/
    //
    __MAKElog(argc, argv);
    __setSFMT_seed_rand();
    for (int i = 0; i < NMODES; i++)
    {
        run[i].__mode__ = FUNCS[i];
        run[i].__name__ = (char *) MODES[i];
    }
    /*////////////////////////////////////////////////// execute program mode */
    //
    for (int a = TWO; a < argc; a++)
        if ((MODE = strIn_(argv[a], NMODES, MODES)))
            run[MODE - 1].__mode__(argv[1]);
    //
    /*////////////////////////////////////////////////////////////////////////*/
    fclose(f_log);
    return 0;
}