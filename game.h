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

#define CAT1_POS 8
#define CAT2_POS 9
#define DOG1_POS 10
#define DOG2_POS 11
#define HORSE1_POS 12
#define HORSE2_POS 13
#define CAMMEL_POS 14
#define ELEPHANT_POS 15 

struct Board{
    uint64_t gold[6];
    uint64_t silver[6];
    uint64_t empty;

    uint8_t goldPositions[16];
    uint8_t silverPositions[16];

    int pieceIndexes[16];

    int weights[6];
    int gMaterial;
    int sMaterial;
    int gWave;
    int sWave;
};

extern struct Board ARIMAABOARD;

int makeBoard(char *filepath);
int printBoard();
int printPieceArray();
int updateBoard(char *move);
int setupArray(bool isGold, int p, int shift);
int updateArray(bool isGold, int p, int shift, char direction);
uint16_t updateTraps();

#endif
