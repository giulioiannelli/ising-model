#include <stdio.h>
#include <stdlib.h>
#include <imdefs.h>
#include <imdtlib.h>
#include <imfnlib.h>
#include <imllib.h>

extern void __compute_acf(char *config_fn)
{
    smdtc_t d1;
    sysz_t tMC;
    dtc_t d;
    /*///////////////////////////////// open and read config_file + variables */
    __fscanf_configfile(&d, config_fn);
    __set_localdtc(&d1, &d);
    tMC = d1.tMC;
    /*////////////////////////////////////////////////////////////////////////*/
    while (d1.N <= d.N_M)
    {
        d1.tMC = tMC * d1.N;
        for (d1.b = d.b_m; d1.b < d.b_M; d1.b += d.b_s)
            __ifNETauint_makeit(d1);
        d1.L1 += d.Ls;
        d1.L2 += d.Ls;
        d1.N = (d1.L1 * d1.L2);
    }
}
extern void __gen_K_conf(char *config_fn)
{
    smdtc_t d;
    __fscanf_Nb_configfile(&d, config_fn);
    __Tauint(d);
    __wbrite_nconf_d(d);
}