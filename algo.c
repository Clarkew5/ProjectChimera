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
        ((bitboard << 1) & ~lEdge) | // west
        (bitboard >> 8) | // North
        ((bitboard >> 1) & ~rEdge); // east
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

/*
uint64_t pushFull(int i, bool isGold){
    uint64_t weaker = 0;
    if (isGold)
        for (i = j-1; j < 0; j--)
            weaker |= ARIMAABOARD.gold[i];
    else
        for (i = j-1; j < 0; j--)
            weaker |= ARIEMAABOARD.silver[i];
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
        for (i = j-1; j < 0; j--)
            weaker |= ARIMAABOARD.gold[i];
    else
        for (i = j-1; j < 0; j--)
            weaker |= ARIEMAABOARD.silver[i];

    uint64_t notFrozen = 0L;    
    if (isGold)
         notFrozen = ARIMAABOARD.gold[i] & ~frozen(i, isGold);
    else
         notFrozen = ARIMAABOARD.silver[i] & ~frozen(i, isGold);

    uint64_t tempPullFull = near(notfrozen, i, isGold) & weaker;
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
*/

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
    uint64_t *bitboards = malloc(6 * sizeof(uint64_t)); 
    for (int p = 0; p < 6; p++){
        printf("%d\n", p);
        *(bitboards + p) = move(p, isGold);
        printBitboard(*(bitboards+p));
        printf("\n-------------------------\n");
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
    numOfMoves = numOfMovesNorth + numOfMovesEast + numOfMovesSouth + numOfMovesWest;
    printf("North: %d\n", numOfMovesNorth);
    printf("East: %d\n", numOfMovesEast);
    printf("South: %d\n", numOfMovesSouth);
    printf("West: %d\n", numOfMovesWest);
    return numOfMoves;
}

uint16_t randomMove();
