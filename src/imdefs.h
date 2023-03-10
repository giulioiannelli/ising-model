#ifndef __IMDEFS_H_INC__
#define __IMDEFS_H_INC__
/**/
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define DUMP(x) (#x)
/**/
#define _HLIFI "{+}"
#define _LLPFI "[?]"
#define _LLPWI "[!]"
#define _LLPII "[+]"
#define _LLPEI "[*]"
#define _LLPPI "[•]"
#define _LOGON "| ON |"
#define _LOGOF "| OFF|"
/**/
#define __ " "
#define _C ","
#define _E "="
#define _H "-"
#define _N "\n"
#define _S "/"
#define _T "\t"
#define _U "_"
#define _V "|"
#define _W "!"
/**/
#define _NN _N _N     /*                                             "\\n\\n" */
#define _N_ _N __     /*                                               "\\n " */
#define _NT _N _T     /*                                             "\\n\\t" */
#define _TT _T _T     /*                                             "\\t\\t" */
#define __V __ _V     /*                                                 " |" */
#define _VT _V _T     /*                                               "|\\t" */
#define _WT _W _T     /*                                               "!\\t" */
#define _N_V _N_ _V   /*                                              "\\n |" */
#define _N_W _N_ _W   /*                                              "\\n |" */
#define _N_VT _N_ _VT /*                                           "\\n |\\t" */
#define _NT_V _NT __V /*                                           "\\n\\t |" */
#define _N_WT _N_ _WT /*                                      "\\n !\\t |\\t" */
/**/
#define _SARW "->"
#define _LARW "-->"
#define _TTSARW _TT _SARW
/**/
#define _N_V3LARW _N_V _N_V _N_V _LARW
/**/
#define _UMOD _U "MODE="
#define _UCFG _U "CONF="
/**/
#define MSGFAIL _N _LLPFI " PROGRAM FAILURE: "
#define MSGWARN _N _LLPWI " WARNING: "
#define MSGINFO _N _LLPII " INFO: "
#define MSGEXEC _N _LLPEI " EXECUTED: "
#define MSGEXIT _N_V3LARW " EXITING THE PROGRAM" _NN
/**/
#define LOGHEAD _HLIFI " LOG FILE OF ----------------------------|"
#define LOGFOOT _HLIFI " END OF LOG FILE ------------------------|" 
#define LOGPPDO _N _LLPPI " PREPROCESSOR DIRECTIVES: "
#define LOGWRKS _N _LLPPI " WORKING IN SETTING: "
#define LOGONOFF(x) (x ? LOGONN : LOGOFF)
/**/
#define EXTBIN ".bin"
#define EXTLOG ".log"
#define EXTTXT ".txt"
#define EXTDAT ".dat"
#define EXTEXE ".o"
#define EXTCPG ".c"
/**/
#define DIR "./"
#define DIRdep "dep/"
#define DIRdoc "doc/"
#define DIRres "res/"
#define DIRsrc "src/"
#define DIRtes "test/"
#define DIRtoo "tools/"
/**/
#define DIRanm DIRres "animated/"
#define DIRcfg DIRres "config/"
#define DIRlog DIRres "log/"
#define DIRvbc DIRres "vbc/"
#define DIRobs DIRres "obs/"
/**/


#define STR_bt "bt"
#define STR_N  "N"
#define STR_L1 "L1"
#define STR_L2 "L2"

#define FMT_bt "%.3g"
#define FMT_na "%" PRIu16
#define FMT_L  "%" PRIu16
#define FMT_N  "%" PRIu32

#define __L1IS STR_L1 _E                                             /* "L1=" */
#define __L2IS STR_L2 _E                                             /* "L2=" */
#define __NIS  STR_N  _E                                             /* "N="  */
#define __BTIS STR_bt _E                                             /* "bt=" */

#define __L1IS__ __L1IS FMT_L
#define __L2IS__ __L2IS FMT_L
#define __BTIS__ __BTIS FMT_bt
#define __NIS__  __NIS  FMT_N

#define __L1IS_L2IS__ __L1IS__ _U __L2IS__



#define PFNARGC _N_VT "wrong number of arguments (argc) passed."
#define PFCLU64 _N_VT "expected uint64 type as command line input."
#define PFCLU32 _N_VT "expected uint32 type as command line input."
#define PFCLDBL _N_VT "expected double type as command line input."
#define PFEXECV _N_VT "error executing (execv) " _SARW
#define PFFOPEN _N_VT "error opening file " _SARW
#define PFGCCMP _N_VT "error gcc-compiling " _SARW
#define PFMEMAL _N_VT "error allocating memory " _SARW
#define PFNKNUM _N_VT "not enough KNUM patterns for coupling matrices."
#define PFUNREN _N_VT "unable to rename the file."

#define PWFRZNS _NSWTVT "configurations reached frozen state."
#define PWFLPTH _NSWTVT "flip probability less than flip threshold."
#define PWTAUTH _NSWTVT "tauint is less than 1 or couldnt be established."
#if ISING1D
#define PWEXDTH PWTAUTH
#elif MARKOVP
#define PWEXDTH PWFLPTH
#endif

#define PIFEXST _N_VT "file already exists " _SARW
#define PIFNXST _N_VT "file did not exist " _SARW
#define PIFRSUC _N_VT "file renamed successfully."
#define PIGOTIN _N_VT "got instead: "
#define PIMKDIR _N_VT "created directory (w ERRNO) " _SARW
#define PIREADF _N_VT "reading from file " _SARW
#define PIRESER _N_VT "resetting errno to 0."
#define PISKPCV _N_VT "skipping current set of values."
#define PICJMOK _N_VT "coupling matrix done for values" _SARW
#define PICJMNK _N_VT "coupling matrix exists or 1/b < 1/b_c (SKIP)" _SARW


#define J 1.                /* coupling strenght*/
#define TWO 2               /* number two */
#define LENSRND 4           /* seed array for sfmt length */
#define MAX_tMCMC 10        /**/

#define PRIconfSIGNED(x) printf("%+" PRIi8 " ", x);
#define PRIconfUNSGND(x) printf("%"  PRIi8 " ", (x + 1) / 2);

#define FPRIconfSIGNED(x,f) fprintf(f, "%+" PRIi8 " ", x);
#define FPRIconfUNSGND(x,f) fprintf(f, "%"  PRIi8 " ", (x + 1) / 2);

#define PRIconf PRIconfUNSGND
#define FPRIconf FPRIconfUNSGND

extern FILE *f_log;
extern char buf[1024];



#endif /* __IMDEFS_H_INC__ */