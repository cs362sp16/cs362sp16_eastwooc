#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DominionRandomGenerator.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

#define MAX_TESTS 100000

static int failed = 0;
void myassert(int bool_, char* message){
    if(!bool_){
        printf("ASSERT FAILED: %s\n", message);
        failed = 1;
    }
}

int main(int argc, char** argv){
    int seed, i, p;
    struct gameState game, old;
    if (argc != 2){
        printf("You must enter a random seed\n");
        return -1;
    }

    seed = atoi(argv[1]);
    SelectStream(1);
    PutSeed(seed);
    Random();

    for(i = 0; i < MAX_TESTS; i++){
        //generate a random game
        GenerateRandomGame(&game, smithy);
        //make a copy of it
        memcpy(&old, &game, sizeof(struct gameState));

        //run card effect
        cardEffect(smithy, 0, 0, 0, &game, 0, &seed);
        
        p = old.whoseTurn;
        //make sure no one else changed
        myassert(checkOthersUnaltered(&old, &game) == 1, "Others were altered");

        //check that we have two more cards in hand, if possible
        if(old.deckCount[p] + old.discardCount[p] >= 3)
            myassert(game.handCount[p] == old.handCount[p] + 2, "Don't have 2 more cards in hand");
        else
            myassert(game.handCount[p] == old.handCount[p] - 1 + old.deckCount[p] + old.discardCount[p], "Don't have more cards in hand");

        //if there were more than 3 cards in the deck before, make sure deck count went down
        if(old.deckCount[p] >= 3){
            myassert(game.deckCount[p] == old.deckCount[p] - 3, "Deck count not 3 less");
        } else if(old.deckCount[p] + old.discardCount[p] >= 3){
            myassert(game.deckCount[p] == old.deckCount[p] + old.discardCount[p] - 3, "Deck count not 3 less than old deck and discard");
        } else {
            myassert(game.deckCount[p] == 0, "Deck count not 0");
        }
        myassert(game.playedCardCount == old.playedCardCount + 1, "PCC not 1 more");
    }

   if (!failed){
       printf("Random Smithy test passed successfully!\n");
   }else{
       printf("Random Smithy test FAILED\n");
   }
   return 0;

}
