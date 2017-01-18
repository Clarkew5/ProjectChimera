CC=gcc
CFLAGS=-Wall -std=c99 -g

.PHONY: all clean

all: arimaa

test: test.c game.o

arimaa: arimaa.c game.o

game.o: game.c game.h

clean:
	rm *.o arimaa

