#include "game.h"

struct Board ARIMAABOARD = {
    //Initalizes all global variables to their start state
    .gold = {0L, 0L, 0L, 0L, },
    .silver = {0L, 0L, 0L, 0L, 0L, 0L},
    .empty = ~0L,

    .goldPositions = {64, 64, 64, 64, 64, 64, 64, 64,
                      64, 64, 64, 64, 64, 64, 64, 64},

    .silverPositions = {64, 64, 64, 64, 64, 64, 64, 64,
                        64, 64, 64, 64, 64, 64, 64, 64},

    .pieceIndexes = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5},

    .weights = {1, 2, 3, 5, 9, 13}, //weights borrowed from bomb.
    .gMaterial = 50,
    .sMaterial = 50,
    .gWave = 0,
    .sWave = 0
};

int makeBoard(char *filepath){
    //sets all the board global variables to 0
    if (filepath == NULL){    
        for (int i = 0; i < 6; i++){
            ARIMAABOARD.gold[i] = 0L;
            ARIMAABOARD.silver[i] = 0L;
        }
        ARIMAABOARD.empty = ~0L;

        for (int i= 0; i < 16; i++){
            ARIMAABOARD.goldPositions[i] = 64;
            ARIMAABOARD.silverPositions[i] = 64;
        }

        ARIMAABOARD.gMaterial = 50;
        ARIMAABOARD.sMaterial = 50;
        ARIMAABOARD.gWave = 0;
        ARIMAABOARD.sWave = 0;
    }
    else{
        //insert building a board form text file
    }
    return 0;
}

