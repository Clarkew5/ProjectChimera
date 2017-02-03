#ifndef GAME
#define GAME

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define RABBIT 0
#define CAT 1
#define DOG 2
#define HORSE 3
#define CAMMEL 4
#define ELEPHANT 5

struct Board{
    uint64_t gold[6];
    uint64_t silver[6];
    //uint64_t goldRabbits;
    //uint64_t goldAnimals;
    //uint64_t silverRabits;
    //uint64_t silverAnimals;
    uint64_t empty;
};

#ifdef BOARD
struct Board ARIMAABOARD;
#else
extern struct Board ARIMAABOARD;
#endif

int makeBoard(char *filepath);
int printBoard();
int updateBoard(char *move);

#endif
