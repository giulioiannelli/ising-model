# # Minimal makefile for main.c

PATH_SRC  = src/
PATH_SFMT = dep/SFMT/
srcFILESL.c = ising-lattice.c rngs.c fnlattice.c generic.c imcommon.c main-fnlattice.c
srcFILESC.c = ising-chain.c rngs.c fnchain.c generic.c imcommon.c main-fnchain.c
SFMTFILES.c = SFMT.c
FILESL.c = ${srcFILESL.c} ${SFMTFILES.c}
FILESL.o = ${FILESL.c:.c=.o}
FILESC.c = ${srcFILESC.c} ${SFMTFILES.c}
FILESC.o = ${FILESC.c:.c=.o}
PATHSL.c := $(addprefix $(PATH_SRC), $(srcFILESL.c)) $(addprefix $(PATH_SFMT), $(SFMTFILES.c))
PATHSC.c := $(addprefix $(PATH_SRC), $(srcFILESC.c)) $(addprefix $(PATH_SFMT), $(SFMTFILES.c))

DIRS := res/ res/vbc res/obs res/obs_c res/log res/unconf
$(shell mkdir -p $(DIRS))

PROGRAML ?= ising-lattice
PROGRAMC ?= ising-chain
CC        = gcc
GFLAGS    = -g
OFLAGS    = -O3
DSFMTFLAG = -DSFMT_MEXP=19937
LMFLAG    = -lm
WFLAG1    = -Wall
WFLAG2    = -Wextra
WFLAGS    = ${WFLAG1} ${WFLAG2}
INC_PATH1 = -I.
INC_PATH2 = -Isrc
INC_PATH3 = -Idep/SFMT
INC_PATHS = ${INC_PATH1} ${INC_PATH2} ${INC_PATH3}
ALLFLAGS  = ${GFLAGS} ${OFLAGS} ${DSFMTFLAG} ${WFLAGS} ${INC_PATHS}

all: ${PROGRAML} ${PROGRAMC}
${PROGRAML}: ${PATHSL.c}
	${CC} ${ALLFLAGS} -o $@ $^ ${LMFLAG}
${PROGRAMC}: ${PATHSC.c}
	${CC} ${ALLFLAGS} -o $@ $^ ${LMFLAG}

DEBRIS = a.out *~ 
RM_FR  = rm -fr

clean:
	${RM_FR} ${FILESL.o} ${FILESC.o} ${DEBRIS}




