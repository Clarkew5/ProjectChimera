#include "moves.h"

uint16_t conMove(bool isGold, int piece, int i, int j, char direction){
    /*Constructs a 16 bit number that represents a move. 
      See notes.txt for details*/
    uint16_t bitMove = 0L;    
    if (isGold){
        switch (piece){
            case RABBIT:
                bitMove |= GOLDR;
                break;
            case CAT:
                bitMove |= GOLDC;
                break;
            case DOG:
                bitMove |= GOLDD;
                break;
            case HORSE:
                bitMove |= GOLDH;
                break;
            case CAMMEL:
                bitMove |= GOLDM;
                break;
            case ELEPHANT:
                bitMove |= GOLDE;
                break;
            case 6:
                return PASS;
        }
    }
    else{
        switch (piece){
            case RABBIT:
                bitMove |= SILVERR;
                break;
            case CAT:
                bitMove |= SILVERC;
                break;
            case DOG:
                bitMove |= SILVERD;
                break;
            case HORSE:
                bitMove |= SILVERH;
                break;
            case CAMMEL:
                bitMove |= SILVERM;
                break;
            case ELEPHANT:
                bitMove |= SILVERE;
                break;
            case 6:
                return PASS;
        }
    }
    switch (i){
        case 0:
            bitMove |= COLLUMa;
            break;
        case 1:
            bitMove |= COLLUMb;
            break;
        case 2:
            bitMove |= COLLUMc;
            break;
        case 3:
            bitMove |= COLLUMd;
            break;
        case 4:
            bitMove |= COLLUMe;
            break;
        case 5:
            bitMove |= COLLUMf;
            break;
        case 6:
            bitMove |= COLLUMg;
            break;
        case 7:
            bitMove |= COLLUMh;
            break;
    }
    switch (j){
        case 0:
            bitMove |= ROW1;
            break;
        case 1:
            bitMove |= ROW2;
            break;
        case 2:
            bitMove |= ROW3;
            break;
        case 3:
            bitMove |= ROW4;
            break;
        case 4:
            bitMove |= ROW5;
            break;
        case 5:
            bitMove |= ROW6;
            break;
        case 6:
            bitMove |= ROW7;
            break;
        case 7:
            bitMove |= ROW8;
            break;
    }
    switch (direction){
        case 'n':
            bitMove |= DIRECTIONN;
            break;
        case 'e':
            bitMove |= DIRECTIONE;
            break;
        case 's':
            bitMove |= DIRECTIONS;
            break;
        case 'w':
            bitMove |= DIRECTIONW;
            break;
    }
    return bitMove;
}


