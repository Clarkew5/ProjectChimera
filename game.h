#ifndef GAME
#define GAME

#include <stdlib.h>
#include <stdio.h>

struct Board{
    unsigned long long gRabbit;
    unsigned long long gCat;
    unsigned long long gDog;
    unsigned long long gHorse;
    unsigned long long gCammel;
    unsigned long long gElephant;
    unsigned long long sRabbit;
    unsigned long long sCat;
    unsigned long long sDog;
    unsigned long long sHorse;
    unsigned long long sCammel;
    unsigned long long sElephant;
    unsigned long long empty;
};

struct Board makeBoard(char *filepath);
int printBoard(Board *b);
int makeMove(char *move, Board *b);

#endif
