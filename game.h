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

#define trapMask 0b0000000000001111
#define rTRAPPED 0b0000000000000001
#define cTRAPPED 0b0000000000000010
#define dTRAPPED 0b0000000000000011
#define hTRAPPED 0b0000000000000100
#define mTRAPPED 0b0000000000000101
#define eTRAPPED 0b0000000000000110
#define RTRAPPED 0b0000000000000111
#define CTRAPPED 0b0000000000001000
#define DTRAPPED 0b0000000000001001
#define HTRAPPED 0b0000000000001010
#define MTRAPPED 0b0000000000001011
#define ETRAPPED 0b0000000000001100

#define arrayMask 63L

struct Board{
    uint64_t gold[6];
    uint64_t silver[6];
    uint64_t empty;

    uint64_t goldRabbitArray;
    uint64_t goldOtherAnimalArray;
    uint64_t silverRabbitArray;
    uint64_t silverOtherAnimalArray;

    int weights[6];
    int gMaterial;
    int sMaterial;
    int gWave;
    int sWave;
};

extern struct Board ARIMAABOARD;

int makeBoard(char *filepath);
int printBoard();
int updateBoard(char *move);
uint16_t updateTraps();

#endif