int printBoard(){
    printf(" +-----------------+\n");
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

int setupArray(bool isGold, int p, int shift){
    if (isGold){
        int i = 0;
        switch(p){            
            case RABBIT:
                while (ARIMAABOARD.goldPositions[i] != 64){
                    i++;
                    if (i >= 8){
                        printf("peice array setup error\n");
                        exit(1);
                    }
                }
            break;
            case CAT:
                if (ARIMAABOARD.goldPositions[CAT1_POS] == 64)
                    i = CAT1_POS;
                else if (ARIMAABOARD.goldPositions[CAT2_POS] == 64)
                    i = CAT2_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
            case DOG:
                if (ARIMAABOARD.goldPositions[DOG1_POS] == 64)
                    i = DOG1_POS;
                else if (ARIMAABOARD.goldPositions[DOG2_POS] == 64)
                    i = DOG2_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
            case HORSE:
                if (ARIMAABOARD.goldPositions[HORSE1_POS] == 64)
                    i = HORSE1_POS;
                else if (ARIMAABOARD.goldPositions[HORSE2_POS] == 64)
                    i = HORSE2_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
            case CAMMEL:
                if (ARIMAABOARD.goldPositions[CAMMEL_POS] == 64)
                    i =CAMMEL_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
            case ELEPHANT:
                if (ARIMAABOARD.goldPositions[ELEPHANT_POS] == 64)
                    i =ELEPHANT_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
        }
        ARIMAABOARD.goldPositions[i] = shift;
    }
    else{
        int i = 0;
        switch(p){
            
            case RABBIT:
                while (ARIMAABOARD.silverPositions[i] != 64){
                    i++;
                    if (i >= 8){
                        printf("peice array setup error\n");
                        exit(1);
                    }
                }
            break;
            case CAT:
                if (ARIMAABOARD.silverPositions[CAT1_POS] == 64)
                    i = CAT1_POS;
                else if (ARIMAABOARD.silverPositions[CAT2_POS] == 64)
                    i = CAT2_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
            case DOG:
                if (ARIMAABOARD.silverPositions[DOG1_POS] == 64)
                    i = DOG1_POS;
                else if (ARIMAABOARD.silverPositions[DOG2_POS] == 64)
                    i = DOG2_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
            case HORSE:
                if (ARIMAABOARD.silverPositions[HORSE1_POS] == 64)
                    i = HORSE1_POS;
                else if (ARIMAABOARD.silverPositions[HORSE2_POS] == 64)
                    i = HORSE2_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
            case CAMMEL:
                if (ARIMAABOARD.silverPositions[CAMMEL_POS] == 64)
                    i =CAMMEL_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;
            case ELEPHANT:
                if (ARIMAABOARD.silverPositions[ELEPHANT_POS] == 64)
                    i =ELEPHANT_POS;
                else{
                    printf("peice array setup error\n");
                    exit(1);
                }
            break;            
        }
        ARIMAABOARD.silverPositions[i] = shift;
    }
    return 0;
}

int updateArray(bool isGold, int p, int shift, char direction){
    if (isGold){
        int i = 0;
        switch(p){
            case RABBIT:
                while (ARIMAABOARD.goldPositions[i] != shift){
                    i++;
                    if (i >= 8){
                        printf("\npeice array update error:\n");
                        printf("R: %d %c\n", shift, direction);
                        printf("-------------------------------------------\n");
                        printPieceArray();
                        printBoard();
                        printf("-------------------------------------------\n");
                        exit(1);
                    }
                }
                break;
            case CAT:
                if (ARIMAABOARD.goldPositions[CAT1_POS] == shift)
                    i = CAT1_POS;
                else if (ARIMAABOARD.goldPositions[CAT2_POS] == shift)
                    i = CAT2_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("C: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
            case DOG:
                if (ARIMAABOARD.goldPositions[DOG1_POS] == shift)
                    i = DOG1_POS;
                else if (ARIMAABOARD.goldPositions[DOG2_POS] == shift)
                    i = DOG2_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("D: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
            case HORSE:
                if (ARIMAABOARD.goldPositions[HORSE1_POS] == shift)
                    i = HORSE1_POS;
                else if (ARIMAABOARD.goldPositions[HORSE2_POS] == shift)
                    i = HORSE2_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("H: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
            case CAMMEL:
                if (ARIMAABOARD.goldPositions[CAMMEL_POS] == shift)
                    i =CAMMEL_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("M: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
            case ELEPHANT:
                if (ARIMAABOARD.goldPositions[ELEPHANT_POS] == shift)
                    i =ELEPHANT_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("E: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
        }

        switch(direction){
            case 'n':
                ARIMAABOARD.goldPositions[i] -= 8;
                break;
            case 'e':
                ARIMAABOARD.goldPositions[i] += 1;
                break;
            case 's':
                ARIMAABOARD.goldPositions[i] += 8;
                break;
            case 'w':
                ARIMAABOARD.goldPositions[i] -= 1;
                break;
            case 'x':
                ARIMAABOARD.goldPositions[i] = 64;
                break;
            default:
                printf("error, invalid direction: %c\n", direction);
                break;
        }
    }
    else{
        int i = 0;
        switch(p){
            case RABBIT:
                while (ARIMAABOARD.silverPositions[i] != shift){
                    i++;
                    if (i >= 8){
                        printf("\npeice array update error:\n");
                        printf("r: %d %c\n", shift, direction);
                        printf("-------------------------------------------\n");
                        printPieceArray();
                        printBoard();
                        printf("-------------------------------------------\n");
                        exit(1);
                    }
                }
                break;
            case CAT:
                if (ARIMAABOARD.silverPositions[CAT1_POS] == shift)
                    i = CAT1_POS;
                else if (ARIMAABOARD.silverPositions[CAT2_POS] == shift)
                    i = CAT2_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("c: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
            case DOG:
                if (ARIMAABOARD.silverPositions[DOG1_POS] == shift)
                    i = DOG1_POS;
                else if (ARIMAABOARD.silverPositions[DOG2_POS] == shift)
                    i = DOG2_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("d: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
            case HORSE:
                if (ARIMAABOARD.silverPositions[HORSE1_POS] == shift)
                    i = HORSE1_POS;
                else if (ARIMAABOARD.silverPositions[HORSE2_POS] == shift)
                    i = HORSE2_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("h: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
            case CAMMEL:
                if (ARIMAABOARD.silverPositions[CAMMEL_POS] == shift)
                    i =CAMMEL_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("m: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
            case ELEPHANT:
                if (ARIMAABOARD.silverPositions[ELEPHANT_POS] == shift)
                    i =ELEPHANT_POS;
                else{
                    printf("\npeice array update error:\n");
                    printf("e: %d %c\n", shift, direction);
                    printf("-------------------------------------------\n");
                    printPieceArray();
                    printBoard();
                    printf("-------------------------------------------\n");
                    exit(1);
                }
                break;
        }
        switch(direction){
            case 'n':
                ARIMAABOARD.silverPositions[i] -= 8;
                break;
            case 'e':
                ARIMAABOARD.silverPositions[i] += 1;
                break;
            case 's':
                ARIMAABOARD.silverPositions[i] += 8;
                break;
            case 'w':
                ARIMAABOARD.silverPositions[i] -= 1;
                break;
            case 'x':
                ARIMAABOARD.silverPositions[i] = 64;
                break;
            default:
                printf("error, invalid direction: %c\n", direction);
                break;
        }
    }
    return 0;
}

uint16_t updateTraps(){
    /*updates the trap squares and returns a 16 bit number that represents
      which pieces have fallen into which traps*/ 
    int squares[4] = {18, 21, 42, 45};
    uint16_t updatedTraps = 0;
    uint64_t goldFriends = 0L;
    for (int p = 0; p < 6; p++){
        goldFriends |= ARIMAABOARD.gold[p];
    }
    uint64_t silverFriends = 0L;
    for (int p = 0; p < 6; p++){
        silverFriends |= ARIMAABOARD.silver[p];
    }
    for (int i = 0; i < 4; i++){
        if (((1L << squares[i]) & ARIMAABOARD.empty) != 0L) continue;
        for (int p = 0; p < 6; p++){
            if (((1L << squares[i]) & ARIMAABOARD.gold[p]) != 0L){
                if ((1L << (squares[i] + 8) & goldFriends) == 0L &&
                    (1L << (squares[i] - 8) & goldFriends) == 0L &&
                    (1L << (squares[i] + 1) & goldFriends) == 0L &&
                    (1L << (squares[i] - 1) & goldFriends) == 0L){
                        ARIMAABOARD.gold[p] &= ~(1L << squares[i]);
                        ARIMAABOARD.empty |= 1L << squares[i];
                        updateArray(true, p, squares[i], 'x');
                        switch (p){
                            case RABBIT:
                                updatedTraps |= (RTRAPPED << 4*i);
                                ARIMAABOARD.gMaterial -= ARIMAABOARD.weights[0];
                                break;
                            case CAT:
                                updatedTraps |= (CTRAPPED << 4*i);
                                ARIMAABOARD.gMaterial -= ARIMAABOARD.weights[1];
                                break;
                            case DOG:
                                updatedTraps |= (DTRAPPED << 4*i);
                                ARIMAABOARD.gMaterial -= ARIMAABOARD.weights[2];
                                break;
                            case HORSE:
                                updatedTraps |= (HTRAPPED << 4*i);
                                ARIMAABOARD.gMaterial -= ARIMAABOARD.weights[3];
                                break;
                            case CAMMEL:
                                updatedTraps |= (MTRAPPED << 4*i);
                                ARIMAABOARD.gMaterial -= ARIMAABOARD.weights[4];
                                break;
                            case ELEPHANT:
                                updatedTraps |= (ETRAPPED << 4*i);
                                ARIMAABOARD.gMaterial -= ARIMAABOARD.weights[5];
                                break;
                        }
                    
                }
            }
            if (((1L << squares[i]) & ARIMAABOARD.silver[p]) != 0L){
                if ((1L << (squares[i] + 8) & silverFriends) == 0L &&
                    (1L << (squares[i] - 8) & silverFriends) == 0L &&
                    (1L << (squares[i] + 1) & silverFriends) == 0L &&
                    (1L << (squares[i] - 1) & silverFriends) == 0L){
                        ARIMAABOARD.silver[p] &= ~(1L << squares[i]);
                        ARIMAABOARD.empty ^= 1L << squares[i];
                        updateArray(false, p, squares[i], 'x');
                        switch (p){
                            case RABBIT:
                                updatedTraps |= (rTRAPPED << 4*i);
                                ARIMAABOARD.sMaterial -= ARIMAABOARD.weights[0];
                                break;
                            case CAT:
                                updatedTraps |= (cTRAPPED << 4*i);
                                ARIMAABOARD.sMaterial -= ARIMAABOARD.weights[1];
                                break;
                            case DOG:
                                updatedTraps |= (dTRAPPED << 4*i);
                                ARIMAABOARD.sMaterial -= ARIMAABOARD.weights[2];
                                break;
                            case HORSE:
                                updatedTraps |= (hTRAPPED << 4*i);
                                ARIMAABOARD.sMaterial -= ARIMAABOARD.weights[3];
                                break;
                            case CAMMEL:
                                updatedTraps |= (mTRAPPED << 4*i);
                                ARIMAABOARD.sMaterial -= ARIMAABOARD.weights[4];
                                break;
                            case ELEPHANT:
                                updatedTraps |= (eTRAPPED << 4*i);
                                ARIMAABOARD.sMaterial -= ARIMAABOARD.weights[5];
                                break;
                        }
                }
            }
        }
    }
    return updatedTraps;
}

int updateBoard(char *move){
    /*updates the board after taking in a string that represents a move. 
      Used to make moves read from stdin*/
    char *colLetters = "abcdefgh";   
    char *rowNumbers = "87654321";
    //used for the initial board set up moves
    if (true){//strlen(move) == 63){
        printf("%s\n", move);
        do{
            int i = strchr(colLetters, *(move+1)) - colLetters;
            int j = strchr(rowNumbers, *(move+2)) - rowNumbers;
            switch (*move){
                case 'R':
                    ARIMAABOARD.gold[RABBIT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(true, RABBIT, (8*j) + i);
                    break;
                case 'r':
                    ARIMAABOARD.silver[RABBIT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(false, RABBIT, (8*j) + i);
                    break;
                case 'C':
                    ARIMAABOARD.gold[CAT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(true, CAT, (8*j) + i);
                    break;
                case 'c':
                    ARIMAABOARD.silver[CAT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(false, CAT, (8*j) + i);
                    break;
                case 'D':
                    ARIMAABOARD.gold[DOG] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(true, DOG, (8*j) + i);
                    break;
                case 'd':
                    ARIMAABOARD.silver[DOG] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(false, DOG, (8*j) + i);
                    break;
                case 'H':
                    ARIMAABOARD.gold[HORSE] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(true, HORSE, (8*j) + i);
                    break;
                case 'h':
                    ARIMAABOARD.silver[HORSE] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(false, HORSE, (8*j) + i);
                    break;
                case 'M':
                    ARIMAABOARD.gold[CAMMEL] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(true, CAMMEL, (8*j) + i);
                    break;
                case 'm':
                    ARIMAABOARD.silver[CAMMEL] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(false, CAMMEL, (8*j) + i);
                    break;
                case 'E':
                    ARIMAABOARD.gold[ELEPHANT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(true, ELEPHANT, (8*j) + i);
                    break;
                case 'e':
                    ARIMAABOARD.silver[ELEPHANT] ^= 1L << ((8*j) + i);
                    ARIMAABOARD.empty ^= 1L << ((8*j) + i);
                    setupArray(false, ELEPHANT, (8*j) + i);
                    break;
                default:
                    printf("error no such piece\n");
                    return 1;
            }
            move += 4;
        } while (*(move-1) != '\0');
                
    }
    //game moves after set up
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
                    printf("error no such piece\n");
                    return 1;
            }        
            switch (*move){
                case 'R':
                    ARIMAABOARD.gold[RABBIT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[RABBIT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(true, RABBIT, 8*j2 + i2, *(move+3));
                    break;
                case 'r':
                    ARIMAABOARD.silver[RABBIT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[RABBIT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(false, RABBIT, 8*j2 + i2, *(move+3));
                    break;
                case 'C':
                    ARIMAABOARD.gold[CAT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[CAT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(true, CAT, 8*j2 + i2, *(move+3));
                    break;
                case 'c':
                    ARIMAABOARD.silver[CAT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[CAT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(false, CAT, 8*j2 + i2, *(move+3));
                    break;
                case 'D':
                    ARIMAABOARD.gold[DOG] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[DOG] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(true, DOG, 8*j2 + i2, *(move+3));
                    break;
                case 'd':
                    ARIMAABOARD.silver[DOG] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[DOG] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(false, DOG, 8*j2 + i2, *(move+3));
                    break;
                case 'H':
                    ARIMAABOARD.gold[HORSE] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[HORSE] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(true, HORSE, 8*j2 + i2, *(move+3));
                    break;
                case 'h':
                    ARIMAABOARD.silver[HORSE] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[HORSE] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(false, HORSE, 8*j2 + i2, *(move+3));
                    break;
                case 'M':
                    ARIMAABOARD.gold[CAMMEL] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[CAMMEL] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(true, CAMMEL, 8*j2 + i2, *(move+3));
                    break;
                case 'm':
                    ARIMAABOARD.silver[CAMMEL] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[CAMMEL] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(false, CAMMEL, 8*j2 + i2, *(move+3));
                    break;
                case 'E':
                    ARIMAABOARD.gold[ELEPHANT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[ELEPHANT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(true, ELEPHANT, 8*j2 + i2, *(move+3));
                    break;
                case 'e':
                    ARIMAABOARD.silver[ELEPHANT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[ELEPHANT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateArray(false, ELEPHANT, 8*j2 + i2, *(move+3));
                    break;
                case 'p':
                    return 0;
                default:
                    printf("error no such piece\n");
                    return 1;
            }
            updateTraps();
            move += 5;
        } while (*(move-1) != '\0');
    }
    else{
        printf("Incorrect Input\n");
        return 1;
    }
    return 0;
}

int printPieceArray(){
    for (int i = 0; i < 8; i++){
        printf("R%d: %02d ", i+1, ARIMAABOARD.goldPositions[i]);
    }
    printf("\n");
    printf("C1: %02d ", ARIMAABOARD.goldPositions[CAT1_POS]);
    printf("C2: %02d ", ARIMAABOARD.goldPositions[CAT2_POS]);
    printf("D1: %02d ", ARIMAABOARD.goldPositions[DOG1_POS]);
    printf("D2: %02d ", ARIMAABOARD.goldPositions[DOG2_POS]);
    printf("H1: %02d ", ARIMAABOARD.goldPositions[HORSE1_POS]);
    printf("H2: %02d ", ARIMAABOARD.goldPositions[HORSE2_POS]);
    printf("M1: %02d ", ARIMAABOARD.goldPositions[CAMMEL_POS]);
    printf("E1: %02d\n", ARIMAABOARD.goldPositions[ELEPHANT_POS]);

    printf("\n");

    for (int i = 0; i < 8; i++){
        printf("r%d: %02d ", i+1, ARIMAABOARD.silverPositions[i]);
    }
    printf("\n");
    printf("c1: %02d ", ARIMAABOARD.silverPositions[CAT1_POS]);
    printf("c2: %02d ", ARIMAABOARD.silverPositions[CAT2_POS]);
    printf("d1: %02d ", ARIMAABOARD.silverPositions[DOG1_POS]);
    printf("d2: %02d ", ARIMAABOARD.silverPositions[DOG2_POS]);
    printf("h1: %02d ", ARIMAABOARD.silverPositions[HORSE1_POS]);
    printf("h2: %02d ", ARIMAABOARD.silverPositions[HORSE2_POS]);
    printf("m1: %02d ", ARIMAABOARD.silverPositions[CAMMEL_POS]);
    printf("e1: %02d\n", ARIMAABOARD.silverPositions[ELEPHANT_POS]);

    return 0;
}
