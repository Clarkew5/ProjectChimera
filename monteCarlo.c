#include "monteCarlo.h"

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

bool makeRandomMoves(bool isGold, int movesLeft){
    //recursivaly makes random moves to play a random game.
    bool goldWon;
    if (!gameOver(isGold)){
        if (movesLeft == 0){
            goldWon = makeRandomMoves(!isGold, 4);
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
        uint16_t *moves = generateMovesFromBoard(bitboards, isGold, movesLeft);
        free(bitboards);


        int randomIndex = (rand() % size);
        if (randomIndex >= 0 && randomIndex < numOfMoves){
            uint16_t move = *(moves + randomIndex);
            free(moves);

            updateBoardBit(move);
            uint16_t updatedTraps = updateTraps();

            goldWon = makeRandomMoves(isGold, movesLeft-1);

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

            updateBoardBit(move);
            updateBoardBit(move2);
            uint16_t updatedTraps = updateTraps();
            uint16_t updatedTraps2 = updateTraps();

            goldWon = makeRandomMoves(isGold, movesLeft-2);

            undoTraps(updatedTraps2);
            undoTraps(updatedTraps);
            undoMove(move2);
            undoMove(move);
        }
        else if (randomIndex >= (numOfMoves + numOfPushes) && randomIndex < size){
            if ((randomIndex - numOfMoves) % 2 == 1)
                randomIndex--;

            uint16_t move = *(moves + randomIndex);
            uint16_t move2 = *(moves + randomIndex + 1);
            free(moves);

            updateBoardBit(move);
            updateBoardBit(move2);
            uint16_t updatedTraps = updateTraps();
            uint16_t updatedTraps2 = updateTraps();

            goldWon = makeRandomMoves(isGold, movesLeft-2);

            undoTraps(updatedTraps2);
            undoTraps(updatedTraps);
            undoMove(move2);
            undoMove(move);
        }
        return goldWon;
    }
    return goldWin(isGold);
}

int monteCarloTS(bool isGold, double tTime){
    srand((unsigned) time(NULL));
    struct Hash *branches = branchHash(isGold);
    uint16_t **moves = malloc(branches->numOfElements * sizeof(uint16_t*));
    double *gamesWon = calloc(branches->numOfElements, sizeof(double));
    double *gamesPlayed =  calloc(branches->numOfElements, sizeof(double));
    //copies moves from the branch hash into an array
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

    int numOfElements = branches->numOfElements;
    destroyHash(branches);
    //steps through each state and plays 10 games per state, playing then more 
    //if the win percentage is 90%
    double maxGamesWon = 0;
    int maxGamesWonIndex = 0;
    printf("000");
    for (int i = 0; i < numOfElements; i++){
        for (int game = 0; game < 10; game++){
            uint16_t updatedTraps[4];
            for (int j = 0; j < 4; j++){
                updateBoardBit(*(*(moves + i) + j));
                updatedTraps[j] = updateTraps();
            }
            //checks if the game is won.
            if (gameOver(isGold) && (goldWin(isGold) == isGold)){
                printf("\b\b\b");
                //printf("the game is mine ");
                for (int j = 0; j < 4; j++)
                    printMove(*(*(moves + i) + j));
                printf("\n");

                for(int i = 0; i < numOfElements; i++)
                    free(*(moves + i));
                free(moves);
                free(gamesWon);
                return 0;
            }

            bool goldWon = makeRandomMoves(!isGold, 4);

            (*(gamesPlayed + i))++; //updates games played

            for (int j = 0; j < 4; j++){
                undoTraps(updatedTraps[3 - j]);
                undoMove(*(*(moves + i) + (3 - j)));
            }

            if (goldWon == isGold)
                (*(gamesWon + i))++;
        }

        if ( (*(gamesWon + i)) / (*(gamesPlayed + i)) >= 0.9  && (*(gamesPlayed + i)) == 10){
            i--;
            continue;
        }

        if ((*(gamesWon + i))/(*(gamesPlayed + i)) >= maxGamesWon){
            if (rand() % 2){
                maxGamesWon = (*(gamesWon + i))/(*(gamesPlayed + i));
                maxGamesWonIndex = i;
            }
        }
        printf("\b\b\b%03.0f", (double)i/(double)numOfElements*100);
    }
    printf("\b\b\b\n");
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


