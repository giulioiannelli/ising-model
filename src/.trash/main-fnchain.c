#include <stdio.h>
#include <stdlib.h>
#include <imdefs.h>
#include <imdtlib.h>
#include <imfnlib.h>
#include <imclib.h>

extern void __gen_kconf(char *config_fn)
{
    imcd_t d;
    __fscanf_Nb_configfile(&d, config_fn);
    __Tauint(d);
    __wbrite_nconf_d(d);
}