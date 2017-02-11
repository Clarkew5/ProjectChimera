#ifndef ALGO
#define ALGO

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"

/*--move masks----------------------------------------------------------------*/
#define pieceMask       0b0000111100000000
#define SILVERR         0b0000000000000000
#define SILVERC         0b0000000100000000
#define SILVERD         0b0000001000000000
#define SILVERH         0b0000001100000000
#define SILVERM         0b0000010000000000
#define SILVERE         0b0000010100000000
#define GOLDR           0b0000011000000000
#define GOLDC           0b0000011100000000
#define GOLDD           0b0000100000000000
#define GOLDH           0b0000100100000000
#define GOLDM           0b0000101000000000
#define GOLDE           0b0000101100000000
#define PASS            0b0000110000000000

#define collumMask      0b0000000011100000
#define COLLUMa         0b0000000000000000
#define COLLUMb         0b0000000000100000
#define COLLUMc         0b0000000001000000
#define COLLUMd         0b0000000001100000
#define COLLUMe         0b0000000010000000
#define COLLUMf         0b0000000010100000
#define COLLUMg         0b0000000011000000
#define COLLUMh         0b0000000011100000

#define rowMask         0b0000000000011100
#define ROW1            0b0000000000000000
#define ROW2            0b0000000000000100
#define ROW3            0b0000000000001000
#define ROW4            0b0000000000001100
#define ROW5            0b0000000000010000
#define ROW6            0b0000000000010100
#define ROW7            0b0000000000011000
#define ROW8            0b0000000000011100

#define directionMask   0b0000000000000011
#define DIRECTIONN      0b0000000000000000
#define DIRECTIONE      0b0000000000000001
#define DIRECTIONS      0b0000000000000010
#define DIRECTIONW      0b0000000000000011

/*----------------------------------------------------------------------------*/

int randomAgent(bool isGold);
int numberOfMoves(uint64_t *bitboards, bool isGold);
uint64_t *generateMoveBitboards(bool isGold);
int printBitboard(uint64_t bitboard);

#endif
