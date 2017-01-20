#include "game.h"

struct Board *makeBoard(char *filepath){
    struct Board *b = malloc(sizeof(struct Board));    
    if (filepath == NULL){    
        for(int i = 0; i < 6; i++){
            b->gold[i] = 0;
            b->silver[i] = 0;
        }
        b->empty = ~0; 
    }
    else{
        //insert building a board form text file
    }
	return b;
}

int printBoard(struct Board *b){
    printf("+--------+\n");
    for (int shift = 0; shift < 64; shift++){
        uint64_t i = 1L <<shift;
        if (shift%8 == 0)
            printf("|");
        if ((i & b->gold[RABBIT]) == i)
            printf("R");
        else if ((i & b->silver[RABBIT]) == i)
            printf("r");
        else if ((i & b->gold[CAT]) == i)
            printf("C");
        else if ((i & b->silver[CAT]) == i)
            printf("c");
        else if ((i & b->gold[DOG]) == i)
            printf("D");
        else if ((i & b->silver[DOG]) == i)
            printf("d");
        else if ((i & b->gold[HORSE]) == i)
            printf("H");
        else if ((i & b->silver[HORSE]) == i)
            printf("h");
        else if ((i & b->gold[CAMMEL]) == i)
            printf("M");
        else if ((i & b->silver[CAMMEL]) == i)
            printf("m");
        else if ((i & b->gold[ELEPHANT]) == i)
            printf("E");
        else if ((i & b->silver[ELEPHANT]) == i)
            printf("e");
        else{
            if(shift == 18 || shift == 21 || shift == 42 || shift == 45)
                printf("X");
            else
                printf("-");
        }

        if (shift%8 == 7)
            printf("|\n");
    }
    printf("+--------+\n");
	return 0;
}

int makeMove(char *move, struct Board *b){
	return 0;
}
