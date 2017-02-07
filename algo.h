#ifndef ALGO
#define ALGO

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include "game.h"

uint16_t randomMove();
int numberOfMoves(uint64_t *bitboards, bool isGold);
uint64_t *generateMoveBitboards(bool isGold);
int printBitboard(uint64_t bitboard);

#endif
