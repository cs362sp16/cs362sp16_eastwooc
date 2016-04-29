#include "dominion.h"
#include "stdio.h"
static int failed = 0;
void myassert(int bool_){
    if(!bool_){
        printf("ASSERT FAILED\n");
        failed = 1;
    }
}
/* We're gonna test buyCard() */
int main(){
    struct gameState game;
    int numplayers = 4;
    int i = 0;

    //set up the game
    game.numPlayers = numplayers;

    //give us some cards to play with
    game.whoseTurn = 0;
    game.phase = 0;
    game.handCount[0] = 1;
    game.discardCount[0] = 0;
    game.deckCount[0] = 0;
    game.hand[0][0] = smithy;
    game.numBuys = 2;
    game.numActions = 1;
    game.coins = 0;
    for(i = 0; i <= treasure_map; i++)
        if (i != smithy)
            game.supplyCount[i] = 10; //every card
        else
            game.supplyCount[i] = 0; //no smithys left

    myassert(buyCard(silver, &game) == -1);  //shouldn't be able to buy

    //now give us some gold to buy with
    game.hand[0][0] = gold;
    game.coins = 3;
    myassert(buyCard(smithy, &game) == -1); //no smithys left
    myassert(buyCard(village, &game) == 0);
    myassert(game.discardCount[0] == 1); //should be one more card in our discard
    myassert(game.supplyCount[village] == 9); //one less in supply
    myassert(game.coins == 0); //no more coins
    myassert(game.numBuys == 1); //1 less buy


    if (!failed){
        printf("buyCard() test passed successfully!\n");
    }else{
        printf("buyCard() test FAILED\n");
    }
    return 0;
}
