#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    __read_dtc(config_fn, &d);
    printf("%d\n", d.Navg);
    __set_localdtc(&d1, &d);
    printf("%d\n", d1.Navg);
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
extern void __print_conf(char *config_fn)
{
    
    FILE *fconf;
    dtc_t dtc;
    smdtc_t smdtc;
    // imcd_t imcd;
    /**/
    __F_OPEN(&fconf, config_fn, "r+");
    __get_row_fgets(&fconf, row);
    /**/
    if (!(strcmp(row, HEADER_DT_DTC_I2D)))
    {
        __fscanf_dtc(&fconf, &dtc);
        __printf_dtc(dtc);
    }
    else if (!(strcmp(row, HEADER_DT_SMDTC_I2D)))
    {
        __fscanf_smdtc(&fconf, &smdtc);
        __printf_smdtc(smdtc);
    }
    // else if (strIn_(row, DT_SMDTC_I2D))
    // {
    //     __fscanf_imcd(&imcd, config_fn);
    //     __printf_dtc(imcd, config_fn);
    // }
    fclose(fconf);
}
extern void __gen_kconf(char *config_fn)
{
    smdtc_t d;
    __read_smdtc(config_fn, &d);
    __Tauint(d);
    __wbrite_nconf_d(d);
}