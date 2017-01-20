#ifndef GAME
#define GAME

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define RABBIT 0
#define CAT 1
#define DOG 2
#define HORSE 3
#define CAMMEL 4
#define ELEPHANT 5


struct Board{
    unsigned long long int gold[6];
    unsigned long long int silver[6];
    unsigned long long int empty;
};

struct Board *makeBoard(char *filepath);
int printBoard(struct Board *b);
int makeMove(char *move, struct Board *b);

#endif
