#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dominion.h"
#include "rngs.h"

#define TRUE 1
#define FALSE 0

int compareFloat(const void* a, const void* b) {
	if (*(float*)a > *(float*)b)
		return 1;
	if (*(float*)a < *(float*)b)
		return -1;
	return 0;
}

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

float weightedFloat(float out_low, float out_high, float in_low, float in_high, float weight) {
	float rand = Random();
	float ret;
	if (rand < weight) {
		ret = Random() * (in_high - in_low) + in_low;
	}
	else {
		ret = Random() * (out_high - out_low) + out_low;
	}
	return ret;
}

void generateThresholds(float *thresholds, int numThresholds) {
	int i;
	float weights[3][2] = { {0.3, 0.5}, {0.6, 0.8}, {0.9, 0.98} };
	for (i = 0; i < numThresholds; i++)
		thresholds[i] = weightedFloat(0, 1, weights[i][0], weights[i][1], 0.8);
	qsort((void*)thresholds, numThresholds, sizeof(float), compareFloat);
	return;
}

void buyCards(struct gameState *p, float *thresholds, int* kingdomCards) {
	int bought, i, j, card;
	float rand;
	int ret;
	//try to buy cards
	bought = FALSE;
	for (i = 0; i < 10; i++) {
		rand = Random();
		if (rand < thresholds[0]) {
			//try to buy a random valid kingdom card
			for (j = 0; j < 5; j++) {
				card = floor(Random() * 10);
				ret = buyCard(kingdomCards[card], p);
				if (ret == 0) {
					printf("Player %d bought card %d\n", whoseTurn(p), kingdomCards[card]);
					bought = TRUE;
					break;
				}
				else if (ret != -1)
					printf("BAD RETURN VALUE\n");
			}
		}
		else if (rand >= thresholds[0] && rand < thresholds[1]) {
			//try to buy treasure
			card = gold;
			while (card >= copper) {
				ret = buyCard(kingdomCards[card], p);
				if (ret == 0) {
					printf("Player %d bought card %d\n", whoseTurn(p), card);
					bought = TRUE;
					break;
				}
				else if (ret != -1)
					printf("BAD RETURN VALUE\n");
				card--;
			}
		}
		else if (rand >= thresholds[1] && rand < thresholds[2]) {
			//try to buy victory point
			card = province;
			while (card >= estate) {
				ret = buyCard(kingdomCards[card], p);
				if (ret == 0) {
					printf("Player %d bought card %d\n", whoseTurn(p), card);
					bought = TRUE;
					break;
				}
				else if (ret != -1)
					printf("BAD RETURN VALUE\n");
				card--;
			}
		}
		else {
			//try to buy completely random card
			for (j = 0; j < 5; j++) {
				card = floor(Random() * (treasure_map + 4)) - 1;
				ret = buyCard(kingdomCards[card], p);
				if (ret == 0) {
					printf("Player %d bought card %d\n", whoseTurn(p), card);
					bought = TRUE;
					break;
				}
				else if (ret != -1)
					printf("BAD RETURN VALUE\n");
			}
		}
	}// end of trying to buy cards
	if (bought == FALSE) {
		printf("Player %d didn't buy a card\n", whoseTurn(p));
	}
}

int playCards(struct gameState *p) {
	int played, i, j, choice1, choice2, choice3, card, ret;
	int cardPlayed = -1;
	int numplayed = 0;
	//try to play a card (or cards)
	played = FALSE;
	for (i = 0; i < numHandCards(p) + 2; i++) {
		for (j = 0; j < 5; j++) {
			choice1 = weightedRandom(-1, treasure_map + 1, 1, 3, 0.5);
			choice2 = weightedRandom(-1, treasure_map + 1, 0, numHandCards(p), 0.5);
			choice3 = weightedRandom(-1, treasure_map + 1, 0, numHandCards(p), 0.9);
			card = handCard(i, p);
			ret = playCard(i, choice1, choice2, choice3, p);
			if ( ret == 0) {				
				printf("Player %d played card %d\n", whoseTurn(p), card);
				i = 0; //might have gained more cards
				played = TRUE;
				cardPlayed = card;
				numplayed++;
				break;
			}
			else if (ret != -1)
				printf("BAD RETURN VALUE\n");
		}
		if (numplayed > 20) //don't play more than 20 cards in a turn
			break;
	}
	if (played == FALSE) {
		printf("Player %d didn't play a card\n", whoseTurn(p));
	}
	return cardPlayed;
}

