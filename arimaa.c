#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "algo.h"

#define GOLD true
#define SILVER false

int main(int argc, char *argv[]){
    
    updateBoard("Ra1 Ra2 Rb1 Rc1 Rf1 Rg1 Rh1 Rh2 Cc2 Cf2 De1 Dd1 Hb2 Hg2 Me2 Ed2");
    updateBoard("ra8 rb8 rc8 rd8 re8 rf8 rg8 rh8 cc7 cf7 da7 dh7 hb7 hg7 md7 ee7");
    while (true){
        randomAgent(GOLD);        
        //printBoard();
        printf("***********************************************************\n");
        if (gameOver(GOLD))
            break;
        randomAgent(SILVER);        
        //printBoard();
        printf("***********************************************************\n");
        if (gameOver(SILVER))
            break;
    }
    return 0;
}
