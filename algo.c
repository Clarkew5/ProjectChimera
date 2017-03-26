#include "algo.h"

/*----------------------------------------------------------------------------*/

/*the functions between the two lines are taken from a paper.
  they generate the bitboards used to understand moves*/

uint64_t rEdge = ((1L      ) | (1L <<  8) | (1L << 16) | (1L << 24) | 
                  (1L << 32) | (1L << 40) | (1L << 48) | (1L << 56));

uint64_t lEdge = ((1L <<  7) | (1L << 15) | (1L << 23) | (1L << 31) |
                  (1L << 39) | (1L << 47) | (1L << 55) | (1L << 63));

uint64_t near(uint64_t bitboard, int i, bool isGold){
    uint64_t tempNear = 0L;
    if (isGold){
        tempNear =
        ((bitboard << 1) & ~rEdge) | // West
        ((bitboard >> 1) & ~lEdge) | // East
         (bitboard >> 8)           ; // North
        if(i != 0)
            tempNear |= (bitboard << 8); // South
    }
    else{
        tempNear =
        ((bitboard << 1) & ~rEdge) | // West
        ((bitboard >> 1) & ~lEdge) | // East
         (bitboard << 8)           ; // South
        if(i != 0)
            tempNear |= (bitboard >> 8); // North
    }
    return tempNear;
}

uint64_t nearNoRestrict(uint64_t bitboard){
    uint64_t tempNear = 
        ((bitboard << 1) & ~rEdge) | // West
        ((bitboard >> 1) & ~lEdge) | // East
         (bitboard << 8)           | // South
         (bitboard >> 8)           ; // North

    return tempNear;
}

uint64_t strongEnemyNear(int i, bool isGold){
    uint64_t stronger = 0L;
    uint64_t tempStrongEnemyNear = 0L;
    if (isGold){
        for (int j = i+1; j < 6; j++)
            stronger |= ARIMAABOARD.silver[j];
        tempStrongEnemyNear = nearNoRestrict(stronger) & ARIMAABOARD.gold[i];
    }
    else{
        for (int j = i+1; j < 6; j++)
            stronger |= ARIMAABOARD.gold[j];
        tempStrongEnemyNear = nearNoRestrict(stronger) & ARIMAABOARD.silver[i];
    }

    return tempStrongEnemyNear;
}

