# # Minimal makefile for main.c

# prova: main.c fnlattice.c rngs.c
# 	gcc -I./ -Isrc/ -Idep/SFMT/ -O3 -DSFMT_MEXP=19937 -o prova src/main.c src/fnlattice.c src/rngs.c dep/SFMT/SFMT.c -lm

prova: main.o fnlattice.o rngs.o
	gcc -I./ -Isrc/ -Idep/SFMT/ -O3 -DSFMT_MEXP=19937 -o prova main.o fnlattice.o rngs.o SFMT.o -lm

main.o: src/main.c src/fnlattice.c src/rngs.c dep/SFMT/SFMT.c
	gcc -I./ -Isrc/  -Idep/SFMT/ -O3 -DSFMT_MEXP=19937 -c src/main.c src/fnlattice.c src/rngs.c dep/SFMT/SFMT.c -lm
rngs.o:  src/rngs.c dep/SFMT/SFMT.c
	gcc -I./ -Isrc/ -Idep/SFMT/ -O3 -DSFMT_MEXP=19937 -c src/rngs.c dep/SFMT/SFMT.c
fnlattice.o:  src/fnlattice.c src/rngs.c dep/SFMT/SFMT.c
	gcc -I./ -Isrc/ -Idep/SFMT/ -O3 -DSFMT_MEXP=19937 -c src/fnlattice.c src/rngs.c dep/SFMT/SFMT.c -lm

# PROGRAM = test
# FILES.c = src/main.c src/rngs.c src/fnlattice.c dep/SFMT/SFMT.c
# FILES.h =  imrng.h imtdlib.h imfnlib.h imdefs.h
# FILES.o = ${FILES.c:.c=.o}

# CC      = gcc
# SFLAGS  = -std=c11
# GFLAGS  = -g
# OFLAGS  = -O3
# WFLAG1  = -Wall
# WFLAG2  = -Wextra
# WFLAG3  = -Werror
# WFLAG4  = -Wstrict-prototypes
# WFLAG5  = -Wmissing-prototypes
# WFLAGS  = ${WFLAG1} ${WFLAG2} ${WFLAG3} ${WFLAG4} ${WFLAG5}
# IFLAG1  = -Idep/SFMT
# IFLAGS  = ${IFLAG1}
# SFMTFL  = -DSFMT_MEXP=19937
# # UFLAGS  = # Set on command line only

# CFLAGS  = ${SFLAGS} ${GFLAGS} ${OFLAGS} ${WFLAGS} ${IFLAGS} ${SFMTFL}
# # LDFLAGS =
# # LDLIBS  =

# all:	${PROGRAM}

# ${PROGRAM}: ${FILES.o}
# 	${CC} -o $@ ${CFLAGS} ${FILES.o} 
# main.o: src/main.c src/rngs.c src/fnlattice.c dep/SFMT/SFMT.c
# 	${CC} -o $@ ${CFLAGS} $^ 
# rngs.o: ${FILES.h}
# fnlattice.o: ${FILES.h}

# # If it exists, prog1.dSYM is a directory on macOS
# DEBRIS = a.out core *~ *.dSYM
# RM_FR  = rm -fr

# clean:
# 	${RM_FR} ${FILES.o} ${PROGRAM} ${DEBRIS}
