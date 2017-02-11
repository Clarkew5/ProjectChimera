#include "algo.h"

/*----------------------------------------------------------------------------*/

uint64_t lEdge = ((1L      ) | (1L <<  8) | (1L << 16) | (1L << 24) | 
                  (1L << 32) | (1L << 40) | (1L << 48) | (1L << 56));

uint64_t rEdge =  ((1L <<  7) | (1L << 15) | (1L << 23) | (1L << 31) |
                   (1L << 39) | (1L << 47) | (1L << 55) | (1L << 63));

uint64_t near(uint64_t bitboard, int i, bool isGold){
    uint64_t tempNear = 0L;
    if (isGold){
        tempNear =
        ((bitboard << 1) & ~rEdge) | // west
        ((bitboard >> 1) & ~lEdge) | // North
        (bitboard >> 8); // east
        if(i != 0)
            tempNear |= (bitboard << 8); // South
    }
    else{
        tempNear =
        ((bitboard << 1) & ~rEdge) | // west        
        ((bitboard >> 1) & ~lEdge) | // east
        (bitboard << 8); // South
        if(i != 0)
            tempNear |= (bitboard >> 8); // North
    }
    return tempNear;
}

uint64_t strongEnemyNear(int i, bool isGold){
    uint64_t stronger = 0L;
    uint64_t tempStrongEnemyNear = 0L;
    if (isGold){
        for (int j = i+1; j < 6; j++)
            stronger |= ARIMAABOARD.silver[j];
        tempStrongEnemyNear = near(stronger, i, isGold) & ARIMAABOARD.gold[i];
    }
    else{
        for (int j = i+1; j < 6; j++)
            stronger |= ARIMAABOARD.gold[j];
        tempStrongEnemyNear = near(stronger, i, isGold) & ARIMAABOARD.silver[i];
    }
    return tempStrongEnemyNear;
}

uint64_t friendNear(int i, bool isGold){
    uint64_t tempFriendNear = 0L;
    if (isGold){
        uint64_t GOLD = 0L;
        for (int i = 0; i < 6; i++)
            GOLD |= ARIMAABOARD.gold[i];
        tempFriendNear = near(GOLD, i, isGold) & ARIMAABOARD.gold[i];
    }
    else{
        uint64_t SILVER = 0L;
        for (int i = 0; i < 6; i++)
            SILVER |= ARIMAABOARD.silver[i];
        tempFriendNear = near(SILVER, i, isGold) & ARIMAABOARD.silver[i];
    }
    return tempFriendNear;
}

uint64_t frozen(int i, bool isGold){
    long tempFrozen = strongEnemyNear(i, isGold) & ~friendNear(i, isGold);
    return tempFrozen;
}

uint64_t move(int i, bool isGold){
    uint64_t notFrozen = 0L;    
    if (isGold)
         notFrozen = ARIMAABOARD.gold[i] & ~frozen(i, isGold);
    else
         notFrozen = ARIMAABOARD.silver[i] & ~frozen(i, isGold);
    uint64_t tempMove = ARIMAABOARD.empty & near(notFrozen, i, isGold);
    return tempMove;
}


uint64_t pushFull(int i, bool isGold){
    uint64_t weaker = 0;
    if (isGold)
        for (int j = i-1; j < 0; j--)
            weaker |= ARIMAABOARD.gold[j];
    else
        for (int j = i-1; j < 0; j--)
            weaker |= ARIMAABOARD.silver[j];
    uint64_t tempPushFull = 
        near(near(weaker, i, isGold) & ARIMAABOARD.empty, i, isGold) & weaker;
    return tempPushFull;
}

uint64_t push(int i, bool isGold){
    uint64_t notFrozen = 0L;    
    if (isGold)
         notFrozen = ARIMAABOARD.gold[i] & ~frozen(i, isGold);
    else
         notFrozen = ARIMAABOARD.silver[i] & ~frozen(i, isGold);

    uint64_t tempPush = near(notFrozen, i, isGold) & pushFull(i, isGold);
    return tempPush;
}