uint64_t friendNear(int i, bool isGold){
    uint64_t tempFriendNear = 0L;
    if (isGold){
        uint64_t GOLD = 0L;
        for (int j = 0; j < 6; j++)
            GOLD |= ARIMAABOARD.gold[j];
        tempFriendNear = nearNoRestrict(GOLD) & ARIMAABOARD.gold[i];
    }
    else{
        uint64_t SILVER = 0L;
        for (int j = 0; j < 6; j++)
            SILVER |= ARIMAABOARD.silver[j];
        tempFriendNear = nearNoRestrict(SILVER) & ARIMAABOARD.silver[i];
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

uint64_t moveNoRestrict(int i, bool isGold){
    uint64_t tempMove = 0L;
    if (isGold)
         tempMove = nearNoRestrict(ARIMAABOARD.gold[i]);
    else
         tempMove = nearNoRestrict(ARIMAABOARD.silver[i]);

    tempMove &= ARIMAABOARD.empty;
    return tempMove;
}

uint64_t pushFull(int i, bool isGold){
    uint64_t weaker = 0L;
    if (isGold)
        for (int j = i-1; j >= 0; j--)
            weaker |= ARIMAABOARD.silver[j];
        
    else
        for (int j = i-1; j >= 0; j--)
            weaker |= ARIMAABOARD.gold[j];

    uint64_t tempPushFull = 
        nearNoRestrict((nearNoRestrict(weaker) & ARIMAABOARD.empty)) & weaker;
    return tempPushFull;
}

uint64_t push(int i, bool isGold){
    uint64_t notFrozen = 0L;    
    if (isGold)
         notFrozen = ARIMAABOARD.gold[i] & ~frozen(i, isGold);
    else
         notFrozen = ARIMAABOARD.silver[i] & ~frozen(i, isGold);

    uint64_t tempPush = nearNoRestrict(notFrozen) & pushFull(i, isGold);
    return tempPush;
}

uint64_t pullFull(int i, bool isGold){
    uint64_t weaker = 0L;
    uint64_t notFrozen = 0L; 
    if (isGold){
        for (int j = i-1; j >= 0; j--)
            weaker |= ARIMAABOARD.silver[j];
        notFrozen = ARIMAABOARD.gold[i] & ~frozen(i, isGold);
    }
    else{
        for (int j = i-1; j >= 0; j--)
            weaker |= ARIMAABOARD.gold[j];
        notFrozen = ARIMAABOARD.silver[i] & ~frozen(i, isGold);
    }

    uint64_t tempPullFull = nearNoRestrict(notFrozen) & weaker;
    return tempPullFull;
}

uint64_t pull(int i, bool isGold){
    uint64_t notFrozen = 0L;    
    if (isGold)
         notFrozen = ARIMAABOARD.gold[i] & ~frozen(i, isGold);
    else
         notFrozen = ARIMAABOARD.silver[i] & ~frozen(i, isGold);
    uint64_t tempPull = 
        nearNoRestrict(nearNoRestrict(pullFull(i, isGold)) & notFrozen) & 
            ARIMAABOARD.empty;
    return tempPull;
}


/*----------------------------------------------------------------------------*/

int printBitboard(uint64_t bitboard){
    //prints out a bitboard and is used for testing only
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
    /* makes all the bitboards for each game state. 
       Commented out code was used for testing*/
    uint64_t *bitboards = malloc(16 * sizeof(uint64_t));
    if (bitboards == NULL){
        printf("malloc failed");
        exit(1);
    }
    for (int p = 0; p < 6; p++){ //makes the move bitboards for each piece
        //printf("%d\n", p);
        *(bitboards + p) = move(p, isGold);
        //printBitboard(*(bitboards + p));
        //printf("\n-------------------------\n");
    }
    for (int p = 6; p < 11; p++){ //makes the push bitboards for each piece
        //printf("%d\n", p);
        *(bitboards + p) = push(p-5, isGold);
        //printBitboard(*(bitboards + p));
        //printf("\n-------------------------\n");
    }
    for (int p = 11; p < 16; p++){ //makes the pull bitboards for each piece
        //printf("%d\n", p);
        *(bitboards + p) = pull(p-10, isGold);
        //printBitboard(*(bitboards + p));
        //printf("\n-------------------------\n");
    }
    //printf("moves: %d\n", numberOfMoves(bitboards, isGold));
    //printf("pushes: %d\n", numberOfPushes(bitboards, isGold));
    //printf("pulls: %d\n", numberOfPulls(bitboards, isGold));
    return bitboards;
}

int numberOfMoves(uint64_t *bitboards, bool isGold){
   /* goes through and counts the number of moves each piece can make.*/
    int numOfMoves = 0;
    int numOfMovesNorth = 0;
    int numOfMovesEast = 0;
    int numOfMovesSouth = 0;
    int numOfMovesWest = 0;
    for (int p = 0; p < 6; p++){
        for (int shift = 0; shift < 64; shift++){
            //these two loops are used to scan all the bitboads
            uint64_t i = 1L << shift;
            uint64_t piece = 0L;
            if (isGold)
                piece = i & ARIMAABOARD.gold[p];
            else
                piece = i & ARIMAABOARD.silver[p];

            if (piece == i){ //is only exicuted if a piece is on square i
                //checks if a move is valid around it
                if((piece >> 8 != 0L) &&
                    !(p == 0 && !isGold) &&
                    ((piece >> 8) & *(bitboards+p)) == (piece >> 8))
                        numOfMovesNorth++;
                if((piece >> 1 != 0L) &&
                    ((piece >> 1) & *(bitboards+p) & ~lEdge) == (piece >> 1))
                        numOfMovesEast++;
                if((piece << 1 != 0L) &&
                    ((piece << 1) & *(bitboards+p) & ~rEdge) == (piece << 1))
                        numOfMovesWest++;
                if((piece << 8 != 0L) &&
                    !(p == 0 && isGold) &&
                    ((piece << 8) & *(bitboards+p)) == (piece << 8))
                        numOfMovesSouth++;  
            }
        }
    }
    /*the above loop structuer is reused multiple times through-out the code to 
      accomplish different tasks*/
    numOfMoves = 
        numOfMovesNorth + numOfMovesEast + numOfMovesSouth + numOfMovesWest;
    //printf("North: %d\n", numOfMovesNorth);
    //printf("East: %d\n", numOfMovesEast);
    //printf("South: %d\n", numOfMovesSouth);
    //printf("West: %d\n", numOfMovesWest);
    return numOfMoves;
}

int numberOfMovesForSquare(bool isGold, int shift){
    /*Counts the number of moves that can be made from a single square. 
      It is used for generating push moves.*/
    int numOfMoves = 0;
    uint64_t square = 0L;
    square = 1L << shift;
    uint64_t moveSquare;
 
    if (isGold){
        for(int p = 0; p < 6; p++)
            if ((ARIMAABOARD.gold[p] & square) == square){
                moveSquare = moveNoRestrict(p, isGold);
                break;
            }
    }  
    else{
        for(int p = 0; p < 6; p++)
            if ((ARIMAABOARD.silver[p] & square) == square){
                moveSquare = moveNoRestrict(p, isGold);
                break;
            }
    }

    if((square >> 8 != 0L) &&
        ((square >> 8) & moveSquare) == (square >> 8))
            numOfMoves++;
    if((square >> 1 != 0L) &&
        ((square >> 1) & moveSquare & ~lEdge) == (square >> 1))
            numOfMoves++;
    if((square << 1 != 0L) &&
        ((square << 1) & moveSquare & ~rEdge) == (square << 1))
            numOfMoves++;
    if((square << 8 != 0L) &&
        ((square << 8) & moveSquare) == (square << 8))
            numOfMoves++;
    return numOfMoves;
}

int numberOfMovesToSquare(bool isGold, int shift){
    /*Counts the number of moves that can be made to a single square. 
      It is used for generating pull moves.*/
    int numOfMoves = 0;
    uint64_t square = 1L << shift;
    uint64_t nearSquare = nearNoRestrict(square);
    for (int p = 0; p < 6; p++){
        if (isGold){
            if ((nearSquare & ARIMAABOARD.gold[p]) != 0L){
                if((square >> 8 != 0L) &&
                    ((square >> 8) & ARIMAABOARD.gold[p]) == (square >> 8))
                    numOfMoves++;
                if((square >> 1 != 0L) &&
                    ((square >> 1) & ARIMAABOARD.gold[p] & ~lEdge) == (square >> 1))
                    numOfMoves++;
                if((square << 1 != 0L) &&
                    ((square << 1) & ARIMAABOARD.gold[p] & ~rEdge) == (square << 1))
                    numOfMoves++;
                if((square << 8 != 0L) &&
                    ((square << 8) & ARIMAABOARD.gold[p]) == (square << 8))
                    numOfMoves++;
            }
        }
        else{
            if ((nearSquare & ARIMAABOARD.silver[p]) != 0L){
                if((square >> 8 != 0L) &&
                    ((square >> 8) & ARIMAABOARD.silver[p]) == (square >> 8))
                    numOfMoves++;
                if((square >> 1 != 0L) &&
                    ((square >> 1) & ARIMAABOARD.silver[p] & ~lEdge) == (square >> 1))
                    numOfMoves++;
                if((square << 1 != 0L) &&
                    ((square << 1) & ARIMAABOARD.silver[p] & ~rEdge) == (square << 1))
                    numOfMoves++;
                if((square << 8 != 0L) &&
                    ((square << 8) & ARIMAABOARD.silver[p]) == (square << 8))
                    numOfMoves++;
            }
        }
    }
    return numOfMoves;
}

int numberOfPushes(uint64_t *bitboards, bool isGold){
    //counts number of push moves
    int numOfPushes = 0;
    for (int p = 6; p < 11; p++){
        //bitboards 6-10 represent pushes that can be made
        for (int shift = 0; shift < 64; shift++){
            uint64_t square = 1L << shift;
            uint64_t piece = 0L;
            //p-5 gets the correct animal
            if (isGold)
                piece = square & ARIMAABOARD.gold[p-5];
            else
                piece = square & ARIMAABOARD.silver[p-5];

            if (piece == square){
                if((piece >> 8 != 0L) &&
                    ((piece >> 8) & *(bitboards+p)) == (piece >> 8))
                        numOfPushes += numberOfMovesForSquare(!isGold, shift-8);
                if((piece >> 1 != 0L) &&
                    ((piece >> 1) & *(bitboards+p) & ~lEdge) == (piece >> 1))
                        numOfPushes += numberOfMovesForSquare(!isGold, shift-1);
                if((piece << 1 != 0L) &&
                    ((piece << 1) & *(bitboards+p) & ~rEdge) == (piece << 1))
                        numOfPushes += numberOfMovesForSquare(!isGold, shift+1);
                if((piece << 8 != 0L) &&
                    ((piece << 8) & *(bitboards+p)) == (piece << 8))
                        numOfPushes += numberOfMovesForSquare(!isGold, shift+8);  
            }
        }
    }
    return numOfPushes;
}

int numberOfPulls(uint64_t *bitboards, bool isGold){
    int numOfPulls = 0;
    for (int p = 11; p < 16; p++){
        //bitboards 11-16 represent pulls that can be made
        for (int shift = 0; shift < 64; shift++){
            uint64_t square = 1L << shift;
            uint64_t piece = 0L;
            if (isGold)
                piece = square & ARIMAABOARD.gold[p-10];
            else
                piece = square & ARIMAABOARD.silver[p-10];

            if (piece == square){
                if((piece >> 8 != 0L) &&
                    ((piece >> 8) & *(bitboards+p)) == (piece >> 8))
                        numOfPulls += numberOfMovesToSquare(!isGold, shift);
                if((piece >> 1 != 0L) &&
                    ((piece >> 1) & *(bitboards+p) & ~lEdge) == (piece >> 1))
                        numOfPulls += numberOfMovesToSquare(!isGold, shift);
                if((piece << 1 != 0L) &&
                    ((piece << 1) & *(bitboards+p) & ~rEdge) == (piece << 1))
                        numOfPulls += numberOfMovesToSquare(!isGold, shift);
                if((piece << 8 != 0L) &&
                    ((piece << 8) & *(bitboards+p)) == (piece << 8))
                        numOfPulls += numberOfMovesToSquare(!isGold, shift);
            }
        }
    }
    return numOfPulls;
}



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
            if (isGold)
                ARIMAABOARD.gWave++;
            else
                ARIMAABOARD.sWave--;
            break;
        case(DIRECTIONE):
            i2 = i1 + 1;
            j2 = j1;
            break;
        case(DIRECTIONS):
            i2 = i1;
            j2 = j1 + 1;
            if (isGold)
                ARIMAABOARD.gWave--;
            else
                ARIMAABOARD.sWave++;
            break;
        case(DIRECTIONW):
            i2 = i1 - 1;
            j2 = j1;
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


uint16_t *movesOfASquare(bool isGold, uint64_t *bitboards, int shift, int numOfMoves){
    /*generates the moves that can be made from a square.
      This is used for pushes*/
    uint64_t square = 1L << shift;
    uint64_t piece = 0L;
    uint16_t *moves = malloc(numOfMoves * sizeof(uint16_t));
    if (moves == NULL){
        printf("malloc failed");
        exit(1);
    }
    int p = 0;
    if (isGold){
        for (int i = 0; i < 6; i++){
            if ((square & ARIMAABOARD.gold[i]) == square)
                p = i;
        }
        piece = square & ARIMAABOARD.gold[p];
    }
    else{
        for (int i = 0; i < 6; i++){
            if ((square & ARIMAABOARD.silver[i]) == square)
                p = i;
        }
        piece = square & ARIMAABOARD.silver[p];
    }

    for(int i = 0; i < numOfMoves;){
        if (piece == square){
            if((piece >> 8 != 0L) &&
                ((piece >> 8) & ARIMAABOARD.empty) == (piece >> 8)){
                    *(moves + i) = conMove(isGold,p,shift%8,shift/8,'n');
                    i++;
            }
            if((piece >> 1 != 0L) &&
                ((piece >> 1) & ARIMAABOARD.empty & ~lEdge) == (piece >> 1)){
                    *(moves + i) = conMove(isGold,p,shift%8,shift/8,'w');
                    i++;
            }
            if((piece << 1 != 0L) &&
                ((piece << 1) & ARIMAABOARD.empty & ~rEdge) == (piece << 1)){
                    *(moves + i) = conMove(isGold,p,shift%8,shift/8,'e');
                    i++;
            }
            if((piece << 8 != 0L) &&
                ((piece << 8) & ARIMAABOARD.empty) == (piece << 8)){
                    *(moves + i) = conMove(isGold,p,shift%8,shift/8,'s');
                    i++;  
            }
        }
    }
    return moves;
}

uint16_t *movesToASquare(bool isGold, uint64_t *bitboards, int shift, int numOfMoves){
    /*generates the moves that can be made from a square.
      This is used for pulls*/
    uint64_t square = 1L << shift;
    uint16_t *moves = malloc((numOfMoves) * sizeof(uint16_t));
    if (moves == NULL){
        printf("malloc failed");
        exit(1);
    }
    if (isGold){
        for (int i = 0; i < numOfMoves;){
            for(int p = 0; p < 6; p++){
                if((square << 8 != 0L) &&
                    (square << 8 & ARIMAABOARD.gold[p]) == (square << 8)){
                    *(moves + i) = conMove(isGold,p,shift%8,shift/8+1,'n');//north
                    i++;
                }
                if((square >> 1 != 0L) &&
                    (square >> 1 & ARIMAABOARD.gold[p] & ~lEdge) == (square >> 1)){
                    *(moves + i) = conMove(isGold,p,shift%8-1,shift/8,'e');//east
                    i++;
                }
                if((square << 1 != 0L) &&
                    (square << 1 & ARIMAABOARD.gold[p] & ~rEdge) == (square << 1)){
                    *(moves + i) = conMove(isGold,p,shift%8+1,shift/8, 'w');//west
                    i++;
                }
                if((square >> 8 != 0L) &&
                    (square >> 8 & ARIMAABOARD.gold[p]) == (square >> 8)){
                    *(moves + i) = conMove(isGold,p,shift%8,shift/8-1,'s');//south
                    i++;
                }
            }
        }
    }
    else{
        for (int i = 0; i < numOfMoves;){
            for(int p = 0; p < 6; p++){
                if((square << 8 != 0L) &&
                    (square << 8 & ARIMAABOARD.silver[p]) == (square << 8)){
                    *(moves + i) = conMove(isGold,p,shift%8,shift/8+1,'n');//north
                    i++;
                }
                if((square >> 1 != 0L) &&
                    (square >> 1 & ARIMAABOARD.silver[p] & ~lEdge) == (square >> 1)){
                    *(moves + i) = conMove(isGold,p,shift%8-1,shift/8,'e');//east
                    i++;
                }
                if((square << 1 != 0L) &&
                    (square << 1 & ARIMAABOARD.silver[p] & ~rEdge) == (square << 1)){
                    *(moves + i) = conMove(isGold,p,shift%8+1,shift/8, 'w');//west
                    i++;
                }
                if((square >> 8 != 0L) &&
                    (square >> 8 & ARIMAABOARD.silver[p]) == (square >> 8)){
                    *(moves + i) = conMove(isGold,p,shift%8,shift/8-1,'s');//south
                    i++;
                }
                
            }
        }
    }
    return moves;
}

uint16_t *generateMovesFromBoard(uint64_t *bitboards, bool isGold, int movesLeft){
    //generates all the single moves from a board state
    /*Could wrap multiple checks in one scan of each board so that there are 
      about 1/3 less the scans*/
    int numOfMoves = numberOfMoves(bitboards, isGold);
    int numOfPushes = 0;
    int numOfPulls = 0;
    if (movesLeft >= 2){ //it costs two moves to do a push or a pull
        numOfPushes = 2*numberOfPushes(bitboards, isGold);
        numOfPulls = 2*numberOfPulls(bitboards, isGold);
    }
    int size = numOfMoves + numOfPushes + numOfPulls;
    if (size == 0){
        printf("ERROR\n");
        exit(1);
    }
    uint16_t *moves = calloc(size, sizeof(uint16_t));
    if(moves == NULL){
        printf("calloc failed");
        exit(1);
    }

    for(int i = 0; i < numOfMoves;){//move
        *(moves + i) = 0L;
        for (int p = 0; p < 6; p++){
            for (int shift = 0; shift < 64; shift++){
                uint64_t square = 1L << shift;
                uint64_t piece = 0L;
                if (isGold)
                    piece = square & ARIMAABOARD.gold[p];
                else
                    piece = square & ARIMAABOARD.silver[p];

                if (piece == square){
                    if((piece >> 8 != 0L) &&
                        !(p == 0 && !isGold) &&
                        ((piece >> 8) & *(bitboards+p)) == (piece >> 8)){
                            *(moves + i) = conMove(isGold,p,shift%8,shift/8,'n');
                            i++;
                    }
                    if((piece >> 1 != 0L) &&
                        ((piece >> 1) & *(bitboards+p) & ~lEdge) == (piece >> 1)){
                            *(moves + i) = conMove(isGold,p,shift%8,shift/8,'w'); 
                            i++;
                    }
                    if((piece << 1 != 0L) &&
                        ((piece << 1) & *(bitboards+p) & ~rEdge) == (piece << 1)){
                            *(moves + i) = conMove(isGold,p,shift%8,shift/8,'e');
                            i++;
                    }
                    if((piece << 8 != 0L) &&
                        !(p == 0 && isGold) &&
                        ((piece << 8) & *(bitboards+p)) == (piece << 8)){
                            *(moves + i) = conMove(isGold,p,shift%8,shift/8,'s'); 
                            i++;  
                    }
                }
            }
        }
    }
    for(int i = numOfMoves; i < (size - numOfPulls);){//push
        /*if numOfPulls and numOfPushes is 0 then the for loop breaks right away
          because numOfMoves = size*/
        for (int p = 1; p < 6; p++){
            for (int shift = 0; shift < 64; shift++){
                uint64_t square = 1L << shift;
                uint64_t piece = 0L;
                if (isGold)
                    piece = square & ARIMAABOARD.gold[p];
                else
                    piece = square & ARIMAABOARD.silver[p];

                if (piece == square){
                    if((piece >> 8 != 0L) &&
                        ((piece >> 8) & *(bitboards+p+5)) == (piece >> 8)){
                            int pushes = numberOfMovesForSquare(!isGold, shift-8);
                            uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift-8, pushes); 
                            for (int j = 0; j  < pushes; j++){
                                *(moves + i) = *(pushMoves + j);
                                *(moves + i + 1) = conMove(isGold,p,shift%8,shift/8,'n');
                                i+=2;
                            }
                            free(pushMoves);
                    }
                    if((piece >> 1 != 0L) &&
                        ((piece >> 1) & *(bitboards+p+5) & ~lEdge) == (piece >> 1)){
                            int pushes = numberOfMovesForSquare(!isGold, shift-1);
                            uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift-1, pushes); 
                            for (int j = 0; j < pushes; j++){
                                *(moves + i) = *(pushMoves + j);
                                *(moves + i + 1) = conMove(isGold,p,shift%8,shift/8,'w');
                                i+=2;
                            }
                            free(pushMoves);
                    }
                    if((piece << 1 != 0L) &&
                        ((piece << 1) & *(bitboards+p+5) & ~rEdge) == (piece << 1)){
                            int pushes = numberOfMovesForSquare(!isGold, shift+1);
                            uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift+1, pushes); 
                            for (int j = 0; j < pushes; j++){
                                *(moves + i) = *(pushMoves + j);
                                *(moves + i + 1) = conMove(isGold,p,shift%8,shift/8,'e');
                                i+=2;
                            }
                            free(pushMoves);
                    }
                    if((piece << 8 != 0L) &&
                        ((piece << 8) & *(bitboards+p+5)) == (piece << 8)){
                            int pushes = numberOfMovesForSquare(!isGold, shift+8);
                            uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift+8, pushes); 
                            for (int j = 0; j<pushes; j++){
                                *(moves + i) = *(pushMoves  +j);
                                *(moves + i + 1) = conMove(isGold,p,shift%8,shift/8,'s');
                                i+=2;
                            }
                            free(pushMoves);
                    } 
                }
            }
        }
    }
    for(int i = (numOfMoves + numOfPushes); i < size;){//pull
        /*if numOfPulls and numOfPushes is 0 then the for loop breaks right away
          because numOfMoves = size*/
        for (int p = 1; p < 6; p++){
            for (int shift = 0; shift < 64; shift++){
                uint64_t square = 1L << shift;
                uint64_t piece = 0L;
                if (isGold)
                    piece = square & ARIMAABOARD.gold[p];
                else
                    piece = square & ARIMAABOARD.silver[p];

                if (piece == square){
                    if((piece >> 8 != 0L) &&
                        ((piece >> 8) & *(bitboards + p + 10)) == (piece >> 8)){
                            int pulls = numberOfMovesToSquare(!isGold, shift);
                            uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                            for (int j = 0; j < pulls; j++){
                                *(moves + i) = conMove(isGold, p, shift%8, shift/8, 'n');
                                *(moves + i + 1) = *(pullMoves + j);
                                i += 2;
                            }
                            free(pullMoves);
                    }
                    if((piece >> 1 != 0L) &&
                        ((piece >> 1) & *(bitboards + p + 10) & ~lEdge) == (piece >> 1)){
                            int pulls = numberOfMovesToSquare(!isGold, shift);
                            uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                            for (int j = 0; j < pulls; j++){
                                *(moves + i) = conMove(isGold, p, shift%8, shift/8, 'w');
                                *(moves + i + 1) = *(pullMoves + j);
                                i += 2;
                            }
                            free(pullMoves);
                    }
                    if((piece << 1 != 0L) &&
                        ((piece << 1) & *(bitboards + p + 10) & ~rEdge) == (piece << 1)){
                            int pulls = numberOfMovesToSquare(!isGold, shift);
                            uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                            for (int j = 0; j < pulls; j++){
                                *(moves + i) = conMove(isGold, p, shift%8, shift/8, 'e');
                                *(moves + i + 1) = *(pullMoves + j);
                                i += 2;
                            }
                            free(pullMoves);
                    }
                    if((piece << 8 != 0L) &&
                        ((piece << 8) & *(bitboards + p + 10)) == (piece << 8)){
                            int pulls = numberOfMovesToSquare(!isGold, shift);
                            uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                            for (int j = 0; j<pulls; j++){
                                *(moves + i) = conMove(isGold, p, shift%8, shift/8, 's');
                                *(moves + i + 1) = *(pullMoves + j);
                                i += 2;
                            } 
                            free(pullMoves);
                    }
                    
                }
            }
        }
    }
    return moves;
}

