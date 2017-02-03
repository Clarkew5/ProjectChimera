CC=gcc
CFLAGS=-Wall -std=c99 -g

.PHONY: all clean

all: arimaa

game.o: game.c game.h

algo.o: algo.c algo.h

test: test.c game.o algo.o

arimaa: arimaa.c game.o algo.o

clean:
	rm *.o arimaa

