#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <inttypes.h>
#include <imdefs.h>
#include <imdtlib.h>
#include <imfnlib.h>
#include <imrng.h>
/* MATH FUNCTIONS *********************************************************** */
//
/** perform the sum of a floating point array 
 * @param n (size_t) the number of vector components
 * @param v (double *) the floaring point array
 * @return (double) sum(v)
 */
extern double sum_vs(size_t n, double *v)
{
    double s = 0.;
    for (size_t i = 0; i < n; i++)
        s += v[i];
    return s;
}
/** perform the sum of a floating point array 
 * @param n (size_t) the number of vector components
 * @param v (double *) the floaring point array
 * @return (double) sum(v)
 */
extern double sum_vs_hlttc_t(size_t n, hlttc_t *v)
{
    double s = 0.;
    for (size_t i = 0; i < n; i++)
        s += v[i];
    return s;
}
/** perform the sum of squared components of an array
 * @param n (size_t) the number of vector components
 * @param v (double *) the array
 * @return (double) sum(v)
 */
extern double sum_vs_2(size_t n, double *v)
{
    double s = 0.;
    for (size_t i = 0; i < n; i++)
        s += v[i] * v[i];
    return s;
}
/** perform the average of a floating point array 
 * @param n (size_t) the number of replicas onto which pervorm average
 * @param v (double *) the floaring point array
 * @return (double) avg(v)
 */
extern double avg(size_t n, double *v)
{
    return (sum_vs(n, v) / n);
}
/** perform the average of a floating point array 
 * @param n (size_t) the number of replicas onto which pervorm average
 * @param v (hlttc_t *) the floaring point array
 * @return (double) avg(v)
 */
extern double avg_hlttc_t(size_t n, hlttc_t *v)
{
    return (sum_vs_hlttc_t(n, v) / n);
}
/** perform the average of the square elements of array 
 * @param n (size_t) the number of replicas onto which pervorm average
 * @param v (double *) the floaring point array
 * @return (double) avg(v)
 */
extern double avg_2(size_t n, double *v)
{
    return (sum_vs_2(n, v) / n);
}
/** get the ReLU(x) (or softplus) of input unsigned 32-bit integer.
 * @param x (int32_t) a 32-bit integer 
 * @return (uint32_t) max(0, x).
 */
extern uint32_t softplus_u32(int32_t x)
{
    if(x > 0)
    {
        return x;
    }
    return 0;
}
/* STRING RELATED FUNCTIONS ************************************************* */

/** check if a string is in an array of strings. If so return the index of the
 * array corresponding to the srting element.
 * @param s (char *) the string to be checked
 * @param len (size_t) number of strings in array
 * @param ss (char **) the array of strings
 * @return (size_t) 0 if the string is not in the array, index + 1 if there is.
 */
extern size_t strIn_(char *s, size_t len, const char **ss)
{
    for (size_t id = 0; id < len; id++)
        if (!(strncmp(ss[id], s, strlen(s))))
            return id + 1;
    return 0;
}
/**
 * does not do anything
 * @return (void) none
 */
extern void __splash__(void)
{
    ;
}
uint16_t strtou16(const char *s)
{
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
        // fclose(f_log);
        // exit(EXIT_FAILURE);
    }
    // TBD failed to scan;
    return 0;
}
uint32_t strtou32(const char *s)
{
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
        // fclose(f_log);
        // exit(EXIT_FAILURE);
    }
    return 0;
}
/** get a row from an open file
 * @param row (char *) the char pointer to store the row content
 * @param fc (FILE **) file from which to read
 * @return (void) none
 */
extern void __get_row_fgets(FILE **fc, char *row)
{
    if ((fgets(row, STR1024, *fc) == NULL) && (ferror(*fc)))
    {
        fprintf(f_log, MSGFAIL PFFGETS "%s" MSGEXIT, DUMP(*fc));
        perror(MSG_ERR_FGETS);
        fclose(f_log);
        exit(EXIT_FAILURE);
    }
}
extern void __2get_row_fgets(FILE **fc, char *row)
{
    __get_row_fgets(fc, row);
    __get_row_fgets(fc, row);
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
/** open a file according to an operative mode allowed by fopen and go to the
 * final byte of the file
 * @param f the file pointer
 * @param fn the file name
 * @param md the mode
 * @return (void) none
 */
extern void __F_OPEN_SEEKEND(FILE **f, const char *fn, const char *md)
{
    __F_OPEN(f, fn, md);
    fseek(*f, 0, SEEK_END);
}
/** open a pipe according to an operative mode allowed by fopen
 * @param p the pipe pointer
 * @param fn the file name
 * @param md the mode
 * @return (void) none
 */
extern void __P_OPEN(FILE **p, const char *fn, const char *md)
{
    if ((*p = popen(fn, md)) == NULL)
    {
        fprintf(f_log, MSGFAIL PFFOPEN "%s" MSGEXIT, fn);
        perror("Pipe Error: ");
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
    seed_rand = (uint32_t[LENSRND]){SEED, SIID, LSEED, RSEED};
    sfmt_init_by_array(&sfmt, seed_rand, LENSRND);
}
/**
 * open log file and initialize it with program name and argument from CLI.
 * @param argc the number of arguments passed through CLI
 * @param progn the name of the program
 * @return (voide) none
 */
extern void __MAKElog(int argc, char *argv[])
{
    sprintf(buf, DIRlog "%s" _UCFG "%s" EXTLOG, argv[0] + strlen(DIR), argv[1] + strlen(DIRcfg) + 1);
    if ((f_log = fopen(buf, "w+")) == NULL)
    {
        printf(MSGFAIL PFFOPEN "%s" MSGEXIT, buf);
        exit(EXIT_FAILURE);
    }
    fprintf(f_log, LOGHEAD "%s" MSGINFO, argv[0] + strlen(DIR));
    for (int i = 0; i < argc; i++)
        fprintf(f_log, (i < argc - 1 ? "%s " : "%s"), argv[i]);
}