int randomMovePushPull(uint64_t *bitboards, bool isGold, int movesLeft){
    //make one random move, push, or pull
    //getchar();
    uint16_t *moves = generateMovesFromBoard(bitboards, isGold, movesLeft);
    int numOfMoves = numberOfMoves(bitboards, isGold);
    int numOfPushes = 0;
    int numOfPulls = 0;
    if (movesLeft >= 2){
        numOfPushes = 2*numberOfPushes(bitboards, isGold);
        numOfPulls = 2*numberOfPulls(bitboards, isGold);
    }
    int size = numOfMoves + numOfPushes + numOfPulls;
    if (size == 0){
        //if (isGold)
            //printf("Silver wins, Gold has no more moves\n");
        //else
            //printf("Gold wins, Silver has no more moves\n");
        return 3;
    } 
    time_t t;
    srand((unsigned) time(&t));
    //for (int i = 0; i < size; i++){
        //printMove(*(moves+i));
    //}

    int randomIndex = (rand() % size);
    //printf("\n%d/%d ", randomIndex, size);
    //printf("moves: %d, pushes: %d, pulls: %d\n", numOfMoves, numOfPushes/2, numOfPulls/2);
    if (randomIndex >= 0 && randomIndex < numOfMoves){
        //printf("move\n");
        updateBoardBit(*(moves + randomIndex));
        printMove(*(moves + randomIndex));
        //printf("\n=========================================================\n");
        free(moves);
        return 0; 
    }
    else if (randomIndex >= numOfMoves && randomIndex < (size - numOfPulls)){
        //pushes and pulls start at even indexes
        if ((randomIndex - numOfMoves) % 2 == 1)
            randomIndex--;
        //printf("push\n");
        updateBoardBit(*(moves + randomIndex));
        printMove(*(moves + randomIndex));
        //printf("\n");
        updateBoardBit(*(moves + randomIndex + 1));
        printMove(*(moves + randomIndex + 1));
        //printf("\n=========================================================\n");
        free(moves);
        return 1;
    }
    else if (randomIndex >= (numOfMoves + numOfPushes) && randomIndex < size){
        if ((randomIndex - numOfMoves) % 2 == 1)
            randomIndex--;
        //printf("pull\n");
        updateBoardBit(*(moves + randomIndex));
        printMove(*(moves + randomIndex));
        //printf("\n");
        updateBoardBit(*(moves + randomIndex + 1));
        printMove(*(moves + randomIndex + 1));
        //printf("\n=========================================================\n");
        free(moves);
        return 2;
    }
    return 3;
}

