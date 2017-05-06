#ifndef HASH
#define HASH
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "algoShared.h"
#include "game.h"

struct Hash{
    size_t size;
    int numOfElements;
    struct Entry **entries;
};

struct Entry{//storing the whole board eats a lot of memory and should be changed    
    uint64_t gold[6];   //key
    uint64_t silver[6]; //key
    uint16_t move[4];   //value
    struct Entry *next;
};

struct Hash *createHash(size_t size);
int destroyHash(struct Hash *hash);
int insertEntry(uint16_t *move, struct Hash *hash);
//char *lookUp(char *key, struct Hash *hash);
int printTable(struct Hash *hash);

#endif
