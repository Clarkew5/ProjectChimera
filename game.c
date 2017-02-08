#include "game.h"

struct Board ARIMAABOARD = {
    .gold = {0L, 0L, 0L, 0L, 0L, 0L},
    .silver = {0L, 0L, 0L, 0L, 0L, 0L},
    .empty = ~0L
};

int makeBoard(char *filepath){
    //struct Board *ARIMAABOARD = malloc(sizeof(struct Board));   
    if (filepath == NULL){    
        for (int i = 0; i < 6; i++){
            ARIMAABOARD.gold[i] = 0L;
            ARIMAABOARD.silver[i] = 0L;
        }
        ARIMAABOARD.empty = ~0L; 
    }
    else{
        //insert building a board form text file
    }
    return 0;
}

int printBoard(){
    printf(" +----------------+\n");
    for (int shift = 0; shift < 64; shift++){
        uint64_t i = 1L << shift;
        if (shift%8 == 0)
            printf("%d| ", (8 - (shift/8)));

        if ((i & ARIMAABOARD.gold[RABBIT]) == i)
            printf("R ");
        else if ((i & ARIMAABOARD.silver[RABBIT]) == i)
            printf("r ");
        else if ((i & ARIMAABOARD.gold[CAT]) == i)
            printf("C ");
        else if ((i & ARIMAABOARD.silver[CAT]) == i)
            printf("c ");
        else if ((i & ARIMAABOARD.gold[DOG]) == i)
            printf("D ");
        else if ((i & ARIMAABOARD.silver[DOG]) == i)
            printf("d ");
        else if ((i & ARIMAABOARD.gold[HORSE]) == i)
            printf("H ");
        else if ((i & ARIMAABOARD.silver[HORSE]) == i)
            printf("h ");
        else if ((i & ARIMAABOARD.gold[CAMMEL]) == i)
            printf("M ");
        else if ((i & ARIMAABOARD.silver[CAMMEL]) == i)
            printf("m ");
        else if ((i & ARIMAABOARD.gold[ELEPHANT]) == i)
            printf("E ");
        else if ((i & ARIMAABOARD.silver[ELEPHANT]) == i)
            printf("e ");
        else{
            if (shift == 18 || shift == 21 || shift == 42 || shift == 45)
                printf("X ");
            else
                printf("- ");
        }

        if (shift%8 == 7)
            printf("|\n");
    }
    printf(" +-----------------+\n   a b c d e f g h\n");
	return 0;
}

int updateBoard(char *move){
    char *colLetters = "abcdefgh";   
    char *rowNumbers = "87654321";
    if (strlen(move) == 63){
        printf("%s\n", move);
        do{
            int i = strchr(colLetters, *(move+1)) - colLetters;
            int j = strchr(rowNumbers, *(move+2)) - rowNumbers;
            switch (*move){
                case 'R':
                    ARIMAABOARD.gold[RABBIT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'r':
                    ARIMAABOARD.silver[RABBIT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'C':
                    ARIMAABOARD.gold[CAT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'c':
                    ARIMAABOARD.silver[CAT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'D':
                    ARIMAABOARD.gold[DOG] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'd':
                    ARIMAABOARD.silver[DOG] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'H':
                    ARIMAABOARD.gold[HORSE] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'h':
                    ARIMAABOARD.silver[HORSE] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'M':
                    ARIMAABOARD.gold[CAMMEL] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'm':
                    ARIMAABOARD.silver[CAMMEL] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'E':
                    ARIMAABOARD.gold[ELEPHANT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                case 'e':
                    ARIMAABOARD.silver[ELEPHANT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    break;
                default:
                    printf("error no such peice\n");
                    return 1;
            }
            move += 4;
        } while (*(move-1) != '\0');
                
    }
	else if ((strlen(move) < 21) && ((strlen(move) + 1)%5 == 0)){
        printf("%s\n", move);        
        do {
            int i1 = strchr(colLetters, *(move+1)) - colLetters;
            int j1 = strchr(rowNumbers, *(move+2)) - rowNumbers;
            int i2 = 0;
            int j2 = 0;
            switch (*(move+3)){
                case 'n':
                    i2 = i1;
                    j2 = j1-1;
                    break;
                case 'e':
                    i2 = i1+1;
                    j2 = j1;
                    break;
                case 's':
                    i2 = i1;
                    j2 = j1+1;
                    break;
                case 'w':
                    i2 = i1;
                    j2 = j2-1;
                    break;
                default:
                    printf("error no such peice\n");
                    return 1;
            }        
            switch (*move){
                case 'R':
                    ARIMAABOARD.gold[RABBIT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[RABBIT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);                    
                    break;
                case 'r':
                    ARIMAABOARD.silver[RABBIT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[RABBIT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'C':
                    ARIMAABOARD.gold[CAT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[CAT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);                    
                    break;
                case 'c':
                    ARIMAABOARD.silver[CAT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[CAT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'D':
                    ARIMAABOARD.gold[DOG] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[DOG] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'd':
                    ARIMAABOARD.silver[DOG] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[DOG] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'H':
                    ARIMAABOARD.gold[HORSE] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[HORSE] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'h':
                    ARIMAABOARD.silver[HORSE] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[HORSE] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'M':
                    ARIMAABOARD.gold[CAMMEL] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[CAMMEL] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'm':
                    ARIMAABOARD.silver[CAMMEL] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[CAMMEL] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'E':
                    ARIMAABOARD.gold[ELEPHANT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[ELEPHANT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'e':
                    ARIMAABOARD.silver[ELEPHANT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[ELEPHANT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'p':
                    return 0;
                default:
                    printf("error no such peice\n");
                    return 1;
            }
            move += 5;
        } while (*(move-1) != '\0');
    }
    else{
        printf("Incorrect Input\n");
        return 1;
    }
    return 0;
}