bool inGoal(bool isGold){
    //checks if a rabbit made it to the opposing home row
    if (isGold){
        return  (((1L     ) | (1L << 1) | (1L << 2) | (1L << 3) | 
                  (1L << 4) | (1L << 5) | (1L << 6) | (1L << 7)) & 
                ARIMAABOARD.gold[0]) != 0L;
    }
    else{
        return  (((1L << 56) | (1L << 57) | (1L << 58) | (1L << 59) | 
                  (1L << 60) | (1L << 61) | (1L << 62) | (1L << 63)) & 
                ARIMAABOARD.silver[0]) != 0L;
    }
    return false;
}
 
bool gameOver(bool isGold){
    if(isGold){
        if (ARIMAABOARD.gold[0] == 0L){
            //printf("Silver Won by trapping Gold's rabbits\n");
            return true;
        }
        if (inGoal(isGold)){
            //printf("Gold got a rabbit to Silver's home row\n");
            return true;
        }
    }
    else{
        if (ARIMAABOARD.silver[0] == 0L){
            //printf("Gold Won by trapping Silver's rabbits\n");
            return true;
        }
        if (inGoal(isGold)){
            //printf("Silver got a rabbit to Gold's home row\n");
            return true;
        }
    }
    return false;
}

int randomAgent(bool isGold){
    int movesLeft = 4;
    uint64_t *bitboards;
    int typeOfMove;
    while(movesLeft > 0){
        bitboards = generateMoveBitboards(isGold);
        typeOfMove = randomMovePushPull(bitboards, isGold, movesLeft);
        /* randomMovePushPull returns 0 for move, 1 for push, 2 for pull, 
           3 for game over*/
        switch (typeOfMove){
            case 0:     
                movesLeft--;
                break;
            case 1:
            case 2:
                movesLeft -= 2;
                break;
            case 3:
                free(bitboards);
                exit(0);
        }
        updateTraps();
  
        //printBoard();
        free(bitboards);
        if(gameOver(isGold))
            break;
    }
    printf("\n");
    return 0;
}

