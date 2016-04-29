#include "dominion.h"
#include "stdio.h"
static int failed = 0;
void myassert(int bool_){
    if(!bool_){
        printf("ASSERT FAILED\n");
        failed = 1;
    }
}
/* We're gonna test salvager in cardEffect() */
int main(){
    struct gameState game;
    int numplayers = 4;
    int choice = 1;

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
    game.hand[0][2] = salvager;
    game.hand[0][3] = gold;
    game.hand[0][4] = silver;
    game.deck[0][0] = province;
    game.deck[0][1] = smithy;
    game.deck[0][2] = estate;
    game.discard[0][0] = curse;
    game.discard[0][1] = gold;

    myassert(playCard(2, choice, 0, 0, &game) == 0);
    myassert(game.handCount[0] == 3); //discard 1, trash 1
    myassert(game.coins == 10); //got more money
    myassert(game.numBuys == 2); //got another buy too
    myassert(game.phase == 0); //still in action phase

    if (!failed){
        printf("salvager test passed successfully!\n");
    }else{
        printf("salvager test FAILED\n");
    }
    return 0;
}