uint64_t pullFull(int i, bool isGold){
    uint64_t weaker = 0;
    if (isGold)
        for (int j = i-1; j < 0; j--)
            weaker |= ARIMAABOARD.gold[j];
    else
        for (int j = i-1; j < 0; j--)
            weaker |= ARIMAABOARD.silver[j];

    uint64_t notFrozen = 0L;    
    if (isGold)
         notFrozen = ARIMAABOARD.gold[i] & ~frozen(i, isGold);
    else
         notFrozen = ARIMAABOARD.silver[i] & ~frozen(i, isGold);

    uint64_t tempPullFull = near(notFrozen, i, isGold) & weaker;
    return tempPullFull;
}

uint64_t pull(int i, bool isGold){
    uint64_t notFrozen = 0L;    
    if (isGold)
         notFrozen = ARIMAABOARD.gold[i] & ~frozen(i, isGold);
    else
         notFrozen = ARIMAABOARD.silver[i] & ~frozen(i, isGold);
    uint64_t tempPull = 
        near(notFrozen & pullFull(i, isGold), i, isGold) & ARIMAABOARD.empty;
    return tempPull;
}


/*----------------------------------------------------------------------------*/

int printBitboard(uint64_t bitboard){
     for (int shift = 0; shift < 64; shift++){
        uint64_t i = 1L << shift;
        if ((bitboard & i) == i)
            printf("1");
        else
            printf("0");
        
        if (shift%8 == 7)
            printf("\n");
    }
    return 0;
}

uint64_t *generateMoveBitboards(bool isGold){
    uint64_t *bitboards = malloc(16 * sizeof(uint64_t)); 
    for (int p = 0; p < 6; p++){
        //printf("%d\n", p);
        *(bitboards + p) = move(p, isGold);
        //printBitboard(*(bitboards+p));
        //printf("\n-------------------------\n");
    }
    for (int p = 6; p < 11; p++){
        //printf("%d\n", p);
        *(bitboards + p) = push(p, isGold);
        //printBitboard(*(bitboards+p));
        //printf("\n-------------------------\n");
    }
    for (int p = 11; p < 16; p++){
        //printf("%d\n", p);
        *(bitboards + p) = pull(p, isGold);
        //printBitboard(*(bitboards+p));
        //printf("\n-------------------------\n");
    }
    return bitboards;
}

int numberOfMoves(uint64_t *bitboards, bool isGold){
   
    int numOfMoves = 0;
    int numOfMovesNorth = 0;
    int numOfMovesEast = 0;
    int numOfMovesSouth = 0;
    int numOfMovesWest = 0;
    for (int p = 0; p < 6; p++){
        for (int shift = 0; shift < 64; shift++){
            uint64_t i = 1L << shift;
            uint64_t peice = 0L;
            if (isGold)
                peice = i & ARIMAABOARD.gold[p];
            else
                peice = i & ARIMAABOARD.silver[p];

            if (peice == i){
                if((peice >> 8 != 0L) &&
                    !(p == 0 && !isGold) &&
                    ((peice >> 8) & *(bitboards+p)) == (peice >> 8))
                        numOfMovesNorth++;
                if((peice >> 1 != 0L) &&
                    ((peice >> 1) & *(bitboards+p) & ~rEdge) == (peice >> 1))
                        numOfMovesEast++;
                if((peice << 1 != 0L) &&
                    ((peice << 1) & *(bitboards+p) & ~lEdge) == (peice << 1))
                        numOfMovesWest++;
                if((peice << 8 != 0L) &&
                    !(p == 0 && isGold) &&
                    ((peice << 8) & *(bitboards+p)) == (peice << 8))
                        numOfMovesSouth++;  
            }
        }
    }
    numOfMoves = 
        numOfMovesNorth + numOfMovesEast + numOfMovesSouth + numOfMovesWest;
    //printf("North: %d\n", numOfMovesNorth);
    //printf("East: %d\n", numOfMovesEast);
    //printf("South: %d\n", numOfMovesSouth);
    //printf("West: %d\n", numOfMovesWest);
    return numOfMoves;
}

