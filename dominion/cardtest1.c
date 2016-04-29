#include "dominion.h"
#include "stdio.h"
static int failed = 0;
void myassert(int bool_){
    if(!bool_){
        printf("ASSERT FAILED\n");
        failed = 1;
    }
}
/* We're gonna test smithy in cardEffect() */
int main(){
    struct gameState game;
    int numplayers = 4;

    //set up the game
    game.numPlayers = numplayers;
    game.whoseTurn = 0;
    game.phase = 0;
    game.numActions = 1;
    game.numBuys = 1;
    game.coins = 5; //gold and silver in hand
    game.playedCardCount = 0;

    game.handCount[0] = 5;
    game.deckCount[0] = 5;
    game.discardCount[0] = 2;
    game.hand[0][0] = estate;
    game.hand[0][1] = duchy;
    game.hand[0][2] = smithy;
    game.hand[0][3] = gold;
    game.hand[0][4] = silver;
    game.deck[0][0] = province;
    game.deck[0][1] = village;
    game.deck[0][2] = estate;
    game.discard[0][0] = curse;
    game.discard[0][1] = gold;

    myassert(playCard(2, 0, 0, 0, &game) == 0);
    myassert(game.handCount[0] == 7); //draw 3, discard 1

    if (!failed){
        printf("smithy test passed successfully!\n");
    }else{
        printf("smithy test FAILED\n");
    }
    return 0;
}
