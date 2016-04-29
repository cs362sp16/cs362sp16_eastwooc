#include "dominion.h"
#include "stdio.h"
static int failed = 0;
void myassert(int bool_){
    if(!bool_){
        printf("ASSERT FAILED\n");
        failed = 1;
    }
}
/* We're gonna test numHandCards() */
int main(){
    struct gameState game;
    int numplayers = 4;
    int cardnum[] = {5, 3, 4, 2};
    int i = 0;

    //set up the game
    game.numPlayers = numplayers;
    for(i = 0; i < numplayers; i++){
        game.handCount[i] = cardnum[i];
    }

    //check that the function gets the correct stuff
    for(i = 0; i < numplayers; i++){
        game.whoseTurn = i;
        myassert(numHandCards(&game) == cardnum[i]);
    }

    if (!failed){
        printf("numHandCards() test passed successfully!\n");
    }else{
        printf("numHandCards() test FAILED\n");
    }
    return 0;
}
