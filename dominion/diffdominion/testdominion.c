#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dominion.h"
#include "rngs.h"

#define TRUE 1
#define FALSE 0


void printCardCounts(struct gameState* g) {
	printf("Supply Piles:\n");
	int i;
	for (i = curse; i <= treasure_map; i++) {
		printf("%d\n", g->supplyCount[i]);
	}

	printf("Hand Counts:\n");
	for (i = 0; i < g->numPlayers; i++) {
		printf("Player %d: %d\n", i, g->handCount[i]);
	}
	printf("Deck Counts:\n");
	for (i = 0; i < g->numPlayers; i++) {
		printf("Player %d: %d\n", i, g->deckCount[i]);
	}
	printf("Discard Counts:\n");
	for (i = 0; i < g->numPlayers; i++) {
		printf("Player %d: %d\n", i, g->discardCount[i]);
	}
}

int weightedRandom(int out_low, int out_high, int in_low, int in_high, float weight) {
	float rand = Random();
	int ret;
	if (rand < weight) {
		ret = floor(Random() * (in_high - in_low + 1)) + in_low;
	}
	else {
		ret = floor(Random() * (out_high - out_low + 1)) + out_low;
	}
	return ret;
}

int main(int argc, char** argv) {
	int i, j, card, unique, seed, players;
	int choice1, choice2, choice3;
	//int playedCard; //debugging, useful for setting break points
	int kingdomCards[10];
	int played, bought;
	float rand;
	struct gameState g;
	struct gameState *p = &g;

	if (argc != 2) {
		printf("You must provide a random seed\n");
		return -1;
	}

	seed = atoi(argv[1]);

	SelectStream(2);
	PutSeed(seed);

	//pick random number of players
	players = Random();
	players = floor(Random() * 3) + 2;

	//pick random kingdom cards
	for (i = 0; i < 10; i++)
		kingdomCards[i] = -1;
	i = 0;
	while (i < 10) {
		unique = TRUE;
		card = floor(Random() * (treasure_map - adventurer + 1)) + adventurer;
		for (j = 0; j < i; j++) {
			if (kingdomCards[j] == card) {
				unique = FALSE;
			}
		}
		if (unique == TRUE) {
			kingdomCards[i] = card;
			i++;
		}
	}

	//initialize game
	if (initializeGame(players, kingdomCards, seed, p) != 0) {
		printf("INITIALIZATION FAILED\n");
		return -1;
	}

	while (!isGameOver(p)) {
		//try to play a card (or cards)
		played = FALSE;
		//playedCard = -1;
		for (i = 0; i < numHandCards(p); i++) {
			if (handCard(i, p) >= adventurer && handCard(i, p) <= treasure_map) {
				for (j = 0; j < 5; j++) {
					choice1 = weightedRandom(-1, treasure_map + 1, 1, 3, 0.6);
					choice2 = weightedRandom(-1, treasure_map + 1, 0, numHandCards(p), 0.5);
					choice3 = weightedRandom(-1, treasure_map + 1, 0, numHandCards(p), 0.9);
					card = handCard(i, p);
					if (playCard(i, choice1, choice2, choice3, p) != -1) {
						//playedCard = card;
						printf("Player %d played card %d\n", whoseTurn(p), card);
						i = 0; //might have gained more cards
						played = TRUE;
						break;
					}
				}
			}
		}
		if (played == FALSE) {
			printf("Player %d didn't play a card\n", whoseTurn(p));
		}

		//try to buy cards
		bought = FALSE;
		for (i = 0; i < 5; i++) {
			rand = Random();
			if (rand < 0.6) {
				//try to buy a random valid kingdom card
				for (j = 0; j < 5; j++) {
					card = floor(Random() * 10);
					if (buyCard(kingdomCards[card], p) != -1) {
						printf("Player %d bought card %d\n", whoseTurn(p), kingdomCards[card]);
						bought = TRUE;
						break;
					}
				}			
			}
			else if (rand >= 0.6 && rand < 0.9) {
				//try to buy treasure
				card = gold;
				while (card >= copper){
					if (buyCard(card, p) != -1) {
						printf("Player %d bought card %d\n", whoseTurn(p), card);
						bought = TRUE;
						break;
					}
					card--;
				}
			}
			else if (rand >= 0.9 && rand < 0.98) {
				//try to buy victory point
				card = province;
				while (card >= estate) {
					if (buyCard(card, p) != -1) {
						printf("Player %d bought card %d\n", whoseTurn(p), card);
						bought = TRUE;
						break;
					}
					card--;
				}
			}
			else {
				//try to buy completely random card
				for (j = 0; j < 5; j++) {
					card = floor(Random() * (treasure_map + 2));
					if (buyCard(card, p) != -1) {
						printf("Player %d bought card %d\n", whoseTurn(p), card);
						bought = TRUE;
						break;
					}
				}
			}
		}// end of trying to buy cards
		if (bought == FALSE) {
			printf("Player %d didn't buy a card\n", whoseTurn(p));
		}
		endTurn(p);
	}

	for (i = 0; i < players; i++) {
		printf("Player %d had %d points\n", i, scoreFor(i, p));
	}
	int winners[MAX_PLAYERS];
	getWinners(winners, p);

	for (i = 0; i < MAX_PLAYERS; i++) {
		if (winners[i] == 1) {
			printf("Player %d won!\n", i);
		}
	}
	printCardCounts(p);
	return 0;
}