int updateBoardBit(uint16_t move){
    //updates the board given a 16bit number
    bool isGold;
    int p;
    int i1;
    int j1;
    int i2;
    int j2;
    switch(move & pieceMask){
        case(SILVERR):
            isGold = false;
            p = 0;
            break; 
        case(SILVERC):
            isGold = false;
            p = 1;
            break;
        case(SILVERD):
            isGold = false;
            p = 2;
            break;
        case(SILVERH):
            isGold = false;
            p = 3;
            break;
        case(SILVERM):
            isGold = false;
            p = 4;
            break;
        case(SILVERE):
            isGold = false;
            p = 5;
            break;
        case(GOLDR):
            isGold = true;
            p = 0;
            break;
        case(GOLDC):
            isGold = true;
            p = 1;
            break;
        case(GOLDD):
            isGold = true;
            p = 2;
            break;
        case(GOLDH):
            isGold = true;
            p = 3;
            break;
        case(GOLDM):
            isGold = true;
            p = 4;
            break;
        case(GOLDE):
            isGold = true;
            p = 5;
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & collumMask){
        case(COLLUMa):
            i1 = 0;
            break;
        case(COLLUMb):
            i1 = 1;
            break;
        case(COLLUMc):
            i1 = 2;
            break;
        case(COLLUMd):
            i1 = 3;
            break;
        case(COLLUMe):
            i1 = 4;
            break;
        case(COLLUMf):
            i1 = 5;
            break;
        case(COLLUMg):
            i1 = 6;
            break;
        case(COLLUMh):
            i1 = 7;
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & rowMask){
        case(ROW1):
            j1 = 0;
            break;
        case(ROW2):
            j1 = 1;
            break;
        case(ROW3):
            j1 = 2;
            break;
        case(ROW4):
            j1 = 3;
            break;
        case(ROW5):
            j1 = 4;
            break;
        case(ROW6):
            j1 = 5;
            break;
        case(ROW7):
            j1 = 6;
            break;
        case(ROW8):
            j1 = 7;
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & directionMask){
        case(DIRECTIONN):
            i2 = i1;
            j2 = j1 - 1;
            updateArray(isGold, p, 8*j1+i1, 'n');
            if (isGold)
                ARIMAABOARD.gWave++;
            else
                ARIMAABOARD.sWave--;
            break;
        case(DIRECTIONE):
            i2 = i1 + 1;
            j2 = j1;
            updateArray(isGold, p, 8*j1+i1, 'e');
            break;
        case(DIRECTIONS):
            i2 = i1;
            j2 = j1 + 1;
            updateArray(isGold, p, 8*j1+i1, 's');
            if (isGold)
                ARIMAABOARD.gWave--;
            else
                ARIMAABOARD.sWave++;
            break;
        case(DIRECTIONW):
            i2 = i1 - 1;
            j2 = j1;
            updateArray(isGold, p, 8*j1+i1, 'w');
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    if (isGold){
        ARIMAABOARD.gold[p] ^= 1L << ((8*j1) + i1);
        ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
        ARIMAABOARD.gold[p] ^= 1L << ((8*j2) + i2);
        ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
    }
    else{
        ARIMAABOARD.silver[p] ^= 1L << ((8*j1) + i1);
        ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
        ARIMAABOARD.silver[p] ^= 1L << ((8*j2) + i2);
        ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
    }
    return 0;
}

int printMove(uint16_t move){
    //turns a 16bit move into proper notation
    switch(move & pieceMask){
        case(SILVERR):
            printf("r");
            break; 
        case(SILVERC):
            printf("c");
            break;
        case(SILVERD):
            printf("d");
            break;
        case(SILVERH):
            printf("h");
            break;
        case(SILVERM):
            printf("m");
            break;
        case(SILVERE):
            printf("e");
            break;
        case(GOLDR):
            printf("R");
            break;
        case(GOLDC):
            printf("C");
            break;
        case(GOLDD):
            printf("D");
            break;
        case(GOLDH):
            printf("H");
            break;
        case(GOLDM):
            printf("M");
            break;
        case(GOLDE):
            printf("E");
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & collumMask){
        case(COLLUMa):
            printf("a");
            break;
        case(COLLUMb):
            printf("b");
            break;
        case(COLLUMc):
            printf("c");
            break;
        case(COLLUMd):
            printf("d");
            break;
        case(COLLUMe):
            printf("e");
            break;
        case(COLLUMf):
            printf("f");
            break;
        case(COLLUMg):
            printf("g");
            break;
        case(COLLUMh):
            printf("h");
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & rowMask){
        case(ROW1):
            printf("8");
            break;
        case(ROW2):
            printf("7");
            break;
        case(ROW3):
            printf("6");
            break;
        case(ROW4):
            printf("5");
            break;
        case(ROW5):
            printf("4");
            break;
        case(ROW6):
            printf("3");
            break;
        case(ROW7):
            printf("2");
            break;
        case(ROW8):
            printf("1");
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & directionMask){
        case(DIRECTIONN):
            printf("N ");
            break;
        case(DIRECTIONE):
            printf("E ");
            break;
        case(DIRECTIONS):
            printf("S ");
            break;
        case(DIRECTIONW):
            printf("W ");
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    return 0;
}


int undoMove(uint16_t lastMove){
    uint16_t move = lastMove;
    move &= ~(directionMask);
    //the switch statement checks if the move is gold to update gWave or sWave
    bool isGold;
    switch(lastMove & pieceMask){
        case(SILVERR):
            isGold = false;
            break; 
        case(SILVERC):
            isGold = false;
            break;
        case(SILVERD):
            isGold = false;
            break;
        case(SILVERH):
            isGold = false;
            break;
        case(SILVERM):
            isGold = false;
            break;
        case(SILVERE):
            isGold = false;
            break;
        case(GOLDR):
            isGold = true;
            break;
        case(GOLDC):
            isGold = true;
            break;
        case(GOLDD):
            isGold = true;
            break;
        case(GOLDH):
            isGold = true;
            break;
        case(GOLDM):
            isGold = true;
            break;
        case(GOLDE):
            isGold = true;
            break;
        default:
            printf("move corrupt\n");
            break;
    }

    switch (lastMove & directionMask){
        case(DIRECTIONN):
            move |= DIRECTIONS;
            move &= ~(rowMask);
            switch (lastMove & rowMask){
                case(ROW8):
                    move |= ROW7;
                    break;
                case(ROW7):
                    move |= ROW6;
                    break;
                case(ROW6):
                    move |= ROW5;
                    break;
                case(ROW5):
                    move |= ROW4;
                    break;
                case(ROW4):
                    move |= ROW3;
                    break;
                case(ROW3):
                    move |= ROW2;
                    break;
                case(ROW2):
                    move |= ROW1;
                    break;
            }
            if(isGold)
                ARIMAABOARD.gWave--;
            else
                ARIMAABOARD.sWave++;
            break;
        case(DIRECTIONE):
            move |= DIRECTIONW;
            move &= ~(collumMask);
            switch (lastMove & collumMask){
                case(COLLUMa):
                    move |= COLLUMb;
                    break;
                case(COLLUMb):
                    move |= COLLUMc;
                    break;
                case(COLLUMc):
                    move |= COLLUMd;
                    break;
                case(COLLUMd):
                    move |= COLLUMe;
                    break;
                case(COLLUMe):
                    move |= COLLUMf;
                    break;
                case(COLLUMf):
                    move |= COLLUMg;
                    break;
                case(COLLUMg):
                    move |= COLLUMh;
                    break;
            }
            break;
        case(DIRECTIONS):
            move |= DIRECTIONN;
            move &= ~(rowMask);
            switch (lastMove & rowMask){
                case(ROW1):
                    move |= ROW2;
                    break;
                case(ROW2):
                    move |= ROW3;
                    break;
                case(ROW3):
                    move |= ROW4;
                    break;
                case(ROW4):
                    move |= ROW5;
                    break;
                case(ROW5):
                    move |= ROW6;
                    break;
                case(ROW6):
                    move |= ROW7;
                    break;
                case(ROW7):
                    move |= ROW8;
                    break;
            }
            if(isGold)
                ARIMAABOARD.gWave++;
            else
                ARIMAABOARD.sWave--;
            break;
        case(DIRECTIONW):
            move |= DIRECTIONE;
            move &= ~(collumMask);
            switch (lastMove & collumMask){
                case(COLLUMh):
                    move |= COLLUMg;
                    break;
                case(COLLUMg):
                    move |= COLLUMf;
                    break;
                case(COLLUMf):
                    move |= COLLUMe;
                    break;
                case(COLLUMe):
                    move |= COLLUMd;
                    break;
                case(COLLUMd):
                    move |= COLLUMc;
                    break;
                case(COLLUMc):
                    move |= COLLUMb;
                    break;
                case(COLLUMb):
                    move |= COLLUMa;
                    break;
            }
            break;
        default:
            printf("move corrupt\n");
            return 1;
    }
    updateBoardBit(move);
    return 0;
}

int undoTraps(uint16_t updatedTraps){
    /*takes in the 16 bit number from update traps 
      and puts animals that have been taken off back*/
    int squares[4] = {18, 21, 42, 45};
    for (int i = 0; i < 4; i++){
        uint16_t test = ((trapMask << 4*i) & updatedTraps);
        if (test == 0) continue;

        if (test == (RTRAPPED << 4*i)){
            ARIMAABOARD.gold[RABBIT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(true, RABBIT, squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[RABBIT];
            continue;
        }

        if (test == (CTRAPPED << 4*i)){
            ARIMAABOARD.gold[CAT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(true, CAT, squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[CAT];
            continue;
        }

        if (test == (DTRAPPED << 4*i)){
            ARIMAABOARD.gold[DOG] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(true, DOG, squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[DOG];
            continue;
        }

        if (test == (HTRAPPED << 4*i)){
            ARIMAABOARD.gold[HORSE] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(true, HORSE, squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[HORSE];
            continue;
        }

        if (test == (MTRAPPED << 4*i)){
            ARIMAABOARD.gold[CAMMEL] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(true, CAMMEL, squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[CAMMEL];
            continue;
        }

        if (test == (ETRAPPED << 4*i)){
            ARIMAABOARD.gold[ELEPHANT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(true, ELEPHANT, squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[ELEPHANT];
            continue;
        }

        if (test == (rTRAPPED << 4*i)){
            ARIMAABOARD.silver[RABBIT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(false, RABBIT, squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[RABBIT];
            continue;
        }

        if (test == (cTRAPPED << 4*i)){
            ARIMAABOARD.silver[CAT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(false, CAT, squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[CAT];
            continue;
        }

        if (test == (dTRAPPED << 4*i)){
            ARIMAABOARD.silver[DOG] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(false, DOG, squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[DOG];
            continue;
        }

        if (test == (hTRAPPED << 4*i)){
            ARIMAABOARD.silver[HORSE] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(false, HORSE, squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[HORSE];
            continue;
        }

        if (test == (mTRAPPED << 4*i)){
            ARIMAABOARD.silver[CAMMEL] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(false, CAMMEL, squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[CAMMEL];
            continue;
        }

        if (test == (eTRAPPED << 4*i)){
            ARIMAABOARD.silver[ELEPHANT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            setupArray(false, ELEPHANT, squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[ELEPHANT];
            continue;
        }
        printf("Error in undoinging traps\n");
        exit(1);
    }
    return 0;
}
