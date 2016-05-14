#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
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
    int seed, i, j, p, decktreasures, discardtreasures, treasures;
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
        GenerateRandomGame(&game, adventurer);
        //make a copy of it
        memcpy(&old, &game, sizeof(struct gameState));
        
        //run card effect
        cardEffect(adventurer, 0, 0, 0, &game, 0, &seed);
        
        p = old.whoseTurn;
        
        myassert(checkOthersUnaltered(&old, &game) == 1, "Others were altered");
        
        decktreasures = 0;
        discardtreasures = 0;
        //let's count the number of treasures in deck and discard first
        for(j = 0; j < old.deckCount[p]; j++)
            if (old.deck[p][j] >= copper && old.deck[p][j] <= gold)
                decktreasures++;
        for(j = 0; j < old.discardCount[p]; j++)
            if (old.discard[p][j] >= copper && old.discard[p][j] <= gold)
                discardtreasures++;
            
        treasures = decktreasures + discardtreasures;
        
        if (treasures >= 2){
            myassert(game.handCount[p] == old.handCount[p] + 1, "Wrong hand count 1");
        } else {
            myassert(game.handCount[p] == old.handCount[p] + treasures - 1, "Wrong hand count 2");
            //printf("Deck Treasures: %d\tDiscard Treasures: %d\n", decktreasures, discardtreasures);
            //printf("Old HC: %d\tNew HC: %d\n", old.handCount[p], game.handCount[p]);
        }
        
        if (decktreasures >= 2){
            //make sure the new discard has as many that the new deck is missing
            myassert(game.deckCount[p] < old.deckCount[p], "New deck is bigger");
            myassert(game.discardCount[p] >= old.discardCount[p], "New discard is smaller");
            myassert(game.deckCount[p] + game.discardCount[p] + 2 == old.deckCount[p] + old.discardCount[p], "Deck + discard don't add up");
        } else if (treasures >= 2){
            myassert(game.deckCount[p] + game.discardCount[p] + 2 == old.deckCount[p] + old.discardCount[p], "Deck + discard don't add up");
        } else {
            myassert(game.discardCount[p] + treasures == old.deckCount[p] + old.discardCount[p], NULL);
            //printf("New Discard: %d\n", game.discardCount[p]);
            //printf("Deck Treasures: %d\tDiscard Treasures: %d\n", decktreasures, discardtreasures);
            //printf("Old Deck: %d\tOld Discard: %d\n", old.deckCount[p], old.discardCount[p]);
            
        }
    }

   if (!failed){
       printf("Random Adventurer test passed successfully!\n");
   }else{
       printf("Random Adventurer test FAILED\n");
   }
   return 0;

}