int heuristic(bool isGold){
    int numOfGFrozen = 0;
    int numOfSFrozen = 0;
    int h;

    /*the heuristic uses the forward movement of pieces, the nubmer of frozen,
      and a basic weight given to each piece. Frozen peices are multiplied by
      their wegiht so the agent focuses more on freezing stronger pieces.*/
    
    for (int i = 0; i < 5; i++){
        numOfGFrozen += (ARIMAABOARD.weights[i] *
                         __builtin_popcountll(frozen(i, true)));
        numOfSFrozen += (ARIMAABOARD.weights[i] *
                         __builtin_popcountll(frozen(i, false)));
    }

    /* the wave value is updated in the move and unmove functions while material
       is updated when traps animals fall into traps*/
    if(isGold)
        h = (ARIMAABOARD.gMaterial - ARIMAABOARD.sMaterial)
          //+ (ARIMAABOARD.gWave - ARIMAABOARD.sWave)
          + (numOfSFrozen - numOfGFrozen);
    else
        h = (ARIMAABOARD.sMaterial - ARIMAABOARD.gMaterial)
          //+ (ARIMAABOARD.sWave - ARIMAABOARD.gWave)
          + (numOfGFrozen - numOfSFrozen);
    return h;
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
    //printMove(lastMove);
    //printf("-> ");
    //printMove(move);
    //printf("\n");
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
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[0];
            continue;
        }

        if (test == (CTRAPPED << 4*i)){
            ARIMAABOARD.gold[CAT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[1];
            continue;
        }

        if (test == (DTRAPPED << 4*i)){
            ARIMAABOARD.gold[DOG] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[2];
            continue;
        }

        if (test == (HTRAPPED << 4*i)){
            ARIMAABOARD.gold[HORSE] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[3];
            continue;
        }

        if (test == (MTRAPPED << 4*i)){
            ARIMAABOARD.gold[CAMMEL] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[4];
            continue;
        }

        if (test == (ETRAPPED << 4*i)){
            ARIMAABOARD.gold[ELEPHANT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.gMaterial += ARIMAABOARD.weights[5];
            continue;
        }

        if (test == (rTRAPPED << 4*i)){
            ARIMAABOARD.silver[RABBIT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[0];
            continue;
        }

        if (test == (cTRAPPED << 4*i)){
            ARIMAABOARD.silver[CAT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[1];
            continue;
        }

        if (test == (dTRAPPED << 4*i)){
            ARIMAABOARD.silver[DOG] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[2];
            continue;
        }

        if (test == (hTRAPPED << 4*i)){
            ARIMAABOARD.silver[HORSE] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[3];
            continue;
        }

        if (test == (mTRAPPED << 4*i)){
            ARIMAABOARD.silver[CAMMEL] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[4];
            continue;
        }

        if (test == (eTRAPPED << 4*i)){
            ARIMAABOARD.silver[ELEPHANT] ^= (1L << squares[i]);
            ARIMAABOARD.empty ^= (1L << squares[i]);
            ARIMAABOARD.sMaterial += ARIMAABOARD.weights[5];
            continue;
        }

    }
    return 0;
}

int generateMoves(bool isGold, uint16_t *moves, int movesLeft, struct Hash *moveHash){
    /*Recursivly generates moves and stores them in a hash table.
      The moves are stored in a array and are updated as recursive calls are 
      made. Uses similar code generateMovesFromBoard*/

    //getchar();
    if (movesLeft == 0){ //end case
        insertEntry(moves, moveHash);
        //for (int i = 0; i < 4; i++)
            //printMove(moves[i]);
        //printf("\n");
        //printBoard();
        return 0;
    }
    uint64_t *bitboards = generateMoveBitboards(isGold);
    int numOfMoves = numberOfMoves(bitboards, isGold);
    int numOfPushes = 0;
    int numOfPulls = 0;
    if (movesLeft >= 2){
        numOfPushes = 2*numberOfPushes(bitboards, isGold);
        numOfPulls = 2*numberOfPulls(bitboards, isGold);
    }
    int size = numOfMoves + numOfPushes + numOfPulls;
    if (size == 0){
        free(bitboards);
        return 0;
    }
    for(int i = 0; i < numOfMoves;){//move
        for (int p = 0; p < 6; p++){
            for (int shift = 0; shift < 64; shift++){
                uint64_t square = 1L << shift;
                uint64_t piece = 0L;
                if (isGold)
                    piece = square & ARIMAABOARD.gold[p];
                else
                    piece = square & ARIMAABOARD.silver[p];

                if (piece == square){
                    if((piece >> 8 != 0L) &&
                        !(p == 0 && !isGold) &&
                        ((piece >> 8) & *(bitboards+p)) == (piece >> 8)){

                            uint16_t lastMove = conMove(isGold,p,shift%8,shift/8,'n');
                            updateBoardBit(lastMove);
                            uint16_t updatedTraps = updateTraps();
                            moves[4-movesLeft] = lastMove;
                            //make move

                            generateMoves(isGold, moves, movesLeft-1, moveHash);
                            //recursive call

                            undoTraps(updatedTraps);
                            undoMove(lastMove);
                            //undo move
                            
                            i++;
                    }
                    if((piece >> 1 != 0L) &&
                        ((piece >> 1) & *(bitboards+p) & ~lEdge) == (piece >> 1)){

                            uint16_t lastMove = conMove(isGold,p,shift%8,shift/8,'w');
                            updateBoardBit(lastMove);
                            uint16_t updatedTraps = updateTraps();
                            moves[4-movesLeft] = lastMove;

                            generateMoves(isGold, moves, movesLeft-1, moveHash);

                            undoTraps(updatedTraps);
                            undoMove(lastMove);         

                            i++;
                    }
                    if((piece << 1 != 0L) &&
                        ((piece << 1) & *(bitboards+p) & ~rEdge) == (piece << 1)){

                            uint16_t lastMove = conMove(isGold,p,shift%8,shift/8,'e');
                            updateBoardBit(lastMove);
                            uint16_t updatedTraps = updateTraps();
                            moves[4-movesLeft] = lastMove;

                            generateMoves(isGold, moves, movesLeft-1, moveHash);

                            undoTraps(updatedTraps);
                            undoMove(lastMove);
                            
                            i++;
                    }
                    if((piece << 8 != 0L) &&
                        !(p == 0 && isGold) &&
                        ((piece << 8) & *(bitboards+p)) == (piece << 8)){

                            uint16_t lastMove = conMove(isGold,p,shift%8,shift/8,'s');
                            updateBoardBit(lastMove);
                            uint16_t updatedTraps = updateTraps();
                            moves[4-movesLeft] = lastMove;

                            generateMoves(isGold, moves, movesLeft-1, moveHash);

                            undoTraps(updatedTraps);
                            undoMove(lastMove);
                            
                            i++;
                    }
                }
            }
        }
    }
    for(int i = numOfMoves; i < (size - numOfPulls);){//push
        for (int p = 1; p < 6; p++){
            for (int shift = 0; shift < 64; shift++){
                uint64_t square = 1L << shift;
                uint64_t piece = 0L;
                if (isGold)
                    piece = square & ARIMAABOARD.gold[p];
                else
                    piece = square & ARIMAABOARD.silver[p];

                if (piece == square){
                    if((piece >> 8 != 0L) &&
                        ((piece >> 8) & *(bitboards+p+5)) == (piece >> 8)){
                            int pushes = numberOfMovesForSquare(!isGold, shift-8);
                            uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift-8, pushes); 
                            for (int j = 0; j  < pushes; j++){

                                uint16_t lastMove = *(pushMoves + j);
                                updateBoardBit(lastMove);
                                uint16_t updatedTraps = updateTraps();
                                moves[4-movesLeft] = lastMove;

                                uint16_t lastMove2 = conMove(isGold,p,shift%8,shift/8,'n');
                                updateBoardBit(lastMove2);
                                uint16_t updatedTraps2 = updateTraps();
                                moves[4-movesLeft+1] = lastMove2;

                                generateMoves(isGold, moves, movesLeft-2, moveHash);

                                undoTraps(updatedTraps2);
                                undoMove(lastMove2);
                                
                                undoTraps(updatedTraps);
                                undoMove(lastMove);
                                i += 2;
                            }
                            free(pushMoves);
                    }
                    if((piece >> 1 != 0L) &&
                        ((piece >> 1) & *(bitboards+p+5) & ~lEdge) == (piece >> 1)){
                            int pushes = numberOfMovesForSquare(!isGold, shift-1);
                            uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift-1, pushes); 
                            for (int j = 0; j < pushes; j++){

                                uint16_t lastMove = *(pushMoves + j);
                                updateBoardBit(lastMove);
                                uint16_t updatedTraps = updateTraps();
                                moves[4-movesLeft] = lastMove;

                                uint16_t lastMove2 = conMove(isGold,p,shift%8,shift/8,'w');
                                updateBoardBit(lastMove2);
                                uint16_t updatedTraps2 = updateTraps();
                                moves[4-movesLeft+1] = lastMove2;\

                                generateMoves(isGold, moves, movesLeft-2, moveHash);

                                undoTraps(updatedTraps2);
                                undoMove(lastMove2);

                                undoTraps(updatedTraps);
                                undoMove(lastMove);
                                i += 2;
                            }
                            free(pushMoves);
                    }
                    if((piece << 1 != 0L) &&
                        ((piece << 1) & *(bitboards+p+5) & ~rEdge) == (piece << 1)){
                            int pushes = numberOfMovesForSquare(!isGold, shift+1);
                            uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift+1, pushes); 
                            for (int j = 0; j < pushes; j++){

                                uint16_t lastMove = *(pushMoves + j);
                                updateBoardBit(lastMove);
                                uint16_t updatedTraps = updateTraps();
                                moves[4-movesLeft] = lastMove;

                                uint16_t lastMove2 = conMove(isGold,p,shift%8,shift/8,'e');
                                updateBoardBit(lastMove2);
                                uint16_t updatedTraps2 = updateTraps();
                                moves[4-movesLeft+1] = lastMove2;\

                                generateMoves(isGold, moves, movesLeft-2, moveHash);

                                undoTraps(updatedTraps2);
                                undoMove(lastMove2);

                                undoTraps(updatedTraps);
                                undoMove(lastMove);

                                i += 2;
                            }
                            free(pushMoves);
                    }
                    if((piece << 8 != 0L) &&
                        ((piece << 8) & *(bitboards+p+5)) == (piece << 8)){
                            int pushes = numberOfMovesForSquare(!isGold, shift+8);
                            uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift+8, pushes); 
                            for (int j = 0; j<pushes; j++){

                                uint16_t lastMove = *(pushMoves + j);
                                updateBoardBit(lastMove);
                                uint16_t updatedTraps = updateTraps();
                                moves[4-movesLeft] = lastMove;

                                uint16_t lastMove2 = conMove(isGold,p,shift%8,shift/8,'s');
                                updateBoardBit(lastMove2);
                                uint16_t updatedTraps2 = updateTraps();
                                moves[4-movesLeft+1] = lastMove2;

                                generateMoves(isGold, moves, movesLeft-2, moveHash);

                                undoTraps(updatedTraps2);
                                undoMove(lastMove2);

                                undoTraps(updatedTraps);
                                undoMove(lastMove);

                                i += 2;
                            }
                            free(pushMoves);
                    } 
                }
            }
        }
    }
    for(int i = (numOfMoves + numOfPushes); i < size;){//pull
        for (int p = 1; p < 6; p++){
            for (int shift = 0; shift < 64; shift++){
                uint64_t square = 1L << shift;
                uint64_t piece = 0L;
                if (isGold)
                    piece = square & ARIMAABOARD.gold[p];
                else
                    piece = square & ARIMAABOARD.silver[p];

                if (piece == square){
                    if((piece >> 8 != 0L) &&
                        ((piece >> 8) & *(bitboards + p + 10)) == (piece >> 8)){
                            int pulls = numberOfMovesToSquare(!isGold, shift);
                            uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                            for (int j = 0; j < pulls; j++){

                                uint16_t lastMove = conMove(isGold, p, shift%8, shift/8, 'n');
                                updateBoardBit(lastMove);
                                uint16_t updatedTraps = updateTraps();
                                moves[4-movesLeft] = lastMove;

                                uint16_t lastMove2 = *(pullMoves + j);
                                updateBoardBit(lastMove2);
                                uint16_t updatedTraps2 = updateTraps();
                                moves[4-movesLeft+1] = lastMove2;
                                

                                generateMoves(isGold, moves, movesLeft-2, moveHash);

                                undoTraps(updatedTraps2);
                                undoMove(lastMove2);

                                undoTraps(updatedTraps);
                                undoMove(lastMove);
                                
                                i += 2;
                            }
                            free(pullMoves);
                    }
                    if((piece >> 1 != 0L) &&
                        ((piece >> 1) & *(bitboards + p + 10) & ~lEdge) == (piece >> 1)){
                            int pulls = numberOfMovesToSquare(!isGold, shift);
                            uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                            for (int j = 0; j < pulls; j++){

                                uint16_t lastMove = conMove(isGold, p, shift%8, shift/8, 'w');
                                updateBoardBit(lastMove);
                                uint16_t updatedTraps = updateTraps();
                                moves[4-movesLeft] = lastMove;

                                uint16_t lastMove2 = *(pullMoves + j);
                                updateBoardBit(lastMove2);
                                uint16_t updatedTraps2 = updateTraps();
                                moves[4-movesLeft+1] = lastMove2;

                                generateMoves(isGold, moves, movesLeft-2, moveHash);

                                undoTraps(updatedTraps2);
                                undoMove(lastMove2);

                                undoTraps(updatedTraps);
                                undoMove(lastMove);
                                
                                i += 2;
                            }
                            free(pullMoves);
                    }
                    if((piece << 1 != 0L) &&
                        ((piece << 1) & *(bitboards + p + 10) & ~rEdge) == (piece << 1)){
                            int pulls = numberOfMovesToSquare(!isGold, shift);
                            uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                            for (int j = 0; j < pulls; j++){

                                uint16_t lastMove = conMove(isGold, p, shift%8, shift/8, 'e');
                                updateBoardBit(lastMove);
                                uint16_t updatedTraps = updateTraps();
                                moves[4-movesLeft] = lastMove;

                                uint16_t lastMove2 = *(pullMoves + j);
                                updateBoardBit(lastMove2);
                                uint16_t updatedTraps2 = updateTraps();
                                moves[4-movesLeft+1] = lastMove2;
                                

                                generateMoves(isGold, moves, movesLeft-2, moveHash);

                                undoTraps(updatedTraps2);
                                undoMove(lastMove2);

                                undoTraps(updatedTraps);
                                undoMove(lastMove);
                                
                                i += 2;
                            }
                            free(pullMoves);
                    }
                    if((piece << 8 != 0L) &&
                        ((piece << 8) & *(bitboards + p + 10)) == (piece << 8)){
                            int pulls = numberOfMovesToSquare(!isGold, shift);
                            uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                            for (int j = 0; j<pulls; j++){

                                uint16_t lastMove = conMove(isGold, p, shift%8, shift/8, 's');
                                updateBoardBit(lastMove);
                                uint16_t updatedTraps = updateTraps();
                                moves[4-movesLeft] = lastMove;

                                uint16_t lastMove2 = *(pullMoves + j);
                                updateBoardBit(lastMove2);
                                uint16_t updatedTraps2 = updateTraps();
                                moves[4-movesLeft+1] = lastMove2;

                                generateMoves(isGold, moves, movesLeft-2, moveHash);

                                undoTraps(updatedTraps2);
                                undoMove(lastMove2);

                                undoTraps(updatedTraps);
                                undoMove(lastMove);
                                
                                i += 2;
                            } 
                            free(pullMoves);
                    }
                    
                }
            }
        }
    }
    free(bitboards);
    return 0;
}
    