//I kept having problems with -1 cards appearing in the middle of the deck, so...

int assertShitCorrect(struct gameState *g) {
	int i, j, count;
	for (i = 0; i < g->numPlayers; i++){
		count = -1;
		do {
			count++;
			j = g->hand[i][count];
		} while (j > -1);
		if (count != g->handCount[i])
			return -1;
	}
	for (i = 0; i < g->numPlayers; i++) {
		count = -1;
		do {
			count++;
			j = g->deck[i][count];
		} while (j > -1);
		if (count != g->deckCount[i])
			return -1;
	}
	for (i = 0; i < g->numPlayers; i++) {
		count = -1;
		do {
			count++;
			j = g->discard[i][count];
		} while (j > -1);
		if (count != g->discardCount[i])
			return -1;
	}
	return 0;
}

int cardCount(struct gameState *g) {
	int who = whoseTurn(g);
	int ret = g->handCount[who] + g->deckCount[who] + g->discardCount[who];
	return ret;
}

int main(int argc, char** argv) {
	int i, j, card, loop, unique, seed, players, ret;
	float thresholds[3];
	int kCards[10];
	int *kCardsCheck;
	struct gameState *p = newGame();

	if (argc != 2) {
		printf("You must provide a random seed\n");
		return -1;
	}

	seed = atoi(argv[1]);
	//seed = 1;

	for (loop = 0; loop < 500; loop++) {
		seed += 1;
		//seed = 225;
		SelectStream(2);
		PutSeed(seed);

		//pick random number of players
		players = Random();
		players = weightedRandom(0, 5, 2, 4, 0.9);

		//generate buy thresholds
		generateThresholds(thresholds, 3);

		//pick random kingdom cards
		for (i = 0; i < 10; i++)
			kCards[i] = -1;
		i = 0;
		while (i < 10) {
			unique = TRUE;
			card = floor(Random() * (treasure_map - adventurer + 1)) + adventurer;
			for (j = 0; j < i; j++) {
				if (kCards[j] == card && Random() < 0.98) {
					unique = FALSE;
				}
			}
			if (unique == TRUE) {
				kCards[i] = card;
				i++;
			}
			//stupid just for the sake of coverage
			kCardsCheck = kingdomCards(kCards[0], kCards[1], kCards[2], kCards[3], kCards[4], kCards[5], kCards[6], kCards[7], kCards[8], kCards[9]);
			for (j = 0; j < 10; j++) {
				if (kCards[j] != kCardsCheck[j])
					printf("KINGDOM CARD INIT FAILED\n");
			}
			free(kCardsCheck);
		}

		//initialize game
		ret = initializeGame(players, kCards, seed, p);
		if (ret == -1) {
			printf("GAME INITIALIZATION FAILED\n");
			continue;
		}
		else if (ret != 0) {
			printf("GAME INIT BAD RETURN VALUE\n");
			continue;
		}
			
		ret = isGameOver(p);
		while (ret == FALSE) {
			cardCount(p);
			//occasionally buy cards first just to mess with the game
			if (Random() < 0.02) {
				buyCards(p, thresholds, kCards);
			}
			card = playCards(p);
			ret = assertShitCorrect(p);
			buyCards(p, thresholds, kCards);
			ret = assertShitCorrect(p);
			endTurn(p);
			ret = isGameOver(p);
			if (ret != 0 && ret != 1)
				printf("BAD ISGAMEOVER RETURN VALUE\n");
		}

		for (i = 0; i < players; i++) {
			printf("Player %d had %d points\n", i, scoreFor(i, p));
		}
		int winners[MAX_PLAYERS];
		if (getWinners(winners, p) != 0)
			printf("GET WINNERS BAD RETURN VALUE\n");

		for (i = 0; i < MAX_PLAYERS; i++) {
			if (winners[i] == 1) {
				printf("Player %d won!\n", i);
			}
		}
		printCardCounts(p);
	}
	free(p);
	return 0;
}