#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "algo.h"

#define GOLD true
#define SILVER false

int main(int argc, char *argv[]){
    //for (int game = 1; game < 101; game++){ //used for stress testing
    uint64_t boardCheck;
    makeBoard(NULL); //sets global values to zero
    
    //first move for both players are currently static
    printf("1g ");
    updateBoard("Ra1 Ra2 Rb1 Rc1 Rf1 Rg1 Rh1 Rh2 Cc2 Cf2 De1 Dd1 Hb2 Hg2 Me2 Ed2");
    printf("1s ");
    updateBoard("ra8 rb8 rc8 rd8 re8 rf8 rg8 rh8 cc7 cf7 da7 dh7 hb7 hg7 md7 ee7");
    
    //game loop pitting AB as gold and random as silver
    int turnNumber = 2;
    while (true){

        //gold        
        printf("%dg: ", turnNumber);
        monteCarloTS(GOLD, 3600);
        printBoard();
        if (gameOver(GOLD))
            break;
        boardCheck = 0L;
        for(int i = 0; i < 6; i++){
            boardCheck &= ARIMAABOARD.gold[i];
            boardCheck &= ARIMAABOARD.silver[i];  
        }
        //getchar();

        //silver
        printf("%ds: ", turnNumber);
        randomAgent(SILVER);        
        printBoard();
        if (gameOver(SILVER))
            break;
        boardCheck = 0L;
        for(int i = 0; i < 6; i++){
            boardCheck &= ARIMAABOARD.gold[i];
            boardCheck &= ARIMAABOARD.silver[i];  
        }
        boardCheck &= ARIMAABOARD.empty;
        if(boardCheck != 0L){
            printf("board corrupted by silver\n");
            return 1;
        }
        //getchar();

        turnNumber++;
    }
    //}
    return 0;
}
