#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <imdefs.h>
#include <imtdlib.h>
#include <imfnlib.h>
#include <imrng.h>


void __challoc(void *__ptr)
{
    if (__ptr == NULL)
    {
        fprintf(f_log, MSGFAIL PFMEMAL "%s" MSGEXIT, DUMP(__ptr));
        exit(EXIT_FAILURE);
    }
}

/**
 * check that input file pointer points to an existing file
 * @param n the file name
 * @return (bool) true if file exists, false otherwise
 */
extern bool F_EEXIST(const char *n)
{
    FILE *f;
    if ((f = fopen(n, "r")))
        fclose(f);
    else
        return false;
    return true;
}
/**
 * check that input file pointer points to a non existing file
 * @param n the file name
 * @return (bool) false if file exists, true otherwise
 */
extern bool F_NEXIST(const char *n)
{
    return (!(F_EEXIST(n)));
}
/** open a file according to an operative mode allowed by fopen
 * @param f the file pointer
 * @param fn the file name
 * @param md the mode
 * @return (void) none
 */
extern void __F_OPEN(FILE **f, const char *fn, const char *md)
{
    if ((*f = fopen(fn, md)) == NULL)
    {
        fprintf(f_log, MSGFAIL PFFOPEN "%s" MSGEXIT, buf);
        exit(EXIT_FAILURE);
    }
}
/**
 * check that number of parsed argument equal the required one
 * @param argc number of parsed argument
 * @param NARG number of required arguments
 * @return (void) none
 */
extern void __CHECKargc(int argc, int NARG)
{
    if (argc - 1 != NARG)
    {
        fprintf(f_log, MSGFAIL PFNARGC MSGEXIT);
        fclose(f_log);
        exit(EXIT_FAILURE);
    }
}
/**
 * set the seed of sfmt rng by array.
 * @return (void) none
 */
extern void __setSFMT_seed_rand(void)
{
    seed_rand = (uint32_t [LENSRND]) {SEED, SIID, LSEED, RSEED};
    sfmt_init_by_array(&sfmt, seed_rand, LENSRND);
}
/**
 * open log file and initialize it with program name and argument from CLI.
 * @param argc the number of arguments passed through CLI
 * @param progn the name of the program
 * @return (voide) none
 */
extern void __MAKElog(int argc, const char *progn, char *argv[])
{
    if ((f_log = fopen(buf, "w+")) == NULL)
    {
        printf(MSGFAIL PFFOPEN "%s" MSGEXIT, buf);
        exit(EXIT_FAILURE);
    }
    fprintf(f_log, LOGHEAD "%s" MSGINFO, progn);
    for (int i = 0; i < argc; i++)
        fprintf(f_log, (i < argc - 1 ? "%s " : "%s"), argv[i]);
}