int numberOfMovesForSquare(bool isGold, int i, int shift){
    int numOfMoves = 0;
    uint64_t square = 1L << shift;
    if (isGold){
        for(int p = 0; p < 6; p++){
            if ((ARIMAABOARD.gold[i] & square) == square){
                uint64_t moveSquare = move(p, isGold);
                if((square >> 8 != 0L) &&
                    ((square >> 8) & moveSquare) == (square >> 8))
                        numOfMoves++;
                if((square >> 1 != 0L) &&
                    ((square >> 1) & moveSquare & ~rEdge) == (square >> 1))
                        numOfMoves++;
                if((square << 1 != 0L) &&
                    ((square << 1) & moveSquare & ~lEdge) == (square << 1))
                        numOfMoves++;
                if((square << 8 != 0L) &&
                    ((square << 8) & moveSquare) == (square << 8))
                        numOfMoves++;
            }
        }
    }  
    else{
        for(int p = 0; p < 6; p++){
            if ((ARIMAABOARD.silver[i] & square) == square){
                uint64_t moveSquare = move(p, isGold);
                if((square >> 8 != 0L) &&
                    ((square >> 8) & moveSquare) == (square >> 8))
                        numOfMoves++;
                if((square >> 1 != 0L) &&
                    ((square >> 1) & moveSquare & ~rEdge) == (square >> 1))
                        numOfMoves++;
                if((square << 1 != 0L) &&
                    ((square << 1) & moveSquare & ~lEdge) == (square << 1))
                        numOfMoves++;
                if((square << 8 != 0L) &&
                    ((square << 8) & moveSquare) == (square << 8))
                        numOfMoves++;
            }
        }
    }
    return numOfMoves;
}


int numberOfPushes(uint64_t *bitboards, bool isGold){
    int numOfPushes = 0;
    for (int p = 6; p < 11; p++){
        for (int shift = 0; shift < 64; shift++){
            uint64_t i = 1L << shift;
            uint64_t peice = 0L;
            if (isGold)
                peice = i & ARIMAABOARD.gold[p];
            else
                peice = i & ARIMAABOARD.silver[p];

            if (peice == i){
                if((peice >> 8 != 0L) &&
                    ((peice >> 8) & *(bitboards+p)) == (peice >> 8))
                        numOfPushes += numberOfMovesForSquare(isGold, (p-5), shift);
                if((peice >> 1 != 0L) &&
                    ((peice >> 1) & *(bitboards+p) & ~rEdge) == (peice >> 1))
                        numOfPushes += numberOfMovesForSquare(isGold, (p-5), shift);
                if((peice << 1 != 0L) &&
                    ((peice << 1) & *(bitboards+p) & ~lEdge) == (peice << 1))
                        numOfPushes += numberOfMovesForSquare(isGold, (p-5), shift);
                if((peice << 8 != 0L) &&
                    ((peice << 8) & *(bitboards+p)) == (peice << 8))
                        numOfPushes += numberOfMovesForSquare(isGold, (p-5), shift);  
            }
        }
    }
    return numOfPushes;
}

int numberOfPulls(uint64_t *bitboards, bool isGold){
    int numOfPulls = 0;
    for (int p = 11; p < 15; p++){
        for (int shift = 0; shift < 64; shift++){
            uint64_t i = 1L << shift;
            uint64_t peice = 0L;
            if (isGold)
                peice = i & ARIMAABOARD.gold[p];
            else
                peice = i & ARIMAABOARD.silver[p];

            if (peice == i){
                if((peice >> 8 != 0L) &&
                    ((peice >> 8) & *(bitboards+p)) == (peice >> 8))
                    numOfPulls++;
                if((peice >> 1 != 0L) &&
                    ((peice >> 1) & *(bitboards+p) & ~rEdge) == (peice >> 1))
                    numOfPulls++;
                if((peice << 1 != 0L) &&
                    ((peice << 1) & *(bitboards+p) & ~lEdge) == (peice << 1))
                    numOfPulls++;
                if((peice << 8 != 0L) &&
                    ((peice << 8) & *(bitboards+p)) == (peice << 8))
                    numOfPulls++;  
            }
        }
    }
    return numOfPulls;
}

