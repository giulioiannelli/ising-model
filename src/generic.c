#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include <imdefs.h>
#include <imtdlib.h>
#include <imfnlib.h>
#include <imrng.h>


double avg(avg_t n, double *v)
{
    double s = 0.;
    for (avg_t i = 0; i < n; i++) 
        s += v[i];
    return (s / n);
}
int str_in(char **arr, int len, char *target)
{
  int i;
  for(i = 0; i < len; i++) {
    if(strncmp(arr[i], target, strlen(target)) == 0) {
      return i+1;
    }
  }
  return 0;
}

uint16_t strtou16(const char *s) {
    char c;
    int scanned;
    uint16_t i;
    scanned = sscanf(s, "%" SCNd16 "%c", &i, &c);
    if (scanned == 1)
        return i;
    else if (scanned > 1)
    {
        // TBD about extra data found
        return i;
    }
    if (c != '\0' || errno != 0)
    {
        fprintf(f_log, MSGFAIL PFCLU32 PIGOTIN "%c" MSGEXIT, c);
        fclose(f_log);
        exit(EXIT_FAILURE);
    }
    // TBD failed to scan;
    return 0;
}
uint32_t strtou32(const char *s) {
    char c;
    int scanned;
    uint32_t i;
    scanned = sscanf(s, "%" SCNd32 "%c", &i, &c);
    if (scanned == 1)
        return i;
    else if (scanned > 1)
    {
        // TBD about extra data found
        return i;
    }
    if (c != '\0' || errno != 0)
    {
        fprintf(f_log, MSGFAIL PFCLU32 PIGOTIN "%c" MSGEXIT, c);
        fclose(f_log);
        exit(EXIT_FAILURE);
    }
    return 0;
}

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