#include "dominion.h"
#include "stdio.h"
static int failed = 0;
void myassert(int bool_){
    if(!bool_){
        printf("ASSERT FAILED\n");
        failed = 1;
    }
}
/* We're gonna test scoreFor() */
int main(){
    struct gameState game;
    int numplayers = 4;

    //set up the game
    game.numPlayers = numplayers;

    game.handCount[0] = 2;
    game.deckCount[0] = 3;
    game.discardCount[0] = 2;
    game.hand[0][0] = estate;
    game.hand[0][1] = duchy;
    game.deck[0][0] = province;
    game.deck[0][1] = smithy;
    game.deck[0][2] = estate;
    game.discard[0][0] = curse;
    game.discard[0][1] = gold;
    //total should be 10
    //check that the function gets the correct stuff
    myassert(scoreFor(0, &game) == 10);

    if (!failed){
        printf("scoreFor() test passed successfully!\n");
    }else{
        printf("scoreFor() test FAILED\n");
    }
    return 0;
}
