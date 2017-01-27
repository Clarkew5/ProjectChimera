CC=gcc
CFLAGS=-Wall -std=c99 -g

.PHONY: all clean

all: make

test: test.o game.o

test.o: test.c

arimaa: arimaa.o game.o algo.o

arimaa.o: arima.c

game.o: game.c game.h

algo.o: algo.c algo.h

clean:
	rm *.o arimaa

