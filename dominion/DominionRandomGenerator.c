#include <math.h>
#include "dominion.h"
#include "rngs.h"
#include "DominionRandomGenerator.h"

/*
This function will generate a random valid-looking gamestate for testing @card
Set @card to -1 to not test any specific card
Setting @card to a specific card will guarantee that the card is one of the 10 kingdom 
cards as well that the current player will have the card in their hand to be played

The random number generator should be set up before this function is called!
 */
int GenerateRandomGame(struct gameState *game, enum CARD card) {
	int i, j, cur;
	double rand;
	int kingdomCards[10];

	//Setup state
	game->numPlayers = floor(Random() * 3) + 2;

	//setup treasure and victory cards
	for (i = curse; i <= gold; i++) {
		game->supplyCount[i] = floor(Random() * 20);
	}

	//set everything else to -1 for now
	for (i = adventurer; i <= treasure_map; i++)
		game->supplyCount[i] = -1;

	//setup kingdom cards
	if (card >= adventurer && card <= treasure_map) {
		game->supplyCount[card] = floor(Random() * 10);
		kingdomCards[9] = card;
		i = 9;
	}
	else {
		i = 10;
	}

	//pick random kingdom cards
	while (i > 0) {
		j = floor(Random() * (treasure_map - adventurer + 1)) + adventurer;
		if (game->supplyCount[j] == -1) {
			game->supplyCount[j] = floor(Random() * 10);
			i--;
			kingdomCards[i] = j;
		}
	}

	//set embargo tokens
	for (i = 0; i <= treasure_map; i++)
	{
		if (game->supplyCount[embargo] != -1) //only use embargo tokens if embargo is in play
			if (game->supplyCount[i] != -1)
				game->embargoTokens[i] = floor(pow(Random(), 4) * 3); //heavily weighted down
			else
				game->embargoTokens[i] = 0;
		else
			game->embargoTokens[i] = 0;
	}

	//generate random decks, hands, discards
	for (cur = 0; cur < game->numPlayers; cur++) {
		//set hand
		j = MAX_HAND;
		for (i = 0; i < 5; i++)
			j = floor(Random() * j);
		game->handCount[cur] = floor(Random() * j);
		for (i = 0; i < game->handCount[cur]; i++) {
			rand = Random();
			//25% chance of treasure card
			if (rand < 0.25) {
				game->hand[cur][i] = floor(Random() * 3) + copper;
			}
			else if (rand >= 0.25 && rand < 0.4) { //victory card
				game->hand[cur][i] = floor(Random() * 4) + curse;
			}
			else { //kingdom card
				j = floor(Random() * 10);
				game->hand[cur][i] = kingdomCards[j];
			}
			
		}

		//set deck
		j = MAX_DECK;
		for (i = 0; i < 4; i++)
			j = floor(Random() * j);
		game->deckCount[cur] = floor(Random() * j);
		for (i = 0; i < game->deckCount[cur]; i++) {
			rand = Random();
			//25% chance of treasure card
			if (rand < 0.25) {
				game->deck[cur][i] = floor(Random() * 3) + copper;
			}
			else if (rand >= 0.25 && rand < 0.4) { //victory card
				game->deck[cur][i] = floor(Random() * 4) + curse;
			}
			else { //kingdom card
				j = floor(Random() * 10);
				game->deck[cur][i] = kingdomCards[j];
			}
		}

		//set discard
		j = MAX_DECK;
		for (i = 0; i < 4; i++)
			j = floor(Random() * j);
		game->discardCount[cur] = floor(Random() * j);
		for (i = 0; i < game->discardCount[cur]; i++) {
			rand = Random();
			//25% chance of treasure card
			if (rand < 0.25) {
				game->discard[cur][i] = floor(Random() * 3) + copper;
			}
			else if (rand >= 0.25 && rand < 0.4) { //victory card
				game->discard[cur][i] = floor(Random() * 4) + curse;
			}
			else { //kingdom card
				j = floor(Random() * 10);
				game->discard[cur][i] = kingdomCards[j];
			}
		}
	}

    //set played cards
    j = MAX_DECK;
	for (i = 0; i < 5; i++)
		j = floor(Random() * j);
	game->playedCardCount = floor(Random() * j);

	for (i = 0; i < game->playedCardCount; i++) {
		rand = Random();
		//25% chance of treasure card
		if (rand < 0.4) {
			game->playedCards[i] = floor(Random() * 3) + copper;
		} else { //kingdom card
			j = floor(Random() * 10);
			game->playedCards[i] = kingdomCards[j];
		}
	}

	//set other stuff
	game->outpostPlayed = 0;
	game->outpostTurn = 0;
	game->phase = 0;
	game->numActions = floor(Random() * 5) + 1;
	game->numBuys = floor(Random() * 5) + 1;
	game->coins = floor(Random() * 10);
	game->whoseTurn = floor(Random() * game->numPlayers);

	//set the first card in current player's hand to @card
	if (card >= curse && card <= treasure_map) {
		if (game->handCount[game->whoseTurn] == 0)
			game->handCount[game->whoseTurn]++;
		game->hand[game->whoseTurn][0] = card;
	}
	
	return 0;
}

/* Returns 1 if player is in the same state in both games, else returns 0
*/
int checkUnalteredPlayer(struct gameState *old, struct gameState *cur, int player) {
	int i;
	if (cur->deckCount[player] != old->deckCount[player])
		return 0;
	if (cur->handCount[player] != old->handCount[player])
		return 0;
	if (cur->discardCount[player] != old->discardCount[player])
		return 0;
	for (i = 0; i < cur->deckCount[player]; i++) {
		if (cur->deck[player][i] != old->deck[player][i])
			return 0;
	}
	for (i = 0; i < cur->handCount[player]; i++) {
		if (cur->hand[player][i] != old->hand[player][i])
			return 0;
	}
	for (i = 0; i < cur->discardCount[player]; i++) {
		if (cur->discard[player][i] != old->discard[player][i])
			return 0;
	}
	return 1;
}

/* Checks to make sure that the state of everyone besides the current player wasn't altered
   Returns 1 if others weren't altered, 0 otherwise.
*/
int checkOthersUnaltered(struct gameState *old, struct gameState *cur) {
	int i;
	for (i = 0; i < old->numPlayers; i++) {
		if (i != old->whoseTurn)
			if (checkUnalteredPlayer(old, cur, i) == 0)
				return 0;
	}
	return 1;
}

