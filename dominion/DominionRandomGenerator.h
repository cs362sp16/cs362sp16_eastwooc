#ifndef _DOMINION_RANDOM_GENERATOR
#define _DOMINION_RANDOM_GENERATOR
#include "dominion.h"
/*
This function will generate a random valid-looking gamestate for testing @card
Set @card to -1 to not test any specific card
Setting @card to a specific card will guarantee that the card is one of the 10 kingdom
cards as well that the current player will have the card in their hand to be played

The random number generator should be set up before this function is called!
*/
int GenerateRandomGame(struct gameState *game, enum CARD card);


/* Returns 1 if player is in the same state in both games, else returns 0 */
int checkUnalteredPlayer(struct gameState *old, struct gameState *cur, int player);


/* Checks to make sure that the state of everyone besides the current player wasn't altered
Returns 1 if others weren't altered, 0 otherwise.
*/
int checkOthersUnaltered(struct gameState *old, struct gameState *cur);


#endif