uint16_t conMove(bool isGold, int peice, int i, int j, char direction){
    uint16_t bitMove = 0L;    
    if (isGold){
        switch (peice){
            case 0:
                bitMove |= GOLDR;
                break;
            case 1:
                bitMove |= GOLDC;
                break;
            case 2:
                bitMove |= GOLDD;
                break;
            case 3:
                bitMove |= GOLDH;
                break;
            case 4:
                bitMove |= GOLDM;
                break;
            case 5:
                bitMove |= GOLDE;
                break;
            case 6:
                return PASS;
        }
    }
    else{
        switch (peice){
            case 0:
                bitMove |= SILVERR;
                break;
            case 1:
                bitMove |= SILVERC;
                break;
            case 2:
                bitMove |= SILVERD;
                break;
            case 3:
                bitMove |= SILVERH;
                break;
            case 4:
                bitMove |= SILVERM;
                break;
            case 5:
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
            printf("r");
            break; 
        case(SILVERC):
            isGold = false;
            p = 1;
            printf("c");
            break;
        case(SILVERD):
            isGold = false;
            p = 2;
            printf("d");
            break;
        case(SILVERH):
            isGold = false;
            p = 3;
            printf("h");
            break;
        case(SILVERM):
            isGold = false;
            p = 4;
            printf("m");
            break;
        case(SILVERE):
            isGold = false;
            printf("e");
            p = 5;
            break;
        case(GOLDR):
            isGold = true;
            p = 0;
            printf("R");
            break;
        case(GOLDC):
            isGold = true;
            p = 1;
            printf("C");
            break;
        case(GOLDD):
            isGold = true;
            p = 2;
            printf("D");
            break;
        case(GOLDH):
            isGold = true;
            p = 3;
            printf("H");
            break;
        case(GOLDM):
            isGold = true;
            p = 4;
            printf("M");
            break;
        case(GOLDE):
            isGold = true;
            p = 5;
            printf("E");
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & collumMask){
        case(COLLUMa):
            i1 = 0;
            printf("a");
            break;
        case(COLLUMb):
            i1 = 1;
            printf("b");
            break;
        case(COLLUMc):
            i1 = 2;
            printf("c");
            break;
        case(COLLUMd):
            i1 = 3;
            printf("d");
            break;
        case(COLLUMe):
            i1 = 4;
            printf("e");
            break;
        case(COLLUMf):
            i1 = 5;
            printf("f");
            break;
        case(COLLUMg):
            i1 = 6;
            printf("g");
            break;
        case(COLLUMh):
            i1 = 7;
            printf("h");
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & rowMask){
        case(ROW1):
            j1 = 0;
            printf("8");
            break;
        case(ROW2):
            j1 = 1;
            printf("7");
            break;
        case(ROW3):
            j1 = 2;
            printf("6");
            break;
        case(ROW4):
            j1 = 3;
            printf("5");
            break;
        case(ROW5):
            j1 = 4;
            printf("4");
            break;
        case(ROW6):
            j1 = 5;
            printf("3");
            break;
        case(ROW7):
            j1 = 6;
            printf("2");
            break;
        case(ROW8):
            j1 = 7;
            printf("1");
            break;
        default:
            printf("move corrupt\n");
            break;
    }
    switch (move & directionMask){
        case(DIRECTIONN):
            i2 = i1;
            j2 = j1 - 1;
            printf("N ");
            break;
        case(DIRECTIONE):
            i2 = i1 - 1;
            j2 = j1;
            printf("E ");
            break;
        case(DIRECTIONS):
            i2 = i1;
            j2 = j1 + 1;
            printf("S ");
            break;
        case(DIRECTIONW):
            i2 = i1 + 1;
            j2 = j1;
            printf("W ");
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
        updateTraps(true, p, 8*j2 + i2);
    }
    else{
        ARIMAABOARD.silver[p] ^= 1L << ((8*j1) + i1);
        ARIMAABOARD.empty ^= 1L << ((8*j1) + i1);
        ARIMAABOARD.silver[p] ^= 1L << ((8*j2) + i2);
        ARIMAABOARD.empty ^= 1L << ((8*j2) + i2);
        updateTraps(false, p, 8*j2 + i2);
    }
    return 0;
}

int printMove(uint16_t move){
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


//int randomMove(int isGold, int p, int shift){

//}

int randomMovePushPull(uint64_t *bitboards, bool isGold, int movesLeft){
    int numOfMoves = numberOfMoves(bitboards, isGold);
    int numOfPushes = 0;
    int numOfPulls = 0;
    /*if (moveslefts >= 2){
        numOfPushes = numberOfPushes(bitboards, isGold);
        numOfPulls = numberOfPulls(bitboards, isGold);
    }*/
    int size = numOfMoves + numOfPushes + numOfPulls;
    if (size == 0){
        printf("You Win\n");
        exit(EXIT_SUCCESS);
    } 
    uint16_t *moves = calloc(size, sizeof(uint16_t));
    for(int i = 0; i < numOfMoves;){
        *(moves + i) = 0L;
        for (int p = 0; p < 6; p++){
            for (int shift = 0; shift < 64; shift++){
                uint64_t square = 1L << shift;
                uint64_t peice = 0L;
                if (isGold)
                    peice = square & ARIMAABOARD.gold[p];
                else
                    peice = square & ARIMAABOARD.silver[p];

                if (peice == square){
                    if((peice >> 8 != 0L) &&
                        !(p == 0 && !isGold) &&
                        ((peice >> 8) & *(bitboards+p)) == (peice >> 8)){
                            *(moves + i) = conMove(isGold,p,shift%8,shift/8,'n');
                            //printf("%d\n", shift%8);
                            //printf("%d ", i);                            
                            i++;
                    }
                    if((peice >> 1 != 0L) &&
                        ((peice >> 1) & *(bitboards+p) & ~rEdge) == (peice >> 1)){
                            *(moves + i) = conMove(isGold,p,shift%8,shift/8,'e');
                            //printf("%d ", i);  
                            i++;
                    }
                    if((peice << 1 != 0L) &&
                        ((peice << 1) & *(bitboards+p) & ~lEdge) == (peice << 1)){
                            *(moves + i) = conMove(isGold,p,shift%8,shift/8,'w');
                            //printf("%d ", i);
                            i++;
                    }
                    if((peice << 8 != 0L) &&
                        !(p == 0 && isGold) &&
                        ((peice << 8) & *(bitboards+p)) == (peice << 8)){
                            *(moves + i) = conMove(isGold,p,shift%8,shift/8,'s');
                            //printf("%d ", i);  
                            i++;  
                    }
                }
            }
        }
    }
    /*if (moveslefts >= 2){
        for(int i = numOfMoves; i < (size - numOfPulls); i++){//push
            for (int p = 6; p < 11; p++){
                for (int shift = 0; shift < 64; shift++){
                    uint64_t i = 1L << shift;
                    uint64_t peice = 0L;
                    if (isGold)
                        peice = i & ARIMAABOARD.gold[p];
                    else
                        peice = i & ARIMAABOARD.silver[p];

                    if (peice == i){
                        if((peice >> 8 != 0L) &&
                            ((peice >> 8) & *(bitboards+p)) == (peice >> 8)){
                                *(moves + i) = randomMove(!isGold, p, shift);
                                *(moves + i + 1) = conMove(isGold,p,shift%8,shift/8,'n');
                        }
                        if((peice >> 1 != 0L) &&
                            ((peice >> 1) & *(bitboards+p) & ~rEdge) == (peice >> 1)){
                                *(moves + i) = randomMove(!isGold, p, shift);
                                *(moves + i + 1) = conMove(isGold,p,shift%8,shift/8,'e');
                        }
                        if((peice << 1 != 0L) &&
                            ((peice << 1) & *(bitboards+p) & ~lEdge) == (peice << 1)){
                                *(moves + i) = randomMove(!isGold, p, shift);
                                *(moves + i + 1) = conMove(isGold,p,shift%8,shift/8,'w');
                        }
                        if((peice << 8 != 0L) &&
                            ((peice << 8) & *(bitboards+p)) == (peice << 8)){
                                *(moves + i) = randomMove(!isGold, p, shift);
                                *(moves + i + 1) = conMove(isGold,p,shift%8,shift/8,'s');
                        } 
                    }
                }
            }
        }
        for(int i = numOfPushes; i < size; i++){//pull
        }
    }*/
    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < size; i++){
        printMove(*(moves+i));
    }
    int randomIndex = (rand() % size);
    printf("\n%d/%d ", randomIndex, size);
    uint16_t move = *(moves + randomIndex);
    updateBoardBit(move);
    printf("\n");
    free(moves);
    return 0;
} 

int randomAgent(bool isGold){
    int movesLeft = 4;
    uint64_t *bitboards;
    while(movesLeft > 0){
        bitboards = generateMoveBitboards(isGold);
        randomMovePushPull(bitboards, isGold, movesLeft);
        movesLeft--;
        //free(bitboards);
    }
    printf("\n");
    free(bitboards);
    return 0;
}

