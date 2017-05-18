#include "randomAgent.h"

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

        free(bitboards);
        if(gameOver(isGold))
            break;
    }
    printf("\n");
    return 0;
}
