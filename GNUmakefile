# # Minimal makefile for main.c

PATH_SRC  = src/
PATH_SFMT = dep/SFMT/
srcFILES.c = ising-lattice.c rngs.c fnlattice.c generic.c
SFMTFILES.c = SFMT.c
FILES.c = ${srcFILES.c} ${SFMTFILES.c}
FILES.o = ${FILES.c:.c=.o}
PATHS.c := $(addprefix $(PATH_SRC), $(srcFILES.c)) $(addprefix $(PATH_SFMT), $(SFMTFILES.c))

DIRS := res/ res/vbc res/obs res/log res/unconf
$(shell mkdir -p $(DIRS))

PROGRAM ?= ising-lattice-c
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

${PROGRAM}: ${PATHS.c}
	${CC} ${ALLFLAGS} -o $@ $^ ${LMFLAG}

DEBRIS = a.out *~ 
RM_FR  = rm -fr



clean:
	${RM_FR} ${FILES.o} ${DEBRIS}