struct Hash *branchHash(bool isGold){
    uint16_t *moves = calloc(4, sizeof(uint16_t));
    struct Hash *moveHash = createHash(6011);
    //large enough to all the unique moves
    generateMoves(isGold, moves, 4, moveHash); //initial recursive call
    //printf("number of elements in hash: %d\n", moveHash->numOfElements);
    //getchar();
    //printTable(moveHash);
    free(moves);
    return moveHash;
}

int negaMax(int depth, bool isGold, int A, int B, double tTime, time_t startTime){
    if (gameOver(isGold)){
        return INT_MAX;
    }
    if (depth == 0){ //endcase
        int h = heuristic(isGold);
        //int h = rand()%1000 + 1;
        return h;
    }

    int max = INT_MIN;
    struct Hash *branches = branchHash(isGold);//makes all the moves
    for (int i = 0; i < branches->size; i++){ 
        struct Entry *p = *(branches->entries + i);
        while(p != NULL){
            //steps through the hash function
            uint16_t traps[4] = {0, 0, 0, 0};
            //make the moves
            for (int j = 0; j < 4; j++){
                updateBoardBit(p->move[j]);
                traps[j] = updateTraps();
            }
            //recursive call
            int score = -negaMax(depth-1, !isGold, -B, -A, tTime, startTime);
            //undos moves
            for (int j = 0; j < 4; j++){
                undoTraps(traps[3-j]);
                undoMove(p->move[3-j]);
            }

            if (score > max)
                max = score;
            if (score > A)
                A = score;
            if (A >= B){
                destroyHash(branches);
                return A;
            }
            p = p->next;
        }
    }
    destroyHash(branches);
    return max;
}

