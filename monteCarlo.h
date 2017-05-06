#ifndef MONTECARLO
#define MONTECARLO

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>

#include "game.h"
#include "moves.h"
#include "algoShared.h"

int monteCarloTS(bool isGold, double tTime);

#endif
