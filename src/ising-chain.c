#include <stdio.h>
#include <imdefs.h>
#include <imdtlib.h>
#include <imfnlib.h>
#include <imrng.h>
#include <imclib.h>

int MODE;
md_t run[NMODESC];
char buf[STR1024], buf1[STR1024], buf2[STR1024];
sfmt_t sfmt;
uint32_t *seed_rand;
FILE *f_log;

// const char *MODES[] = {""};
// void (*FUNCS[])() = {__print_conf, __check_RNG};

int main(int argc, char *argv[])
{
    /*///////////////////////////////////////////////////// open log, seed RNG*/
    //
    __MAKElog(argc, argv);
    __setSFMT_seed_rand();
    __gen_kconf(argv[1]);
    fclose(f_log);
    return 0;
}
 