int negaMaxSearch(bool isGold, double tTime){
    time_t startTime = time(NULL);
    int A = INT_MIN;
    int B = INT_MAX;
    int max = INT_MIN;
    uint16_t *maxMoves = calloc(4, sizeof(uint16_t));
    struct Hash *branches = branchHash(isGold);//makes all the moves
    int numOfElementsChecked = 0;
    printf("000");
    for (int depth = 3; depth < 4; depth += 2){
        for (int i = 0; i < branches->size; i++){ 
            struct Entry *p = *(branches->entries + i);
            while(p != NULL){
                //steps through the hash function
                uint16_t traps[4] = {0, 0, 0, 0};
                //make the moves
                for (int j = 0; j < 4; j++){
                    updateBoardBit(p->move[j]);
                    traps[j] = updateTraps();
                }
                //recursive call
                int score = -negaMax(depth-1, !isGold, -B, -A, tTime, startTime);
                //undos moves
                for (int j = 0; j < 4; j++){
                    undoTraps(traps[3-j]);
                    undoMove(p->move[3-j]);
                }

                if (score > max){
                    //printf("[");
                    for(int j = 0; j < 4; j++){
                       *(maxMoves + j) = *(p->move + j);
                        //printMove(*(maxMoves + j));
                    }
                    //printf("] h:%d\n", score);
                        //copy max moves to maxMoves array
                    max = score;
                }
                p = p->next;
                numOfElementsChecked++;
                printf("\b\b\b%03.0f", ((double)numOfElementsChecked/(double)branches->numOfElements) * 100);
            }
        }
    }
    destroyHash(branches);
    printf("\n");
    //make moves and print them out
    for (int i = 0; i < 4; i++){ 
        updateBoardBit(*(maxMoves + i));
        updateTraps();
        printMove(*(maxMoves + i));
    }
    free(maxMoves);
    printf("\n");  
    return 0;
}
//------------------------------------------------------------------------------
bool goldWin(bool isGold){
    if(isGold){
        if (ARIMAABOARD.gold[0] == 0L)
            return false;
        if (inGoal(isGold))
            return true;
    }
    else{
        if (ARIMAABOARD.silver[0] == 0L)
            return true;
        if (inGoal(isGold))
            return false;
    }
    return false;
}

