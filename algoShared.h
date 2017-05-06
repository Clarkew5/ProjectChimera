#ifndef ALGO
#define ALGO

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#include "game.h"
#include "hash.h"
#include "moves.h"

uint64_t frozen(int i, bool isGold);

uint64_t *generateMoveBitboards(bool isGold);
int randomMovePushPull(uint64_t *bitboards, bool isGold, int movesLeft);
bool inGoal(bool isGold);
bool gameOver(bool isGold);
struct Hash *branchHash(bool isGold);

#endif
