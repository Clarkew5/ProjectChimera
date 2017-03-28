#include "game.h"

struct Board ARIMAABOARD = {
    //Initalizes all global variables to their start state
    .gold = {0L, 0L, 0L, 0L, 0L, 0L},
    .silver = {0L, 0L, 0L, 0L, 0L, 0L},
    .empty = ~0L,

    .goldRabbitArray = 0L,
    .goldOtherAnimalArray = 0L,
    .silverRabbitArray = 0L,
    .silverOtherAnimalArray = 0L,

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

        ARIMAABOARD.goldRabbitArray = 0L;
        ARIMAABOARD.goldOtherAnimalArray = 0L;
        ARIMAABOARD.silverRabbitArray = 0L;
        ARIMAABOARD.silverOtherAnimalArray = 0L;

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

uint16_t updateTraps(){
    /*updates the trap squares and returns a 16 bit number that represents
      which pieces have fallen into which traps*/ 
    int squares[4] = {18, 21, 42, 45};
    uint16_t updatedTraps = 0;
    uint64_t friends = 0L;
    for (int i = 0; i < 4; i++){
        if (((1L << squares[i]) & ARIMAABOARD.empty) != 0L) continue;
        for (int j = 0; j < 6; j++){
            if (((1L << squares[i]) & ARIMAABOARD.gold[j]) != 0L){
                for (int p = 0; p < 6; p++){
                    friends |= ARIMAABOARD.gold[p];
                }
                if ((1L << (squares[i] + 8) & friends) == 0L &&
                    (1L << (squares[i] - 8) & friends) == 0L &&
                    (1L << (squares[i] + 1) & friends) == 0L &&
                    (1L << (squares[i] - 1) & friends) == 0L){
                        ARIMAABOARD.gold[j] &= ~(1L << squares[i]);
                        ARIMAABOARD.empty |= 1L << squares[i];
                        switch (j){
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
            if (((1L << squares[i]) & ARIMAABOARD.silver[j]) != 0L){
                for (int p = 0; p < 6; p++){
                    friends |= ARIMAABOARD.silver[p];
                }

                if ((1L << (squares[i] + 8) & friends) == 0L &&
                    (1L << (squares[i] - 8) & friends) == 0L &&
                    (1L << (squares[i] + 1) & friends) == 0L &&
                    (1L << (squares[i] - 1) & friends) == 0L){
                        ARIMAABOARD.silver[j] &= ~(1L << squares[i]);
                        ARIMAABOARD.empty ^= 1L << squares[i];
                        switch (j){
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
                    updateTraps(true, RABBIT, 8*j2 + i2);
                    break;
                case 'r':
                    ARIMAABOARD.silver[RABBIT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[RABBIT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(false, RABBIT, 8*j2 + i2);
                    break;
                case 'C':
                    ARIMAABOARD.gold[CAT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[CAT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(true, CAT, 8*j2 + i2);                    
                    break;
                case 'c':
                    ARIMAABOARD.silver[CAT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[CAT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(false, CAT, 8*j2 + i2);
                    break;
                case 'D':
                    ARIMAABOARD.gold[DOG] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[DOG] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(true, DOG, 8*j2 + i2);
                    break;
                case 'd':
                    ARIMAABOARD.silver[DOG] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[DOG] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(false, DOG, 8*j2 + i2);
                    break;
                case 'H':
                    ARIMAABOARD.gold[HORSE] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[HORSE] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(true, HORSE, 8*j2 + i2);
                    break;
                case 'h':
                    ARIMAABOARD.silver[HORSE] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[HORSE] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(false, HORSE, 8*j2 + i2);
                    break;
                case 'M':
                    ARIMAABOARD.gold[CAMMEL] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[CAMMEL] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(true, CAMMEL, 8*j2 + i2);
                    break;
                case 'm':
                    ARIMAABOARD.silver[CAMMEL] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[CAMMEL] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(false, CAMMEL, 8*j2 + i2);
                    break;
                case 'E':
                    ARIMAABOARD.gold[ELEPHANT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.gold[ELEPHANT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(true, ELEPHANT, 8*j2 + i2);
                    break;
                case 'e':
                    ARIMAABOARD.silver[ELEPHANT] ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
                    ARIMAABOARD.silver[ELEPHANT] ^= 1L << ((8*j2) + i2);
                    ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
                    updateTraps(false, ELEPHANT, 8*j2 + i2);
                    break;
                case 'p':
                    return 0;
                default:
                    printf("error no such piece\n");
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

int setupArray(bool isGold, int p, int shift){
    if (isGold){
        if (p == RABBIT){
            int i = 0;
            while (((arrayMask << (6*i)) & ARIMAABOARD.goldRabbitArray) != 0){
                i++;
                if (i >= 8){
                    printf("piece array error\n");
                    exit(1);
                }
            }
            ARIMAABOARD.goldRabbitArray |= shift << (6*i);
        }
        else if (p == CAMMEL || p == ELEPHANT){
            if (((arrayMask << (12*p)) & ARIMAABOARD.goldOtherAnimalArray) != 0)
                ARIMAABOARD.goldRabbitArray |= shift << (12*p);
            else{
                    printf("piece array error\n");
                    exit(1);
            }
        }
        else{
            if ((((arrayMask << (12*p)) & ARIMAABOARD.goldOtherAnimalArray) != 0))
                ARIMAABOARD.goldOtherAnimalArray |= shift << (12*p);
            else if ((((arrayMask << (12*p + 1)) & ARIMAABOARD.goldOtherAnimalArray) != 0))
                ARIMAABOARD.goldOtherAnimalArray |= shift << (12*p + 1);
            else{
                    printf("piece array error\n");
                    exit(1);
            }
        }
    }
    else{
        if (p == RABBIT){
            int i = 0;
            while (((arrayMask << (6*i)) & ARIMAABOARD.silverRabbitArray) != 0){
                i++;
                if (i >= 8){
                    printf("piece array error\n");
                    exit(1);
                }
            }
            ARIMAABOARD.silverRabbitArray |= shift << (6*i);
        }
        else if (p == CAMMEL || p == ELEPHANT){
            if (((arrayMask << (12*p)) & ARIMAABOARD.silverOtherAnimalArray) != 0)
                ARIMAABOARD.silverRabbitArray |= shift << (12*p);
            else{
                    printf("piece array error\n");
                    exit(1);
            }
        }
        else{
            if ((((arrayMask << (12*p)) & ARIMAABOARD.silverOtherAnimalArray) != 0))
                ARIMAABOARD.silverOtherAnimalArray |= shift << (12*p);
            else if ((((arrayMask << (12*p + 1)) & ARIMAABOARD.silverOtherAnimalArray) != 0))
                ARIMAABOARD.silverOtherAnimalArray |= shift << (12*p + 1);
            else{
                    printf("piece array error\n");
                    exit(1);
            }
        }
    }
    return 0;
}

int updateArray(bool isGold, int p, int shift, char direction){
    if (isGold){
        if (p == RABBIT){
            for (int i = 0; i < 8; i++){
                if (((arrayMask << (6*i)) & ARIMAABOARD.goldRabbitArray) == shift << (6*i))
                    switch (direction){
                        case 'n':
                            ARIMAABOARD.goldRabbitArray += (8 << 6*i);
                            break;
                        case 'e':
                            ARIMAABOARD.goldRabbitArray -= (1 << 6*i);
                            break;
                        case 's':
                            ARIMAABOARD.goldRabbitArray -= (8 << 6*i);
                            break;
                        case 'w':
                            ARIMAABOARD.goldRabbitArray += (1 << 6*i);
                            break;
                    }
            }
        }
        else if (p == CAMMEL || p == ELEPHANT){
            if (((arrayMask << (p-1)) & ARIMAABOARD.goldOtherAnimalArray) == shift << (p-1))
                switch (direction){
                    case 'n':
                        ARIMAABOARD.goldRabbitArray += (8 << 6*p);
                        break;
                    case 'e':
                        ARIMAABOARD.goldRabbitArray -= (1 << 6*p);
                        break;
                    case 's':
                        ARIMAABOARD.goldRabbitArray -= (8 << 6*p);
                        break;
                    case 'w':
                        ARIMAABOARD.goldRabbitArray += (1 << 6*p);
                        break;
                }
        }
        else{
            if (((arrayMask << (12 * (p-1))) & ARIMAABOARD.goldOtherAnimalArray) == (shift << (12 * (p-1))))
                switch (direction){
                    case 'n':
                        ARIMAABOARD.goldOtherAnimalArray += (8 << 6*(p-1));
                        break;
                    case 'e':
                        ARIMAABOARD.goldOtherAnimalArray -= (1 << 6*(p-1));
                        break;
                    case 's':
                        ARIMAABOARD.goldOtherAnimalArray -= (8 << 6*(p-1));
                        break;
                    case 'w':
                        ARIMAABOARD.goldOtherAnimalArray += (1 << 6*(p-1));
                        break;
                }
            else
                switch (direction){
                    case 'n':
                        ARIMAABOARD.goldOtherAnimalArray += (8 << 6*p);
                        break;
                    case 'e':
                        ARIMAABOARD.goldOtherAnimalArray -= (1 << 6*p);
                        break;
                    case 's':
                        ARIMAABOARD.goldOtherAnimalArray -= (8 << 6*p);
                        break;
                    case 'w':
                        ARIMAABOARD.goldOtherAnimalArray += (1 << 6*p);
                        break;
                }
        }
    }
    else{
        if (p == RABBIT){
            for (int i = 0; i < 8; i++){
                if (((arrayMask << 6*i) & ARIMAABOARD.silverRabbitArray) == shift << 6*i)
                    switch (direction){
                        case 'n':
                            ARIMAABOARD.silverRabbitArray += (8 << 6*i);
                            break;
                        case 'e':
                            ARIMAABOARD.silverRabbitArray -= (1 << 6*i);
                            break;
                        case 's':
                            ARIMAABOARD.silverRabbitArray -= (8 << 6*i);
                            break;
                        case 'w':
                            ARIMAABOARD.goldRabbitArray += (1 << 6*i);
                            break;
                    }
            }
        }
        else if (p == CAMMEL || p == ELEPHANT){
            if (((arrayMask << (p-1)) & ARIMAABOARD.silverOtherAnimalArray) == shift << (p-1))
                switch (direction){
                    case 'n':
                        ARIMAABOARD.goldRabbitArray += (8 << 6*(p-1));
                        break;
                    case 'e':
                        ARIMAABOARD.goldRabbitArray -= (1 << 6*(p-1));
                        break;
                    case 's':
                        ARIMAABOARD.goldRabbitArray -= (8 << 6*(p-1));
                        break;
                    case 'w':
                        ARIMAABOARD.goldRabbitArray += (1 << 6*(p-1));
                        break;
                }
        }
        else{
            if (((arrayMask << (12 * (p-1))) & ARIMAABOARD.silverOtherAnimalArray) == (shift << (12 * (p-1))))
                switch (direction){
                    case 'n':
                        ARIMAABOARD.silverOtherAnimalArray += (8 << 6*(p-1));
                        break;
                    case 'e':
                        ARIMAABOARD.silverOtherAnimalArray -= (1 << 6*(p-1));
                        break;
                    case 's':
                        ARIMAABOARD.silverOtherAnimalArray -= (8 << 6*(p-1));
                        break;
                    case 'w':
                        ARIMAABOARD.silverOtherAnimalArray += (1 << 6*(p-1));
                        break;
                }
            else
                switch (direction){
                    case 'n':
                        ARIMAABOARD.silverOtherAnimalArray += (8 << 6*p);
                        break;
                    case 'e':
                        ARIMAABOARD.silverOtherAnimalArray -= (1 << 6*p);
                        break;
                    case 's':
                        ARIMAABOARD.silverOtherAnimalArray -= (8 << 6*p);
                        break;
                    case 'w':
                        ARIMAABOARD.silverOtherAnimalArray += (1 << 6*p);
                        break;
                }
        }
    }
    return 0;
}