bool makeRandomMove(bool isGold, int movesLeft){
    bool goldWon;
    if (!gameOver(isGold)){
        if (movesLeft == 0){
            goldWon = makeRandomMove(!isGold, 4);
            //printf("switch\n");
            return goldWon;
        }

        uint64_t *bitboards = generateMoveBitboards(isGold);
        int numOfMoves = numberOfMoves(bitboards, isGold);
        int numOfPushes = 0;
        int numOfPulls = 0;
        if (movesLeft >= 2){
            numOfPushes = 2*numberOfPushes(bitboards, isGold);
            numOfPulls = 2*numberOfPulls(bitboards, isGold);
        }

        int size = numOfMoves + numOfPushes + numOfPulls;
        if (size == 0){
            free(bitboards);
            if (isGold)
                return false;
            else
                return true;
        }
        //printf("1\n");
        //for (int i = 0; i < 16; i++){
            //printf("%d\n-----------\n", i);
            //printBitboard(*(bitboards + i));
        //}
        uint16_t *moves = generateMovesFromBoard(bitboards, isGold, movesLeft);
        //printf("2\n");
        free(bitboards);


        int randomIndex = (rand() % size);
        //getchar();
        if (randomIndex >= 0 && randomIndex < numOfMoves){
            uint16_t move = *(moves + randomIndex);
            free(moves);
            //printf("move\n");
            updateBoardBit(move);
            uint16_t updatedTraps = updateTraps();
            //printBoard();
            goldWon = makeRandomMove(isGold, movesLeft-1);

            undoTraps(updatedTraps);
            undoMove(move);
        }
        else if (randomIndex >= numOfMoves && randomIndex < (size - numOfPulls)){
            //pushes and pulls start at even indexes
            if ((randomIndex - numOfMoves) % 2 == 1)
                randomIndex--;

            uint16_t move = *(moves + randomIndex);
            uint16_t move2 = *(moves + randomIndex + 1);
            free(moves);
            //printf("push\n");
            updateBoardBit(move);
            uint16_t updatedTraps = updateTraps();
            updateBoardBit(move2);
            uint16_t updatedTraps2 = updateTraps();
            //printBoard();
            goldWon = makeRandomMove(isGold, movesLeft-2);

            undoTraps(updatedTraps2);
            undoMove(move2);
            undoTraps(updatedTraps);
            undoMove(move);
        }
        else if (randomIndex >= (numOfMoves + numOfPushes) && randomIndex < size){
            if ((randomIndex - numOfMoves) % 2 == 1)
                randomIndex--;

            uint16_t move = *(moves + randomIndex);
            uint16_t move2 = *(moves + randomIndex + 1);
            free(moves);
            //printf("pull\n");
            updateBoardBit(move);
            uint16_t updatedTraps = updateTraps();
            updateBoardBit(move2);
            uint16_t updatedTraps2 = updateTraps();
            //printBoard();
            goldWon = makeRandomMove(isGold, movesLeft-2);

            undoTraps(updatedTraps2);
            undoMove(move2);
            undoTraps(updatedTraps);
            undoMove(move);
        }
        //printf("recuring\n");
        return goldWon;
    }
    //printf("recuring from leaf\n");
    return goldWin(isGold);
}

bool playRandomGame(bool isGold, uint16_t *move){
    bool goldWon;
    uint16_t updatedTraps[4];
    for (int i = 0; i < 4; i++){
        updateBoardBit(*(move + i));
        updatedTraps[i] = updateTraps();
    }
    //printf("\tplaying game\n");//printBoard();
    goldWon = makeRandomMove(!isGold, 4);
    //printf("\tgame played\n");//printBoard();
    for (int i = 0; i < 4; i++){
        undoTraps(updatedTraps[3 - i]);
        undoMove(*(move + (3 - i)));
    }
    
    return goldWon;
}


int monteCarloTS(bool isGold, double tTime){
    srand((unsigned) time(NULL));
    struct Hash *branches = branchHash(isGold);
    uint16_t **moves = malloc(branches->numOfElements * sizeof(uint16_t*));
    int *gamesWon = calloc(branches->numOfElements, sizeof(int));
    int j = 0;
    for (int i = 0; i < branches->size; i++){
        struct Entry *p = *(branches->entries + i);
        while(p != NULL){
            *(moves + j) = calloc(4, sizeof(uint16_t));
            for (int k = 0; k < 4; k++){
                *(*(moves + j) + k) = *(p->move + k);
            }
            j++;
            p = p->next;
        }
    }
    //printf("moves copied\n");
    int numOfElements = branches->numOfElements;
    destroyHash(branches);
    int maxGamesWon = 0;
    int maxGamesWonIndex = 0;
    printf("000");
    for (int i = 0; i < numOfElements; i++){
        //printf("move: %d\n", i);
        for (int j = 0; j < 3; j++){
            //printf("\tgame: %d\n", j);
            //printBoard();
            bool goldWon = playRandomGame(isGold, *(moves + i));
            if(goldWon == isGold){
                (*(gamesWon + i))++;
                if(*(gamesWon + i) > maxGamesWon){
                    maxGamesWon = *(gamesWon + i);
                    maxGamesWonIndex = i;
                }
            }
        }
        printf("\b\b\b%03.0f", (double)i/(double)numOfElements*100);
    }
    printf("\b\b\b");
    for(int i = 0; i < 4; i++){
        updateBoardBit(*(*(moves + maxGamesWonIndex) + i));
        updateTraps();
        printMove(*(*(moves + maxGamesWonIndex) + i));
    }
    printf("\n");
    for(int i = 0; i < numOfElements; i++)
        free(*(moves + i));
    free(moves);
    free(gamesWon);
    return 0;
}
//----------------------------------------------------------------------------*/


