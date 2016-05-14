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
    int seed, i, j, p, choice;
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
        GenerateRandomGame(&game, minion);
        //make a copy of it
        memcpy(&old, &game, sizeof(struct gameState));
        //select choice
        choice = floor(Random() * 2) + 1;
        //run card effect
        cardEffect(minion, choice, 0, 0, &game, 0, &seed);
        
        p = old.whoseTurn;

        myassert(game.numActions == old.numActions + 1, "NumActions not increased");

        if (choice == 1){ //gain 2 gold
            //make sure no one else changed
            myassert(checkOthersUnaltered(&old, &game) == 1, "Others were altered with choice 1");
            myassert(game.coins == old.coins + 2, "Didn't gain 2 coins");
        } else {
            //We should have 4 cards if possible
            if(old.deckCount[p] >= 4){
                myassert(game.handCount[p] == 4, "Don't have 4 cards");
                myassert(game.deckCount[p] == old.deckCount[p] - 4, "Deck count didn't decrease by 4");
            } else if (old.deckCount[p] + old.discardCount[p] >= 4){
                myassert(game.handCount[p] == 4, "Don't have 4 cards");
                myassert(game.deckCount[p] == old.deckCount[p] + old.discardCount[p] - 4, NULL);
                myassert(game.discardCount[p] == 0, "Discard count not 0-");
            }else{
                myassert(game.handCount[p] == old.deckCount[p] + old.discardCount[p], "Didn't draw all cards when less than 4");
                myassert(game.deckCount[p] == 0, "Deck count not 0");
                myassert(game.discardCount[p] == 0, "Discard count not 0");
            }
            
            //loop through other players
            for(j = 0; j < game.numPlayers; j++){
                if (j != p){
                    if (old.handCount[j] < 5)
                        myassert(checkUnalteredPlayer(&old, &game, j) == 1, "Player altered when less than 5 cards");
                    else{
                        if(old.deckCount[j] >= 4){
                            myassert(game.handCount[j] == 4, "Others don't have 4 cards");
                            myassert(game.deckCount[j] == old.deckCount[j] - 4, NULL);
                        } else if (old.deckCount[j] + old.discardCount[j] >= 4){
                            myassert(game.handCount[j] == 4, "Others don't have 4 cards");
                            myassert(game.deckCount[j] == old.deckCount[j] + old.discardCount[j] - 4, NULL);
                            myassert(game.discardCount[j] == 0, "Others discard count not 0");
                        }else{
                            myassert(game.handCount[j] == old.deckCount[j] + old.discardCount[j], "Others Didn't draw all cards when less than 4");
                            myassert(game.deckCount[j] == 0, "Others Deck count not 0");
                            myassert(game.discardCount[j] == 0, "Others Discard count not 0");
                        }
                    }
                }
            }
        } //end choice 2
    }

   if (!failed){
       printf("Random Minion test passed successfully!\n");
   }else{
       printf("Random Minion test FAILED\n");
   }
   return 0;

}
