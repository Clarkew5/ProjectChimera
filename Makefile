CC=gcc
CFLAGS=-Wall -std=c99 -g

.PHONY: all clean

all: arimaa

game.o: game.c game.h

algoShared.o: algoShared.c algoShared.h

hash.o: hash.c hash.h

moves.o: moves.h moves.c

randomAgent.o: randomAgent.c randomAgent.h

negaMax.o: negaMax.c negaMax.h

monteCarlo.o: monteCarlo.c monteCarlo.h

arimaa: arimaa.c game.o algoShared.o hash.o moves.o randomAgent.o negaMax.o monteCarlo.o 

clean:
	rm *.o arimaa

