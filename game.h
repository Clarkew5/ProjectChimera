#ifndef GAME
#define GAME

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
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
    uint64_t empty;
};

extern struct Board ARIMAABOARD;

int makeBoard(char *filepath);
int printBoard();
int updateBoard(char *move);
int updateTraps(bool isGold, int animal, int square);

#endif
