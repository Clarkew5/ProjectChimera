#include "game.h"

struct Board *makeBoard(char *filepath){
    struct Board *b = malloc(sizeof(struct Board));    
    if (filepath == NULL){    
        for (int i = 0; i < 6; i++){
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
    printf(" +----------------+\n");
    for (int shift = 0; shift < 64; shift++){
        uint64_t i = 1L <<shift;
        if (shift%8 == 0)
            printf("%d| ", (8 - (shift/8)));

        if ((i & b->gold[RABBIT]) == i)
            printf("R ");
        else if ((i & b->silver[RABBIT]) == i)
            printf("r ");
        else if ((i & b->gold[CAT]) == i)
            printf("C ");
        else if ((i & b->silver[CAT]) == i)
            printf("c ");
        else if ((i & b->gold[DOG]) == i)
            printf("D ");
        else if ((i & b->silver[DOG]) == i)
            printf("d ");
        else if ((i & b->gold[HORSE]) == i)
            printf("H ");
        else if ((i & b->silver[HORSE]) == i)
            printf("h ");
        else if ((i & b->gold[CAMMEL]) == i)
            printf("M ");
        else if ((i & b->silver[CAMMEL]) == i)
            printf("m ");
        else if ((i & b->gold[ELEPHANT]) == i)
            printf("E ");
        else if ((i & b->silver[ELEPHANT]) == i)
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

int updateBoard(char *move, struct Board *b){
    char *colLetters = "abcdefgh";   
    char *rowNumbers = "87654321";
    if (strlen(move) == 63){
        printf("%s\n", move);
        do{
            int i = strchr(colLetters, *(move+1)) - colLetters;
            int j = strchr(rowNumbers, *(move+2)) - rowNumbers;        
            switch (*move){
                case 'R':
                    b->gold[RABBIT] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'r':
                    b->silver[RABBIT] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'C':
                    b->gold[CAT] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'c':
                    b->silver[CAT] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'D':
                    b->gold[DOG] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'd':
                    b->silver[DOG] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'H':
                    b->gold[HORSE] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'h':
                    b->silver[HORSE] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'M':
                    b->gold[CAMMEL] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'm':
                    b->silver[CAMMEL] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'E':
                    b->gold[ELEPHANT] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
                    break;
                case 'e':
                    b->silver[ELEPHANT] ^= 1L << ((8*j) + i);
                    b->empty ^= 1L << ((8*j) + i);
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
                    b->gold[RABBIT] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->gold[RABBIT] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);                    
                    break;
                case 'r':
                    b->silver[RABBIT] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->silver[RABBIT] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'C':
                    b->gold[CAT] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->gold[CAT] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);                    
                    break;
                case 'c':
                    b->silver[CAT] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->silver[CAT] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'D':
                    b->gold[DOG] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->gold[DOG] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'd':
                    b->silver[DOG] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->silver[DOG] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'H':
                    b->gold[HORSE] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->gold[HORSE] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'h':
                    b->silver[HORSE] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->silver[HORSE] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'M':
                    b->gold[CAMMEL] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->gold[CAMMEL] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'm':
                    b->silver[CAMMEL] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->silver[CAMMEL] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'E':
                    b->gold[ELEPHANT] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->gold[ELEPHANT] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
                    break;
                case 'e':
                    b->silver[ELEPHANT] ^= 1L << ((8*j1) + i1);
                    b->empty ^= 1L << ((8*j1) + i1);
                    b->silver[ELEPHANT] ^= 1L << ((8*j2) + i2);
                    b->empty ^= 1L << ((8*j2) + i2);
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
