#include "negaMax.h"

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

int negaMax(int depth, bool isGold, int A, int B, double tTime, time_t startTime){
    if (gameOver(isGold)){
        return INT_MAX;
    }
    if (depth == 0){ //endcase
        int h = heuristic(isGold);
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
    printf(" 00");
    for (int depth = 2; depth < 3; depth += 2){
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
                    }
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
    //make moves and prints them out
    for (int i = 0; i < 4; i++){
        updateBoardBit(*(maxMoves + i));
        updateTraps();
        printMove(*(maxMoves + i));
    }
    free(maxMoves);
    printf("\n");
    return 0;
}
