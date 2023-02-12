#include <stdio.h>
#include <imdefs.h>
#include <imtdlib.h>
#include <imfnlib.h>
#include <imrng.h>

int MODE;
md_t run[NMODESC];
char buf[1024];
sfmt_t sfmt;
uint32_t *seed_rand;
FILE *f_log;

const char *MODES[] = {""};
void (*FUNCS[])() = {__print_configf, __check_RNG, __compute_ACF, __genUNcorr_CONFIG};

int main(int argc, char *argv[])
{
    /*///////////////////////////////////////////////////////// open log file */
    __MAKElog(argc, argv);
    /*////////////////////////////////////////////////////////// seed the RNG */
    __setSFMT_seed_rand();


    FILE *f_out;
    uint64_t effKNUM, missingK;
    /* get variable from command line */
    N = strtoul_chck(argv[1], 10);
    T = strtodb_chck(argv[2]);
    /* check pflip */
    sprintf(buf, DIRCHCK P_TYPES _U __NIS__ _U __TIS__ EXTTXT, N, T);
    if ((f_out = fopen(buf, "w+")) == NULL)
    {
        fprintf(f_log, MSGFAIL PFFOPEN "%s" MSGEXIT, buf);
        exit(EXIT_FAILURE);
    }
    fprintf(f_out, "%lf", PFL1P);
    if (PFL1P < PTHRESH)
    {
        fprintf(f_log, MSGWARN PWFLPTH "%g" MSGEXIT, PTHRESH);
        exit(EXIT_SUCCESS);
        fclose(f_out);
        fclose(f_log);
    }
    fclose(f_out);
    /* find the file, if exists, */
    find_CFGS();
    if (!(strncmp(buf, STR_FIND, strlen(STR_FIND))))
    {
        missingK = KNUM;
        effKNUM = KNUM;
    }
    else if (!(strncmp(buf, DIRCFGS, strlen(DIRCFGS))))
    {
        sprintf(buf2, DIRCFGS P_TYPE _U __NIS__ _U __TIS__ _U "[", N, T);
        strncpy(buf3, buf + strlen(buf2), strlen(buf) - strlen(buf2) - 5);
        effKNUM = strtoull_chck(buf3, 10);
        if (effKNUM >= KNUM)
        {
            return 0;
        }
        missingK = KNUM - effKNUM;
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    /* open the file */
    sprintf(buf, DIRCFGS P_TYPE _U __NIS__ _U __TIS__ _U __KNUMIS__ EXTBIN,
            N, T, effKNUM);
    if ((f_out = fopen(buf, "a+b")) == NULL)
    {
        printf(MSGFAIL PFFOPEN "%s" MSGEXIT, buf);
        exit(EXIT_FAILURE);
    }
    fseek(f_out, 0, SEEK_END);
    /* generate missingK configuration */
    for (uint32_t k = 0; k < missingK; k++)
    {
        s = calloc(N, sizeof(*s));
        s[0] = SGN01(rnd_dbl() - .5);
        for (uint32_t i = 1; i < N; i++)
        {
            if (rnd_dbl() < PFLIP)
                s[i] = s[i - 1];
            else
                s[i] = -s[i - 1];
        }
        fwrite(s, sizeof(*s), N, f_out);
        free(s);
    }
    if (effKNUM < KNUM)
    {
        sprintf(buf2, DIRCFGS P_TYPE _U __NIS__ _U __TIS__ _U __KNUMIS__ EXTBIN,
                N, T, KNUM);
        if ((rename(buf, buf2)) == 0)
        {
            fprintf(f_log, PIFRSUC);
        }
        else
        {
            fprintf(f_log, MSGFAIL PFUNREN);
        }
    }
    fclose(f_log);
    fclose(f_out);
}
 