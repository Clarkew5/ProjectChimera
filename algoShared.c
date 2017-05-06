#include "algoShared.h"

/*----------------------------------------------------------------------------*/

/*the functions between the two lines are taken from the paper 
  "Arimaa: from rules to bitboard analysys" by Stefano Carlini, S Bergamaschi.
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

    for (int index = 0; index < 16; index++){
        uint64_t piece = 0L;
        int p = ARIMAABOARD.pieceIndexes[index];
        int shift = 0; 
        if (isGold){
            shift = ARIMAABOARD.goldPositions[index];
            if (shift == 64)
                continue;
            piece = ARIMAABOARD.gold[p] & (1L << shift);
        }
        else{
            shift = ARIMAABOARD.silverPositions[index];
            if (shift == 64)
                continue;
            piece = ARIMAABOARD.silver[p] & (1L << shift);
        }

        if (piece == 0){
            printf("miss match\n");
            exit(1);
        }

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

    for (int index = 8; index < 16; index++){
        uint64_t piece = 0L;
        int p = ARIMAABOARD.pieceIndexes[index];
        int shift = 0; 
        if (isGold){
            shift = ARIMAABOARD.goldPositions[index];
            if (shift == 64)
                continue;
            piece = ARIMAABOARD.gold[p] & (1L << shift);
        }
        else{
            shift = ARIMAABOARD.silverPositions[index];
            if (shift == 64)
                continue;
            piece = ARIMAABOARD.silver[p] & (1L << shift);
        }

        if (piece == 0){
            printf("miss match\n");
            exit(1);
        }

        uint64_t square = 1L << shift;
        if (isGold)
            piece = square & ARIMAABOARD.gold[p];
        else
            piece = square & ARIMAABOARD.silver[p];

        if((piece >> 8 != 0L) &&
            ((piece >> 8) & *(bitboards+p+5)) == (piece >> 8))
                numOfPushes += numberOfMovesForSquare(!isGold, shift-8);
        if((piece >> 1 != 0L) &&
            ((piece >> 1) & *(bitboards+p+5) & ~lEdge) == (piece >> 1))
                numOfPushes += numberOfMovesForSquare(!isGold, shift-1);
        if((piece << 1 != 0L) &&
            ((piece << 1) & *(bitboards+p+5) & ~rEdge) == (piece << 1))
                numOfPushes += numberOfMovesForSquare(!isGold, shift+1);
        if((piece << 8 != 0L) &&
            ((piece << 8) & *(bitboards+p+5)) == (piece << 8))
                numOfPushes += numberOfMovesForSquare(!isGold, shift+8);  
    }

    return numOfPushes;
}

int numberOfPulls(uint64_t *bitboards, bool isGold){
    int numOfPulls = 0;

    for (int index = 8; index < 16; index++){
        uint64_t piece = 0L;
        int p = ARIMAABOARD.pieceIndexes[index];
        int shift = 0; 
        if (isGold){
            shift = ARIMAABOARD.goldPositions[index];
            if (shift == 64)
                continue;
            piece = ARIMAABOARD.gold[p] & (1L << shift);
        }
        else{
            shift = ARIMAABOARD.silverPositions[index];
            if (shift == 64)
                continue;
            piece = ARIMAABOARD.silver[p] & (1L << shift);
        }

        if (piece == 0){
            printf("miss match\n");
            exit(1);
        }


        uint64_t square = 1L << shift;
        if (isGold)
            piece = square & ARIMAABOARD.gold[p];
        else
            piece = square & ARIMAABOARD.silver[p];

        if((piece >> 8 != 0L) &&
            ((piece >> 8) & *(bitboards+p+10)) == (piece >> 8))
                numOfPulls += numberOfMovesToSquare(!isGold, shift);
        if((piece >> 1 != 0L) &&
            ((piece >> 1) & *(bitboards+p+10) & ~lEdge) == (piece >> 1))
                numOfPulls += numberOfMovesToSquare(!isGold, shift);
        if((piece << 1 != 0L) &&
            ((piece << 1) & *(bitboards+p+10) & ~rEdge) == (piece << 1))
                numOfPulls += numberOfMovesToSquare(!isGold, shift);
        if((piece << 8 != 0L) &&
            ((piece << 8) & *(bitboards+p+10)) == (piece << 8))
                numOfPulls += numberOfMovesToSquare(!isGold, shift);
    }
    return numOfPulls;
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
        printf("no moves\n");
        return 0;//exit(1);
    }
    uint16_t *moves = calloc(size, sizeof(uint16_t));
    if(moves == NULL){
        printf("calloc failed");
        exit(1);
    }

    for(int i = 0; i < numOfMoves;){//move
        *(moves + i) = 0L;

        for (int index = 0; index < 16; index++){
            uint64_t piece = 0L;
            int p = ARIMAABOARD.pieceIndexes[index];
            int shift = 0; 
            if (isGold){
                shift = ARIMAABOARD.goldPositions[index];
                if (shift == 64)
                    continue;
                piece = ARIMAABOARD.gold[p] & (1L << shift);
            }
            else{
                shift = ARIMAABOARD.silverPositions[index];
                if (shift == 64)
                    continue;
                piece = ARIMAABOARD.silver[p] & (1L << shift);
            }

            if (piece == 0){
                printf("miss match\n");
                exit(1);
            }

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

    for(int i = numOfMoves; i < (size - numOfPulls);){//push
        /*if numOfPulls and numOfPushes is 0 then the for loop breaks right away
          because numOfMoves = size*/

        for (int index = 8; index < 16; index++){
            uint64_t piece = 0L;
            int p = ARIMAABOARD.pieceIndexes[index];
            int shift = 0; 
            if (isGold){
                shift = ARIMAABOARD.goldPositions[index];
                if (shift == 64)
                    continue;
                piece = ARIMAABOARD.gold[p] & (1L << shift);
            }
            else{
                shift = ARIMAABOARD.silverPositions[index];
                if (shift == 64)
                    continue;
                piece = ARIMAABOARD.silver[p] & (1L << shift);
            }

            if (piece == 0){
                printf("miss match\n");
                exit(1);
            }

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
    for(int i = (numOfMoves + numOfPushes); i < size;){//pull
        /*if numOfPulls and numOfPushes is 0 then the for loop breaks right away
          because numOfMoves = size*/

        for (int index = 8; index < 16; index++){
            uint64_t piece = 0L;
            int p = ARIMAABOARD.pieceIndexes[index];
            int shift = 0; 
            if (isGold){
                shift = ARIMAABOARD.goldPositions[index];
                if (shift == 64)
                    continue;
                piece = ARIMAABOARD.gold[p] & (1L << shift);
            }
            else{
                shift = ARIMAABOARD.silverPositions[index];
                if (shift == 64)
                    continue;
                piece = ARIMAABOARD.silver[p] & (1L << shift);
            }

            if (piece == 0){
                printf("miss match\n");
                exit(1);
            }

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

int generateMoves(bool isGold, uint16_t *moves, int movesLeft, struct Hash *moveHash){
    /*Recursivly generates moves and stores them in a hash table.
      The moves are stored in a array and are updated as recursive calls are 
      made. Uses similar code generateMovesFromBoard*/

    //getchar();
    if (movesLeft == 0){ //end case
        insertEntry(moves, moveHash);
        return 0;
    }

    if (movesLeft < 0){
        printf("moves left is negative\n");
        exit(1);
    }

    uint64_t *bitboards = generateMoveBitboards(isGold);

    //move
    for (int index = 0; index < 16; index++){
        uint64_t piece = 0L;
        int p = ARIMAABOARD.pieceIndexes[index];
        int shift = 0; 
        if (isGold){
            shift = ARIMAABOARD.goldPositions[index];
            if (shift == 64)
                continue;
            piece = ARIMAABOARD.gold[p] & (1L << shift);
        }
        else{
            shift = ARIMAABOARD.silverPositions[index];
            if (shift == 64)
                continue;
            piece = ARIMAABOARD.silver[p] & (1L << shift);
        }

        if (piece == 0L){
            printf("miss match\n");
            exit(1);
        }

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
        }
        
        if (p == 0 || movesLeft-2 < 0)
            continue;
        
        //pushes
        if((piece >> 8 != 0L) &&
            ((piece >> 8) & *(bitboards+p+5)) == (piece >> 8)){
                int pushes = numberOfMovesForSquare(!isGold, shift-8);
                uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift-8, pushes);
                for (int j = 0; j  < pushes; j++){

                    uint16_t lastMove = *(pushMoves + j);
                    uint16_t lastMove2 = conMove(isGold,p,shift%8,shift/8,'n');
                    updateBoardBit(lastMove);
                    updateBoardBit(lastMove2);

                    uint16_t updatedTraps = updateTraps();
                    uint16_t updatedTraps2 = updateTraps();

                    moves[4-movesLeft] = lastMove;
                    moves[4-movesLeft+1] = lastMove2;

                    generateMoves(isGold, moves, movesLeft-2, moveHash);

                    undoTraps(updatedTraps2);                                
                    undoTraps(updatedTraps);

                    undoMove(lastMove2);
                    undoMove(lastMove);
                }
                free(pushMoves);
        }
        if((piece >> 1 != 0L) &&
            ((piece >> 1) & *(bitboards+p+5) & ~lEdge) == (piece >> 1)){
                int pushes = numberOfMovesForSquare(!isGold, shift-1);
                uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift-1, pushes); 
                for (int j = 0; j < pushes; j++){

                    uint16_t lastMove = *(pushMoves + j);
                    uint16_t lastMove2 = conMove(isGold,p,shift%8,shift/8,'w');
                    updateBoardBit(lastMove);
                    updateBoardBit(lastMove2);

                    uint16_t updatedTraps = updateTraps();
                    uint16_t updatedTraps2 = updateTraps();

                    moves[4-movesLeft] = lastMove;
                    moves[4-movesLeft+1] = lastMove2;

                    generateMoves(isGold, moves, movesLeft-2, moveHash);

                    undoTraps(updatedTraps2);                                
                    undoTraps(updatedTraps);

                    undoMove(lastMove2);
                    undoMove(lastMove);
                }
                free(pushMoves);
        }
        if((piece << 1 != 0L) &&
            ((piece << 1) & *(bitboards+p+5) & ~rEdge) == (piece << 1)){
                int pushes = numberOfMovesForSquare(!isGold, shift+1);
                uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift+1, pushes); 
                for (int j = 0; j < pushes; j++){

                    uint16_t lastMove = *(pushMoves + j);
                    uint16_t lastMove2 = conMove(isGold,p,shift%8,shift/8,'e');
                    updateBoardBit(lastMove);
                    updateBoardBit(lastMove2);

                    uint16_t updatedTraps = updateTraps();
                    uint16_t updatedTraps2 = updateTraps();

                    moves[4-movesLeft] = lastMove;
                    moves[4-movesLeft+1] = lastMove2;

                    generateMoves(isGold, moves, movesLeft-2, moveHash);

                    undoTraps(updatedTraps2);                                
                    undoTraps(updatedTraps);

                    undoMove(lastMove2);
                    undoMove(lastMove);
                }
                free(pushMoves);
        }
        if((piece << 8 != 0L) &&
            ((piece << 8) & *(bitboards+p+5)) == (piece << 8)){
                int pushes = numberOfMovesForSquare(!isGold, shift+8);
                uint16_t *pushMoves = movesOfASquare(!isGold, bitboards, shift+8, pushes); 
                for (int j = 0; j < pushes; j++){

                    uint16_t lastMove = *(pushMoves + j);
                    uint16_t lastMove2 = conMove(isGold,p,shift%8,shift/8,'s');
                    updateBoardBit(lastMove);
                    updateBoardBit(lastMove2);

                    uint16_t updatedTraps = updateTraps();
                    uint16_t updatedTraps2 = updateTraps();

                    moves[4-movesLeft] = lastMove;
                    moves[4-movesLeft+1] = lastMove2;

                    generateMoves(isGold, moves, movesLeft-2, moveHash);

                    undoTraps(updatedTraps2);                                
                    undoTraps(updatedTraps);

                    undoMove(lastMove2);
                    undoMove(lastMove);
                }
                free(pushMoves);
        }
        //pulls
        if((piece >> 8 != 0L) &&
            ((piece >> 8) & *(bitboards + p + 10)) == (piece >> 8)){
                int pulls = numberOfMovesToSquare(!isGold, shift);
                uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                for (int j = 0; j < pulls; j++){

                    uint16_t lastMove = conMove(isGold, p, shift%8, shift/8, 'n');
                    uint16_t lastMove2 = *(pullMoves + j);
                    updateBoardBit(lastMove);
                    updateBoardBit(lastMove2);

                    uint16_t updatedTraps = updateTraps();
                    uint16_t updatedTraps2 = updateTraps();

                    moves[4-movesLeft] = lastMove;
                    moves[4-movesLeft+1] = lastMove2;
                    
                    generateMoves(isGold, moves, movesLeft-2, moveHash);

                    undoTraps(updatedTraps2);                                
                    undoTraps(updatedTraps);

                    undoMove(lastMove2);
                    undoMove(lastMove);
                }
                free(pullMoves);
        }
        if((piece >> 1 != 0L) &&
            ((piece >> 1) & *(bitboards + p + 10) & ~lEdge) == (piece >> 1)){
                int pulls = numberOfMovesToSquare(!isGold, shift);
                uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                for (int j = 0; j < pulls; j++){

                    uint16_t lastMove = conMove(isGold, p, shift%8, shift/8, 'w');
                    uint16_t lastMove2 = *(pullMoves + j);
                    updateBoardBit(lastMove);
                    updateBoardBit(lastMove2);

                    uint16_t updatedTraps = updateTraps();
                    uint16_t updatedTraps2 = updateTraps();

                    moves[4-movesLeft] = lastMove;
                    moves[4-movesLeft+1] = lastMove2;

                    generateMoves(isGold, moves, movesLeft-2, moveHash);

                    undoTraps(updatedTraps2);                                
                    undoTraps(updatedTraps);

                    undoMove(lastMove2);
                    undoMove(lastMove);
                }
                free(pullMoves);
        }
        if((piece << 1 != 0L) &&
            ((piece << 1) & *(bitboards + p + 10) & ~rEdge) == (piece << 1)){
                int pulls = numberOfMovesToSquare(!isGold, shift);
                uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                for (int j = 0; j < pulls; j++){

                    uint16_t lastMove = conMove(isGold, p, shift%8, shift/8, 'e');
                    uint16_t lastMove2 = *(pullMoves + j);
                    updateBoardBit(lastMove);
                    updateBoardBit(lastMove2);

                    uint16_t updatedTraps = updateTraps();
                    uint16_t updatedTraps2 = updateTraps();

                    moves[4-movesLeft] = lastMove;
                    moves[4-movesLeft+1] = lastMove2;                                

                    generateMoves(isGold, moves, movesLeft-2, moveHash);

                    undoTraps(updatedTraps2);                                
                    undoTraps(updatedTraps);

                    undoMove(lastMove2);
                    undoMove(lastMove);
                }
                free(pullMoves);
        }
        if((piece << 8 != 0L) &&
            ((piece << 8) & *(bitboards + p + 10)) == (piece << 8)){
                int pulls = numberOfMovesToSquare(!isGold, shift);
                uint16_t *pullMoves = movesToASquare(!isGold, bitboards, shift, pulls);
                for (int j = 0; j<pulls; j++){

                    uint16_t lastMove = conMove(isGold, p, shift%8, shift/8, 's');
                    uint16_t lastMove2 = *(pullMoves + j);
                    updateBoardBit(lastMove);
                    updateBoardBit(lastMove2);

                    uint16_t updatedTraps = updateTraps();
                    uint16_t updatedTraps2 = updateTraps();

                    moves[4-movesLeft] = lastMove;
                    moves[4-movesLeft+1] = lastMove2;

                    generateMoves(isGold, moves, movesLeft-2, moveHash);

                    undoTraps(updatedTraps2);                                
                    undoTraps(updatedTraps);

                    undoMove(lastMove2);
                    undoMove(lastMove);
                } 
                free(pullMoves);
        }
    
    }
    
    free(bitboards);
    return 0;
}

struct Hash *branchHash(bool isGold){
    uint16_t *moves = calloc(4, sizeof(uint16_t));
    struct Hash *moveHash = createHash(51001);
    //large enough to all the unique moves
    generateMoves(isGold, moves, 4, moveHash); //initial recursive call
    //printf("number of elements in hash: %d\n", moveHash->numOfElements);
    //getchar();
    //printTable(moveHash);
    free(moves);
    return moveHash;
}

