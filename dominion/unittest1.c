#include "dominion.h"
#include "stdio.h"
static int failed = 0;
void myassert(int bool_){
    if(!bool_){
        printf("ASSERT FAILED\n");
        failed = 1;
    }
}
/* We're gonna test whoseTurn() */
int main(){
   struct gameState game;

   //set up the game
   game.numPlayers = 4;
   game.whoseTurn = 2;
   myassert(whoseTurn(&game) == 2);
   if (!failed){
       printf("WhoseTurn() test passed successfully!\n");
   }else{
       printf("WhoseTurn() test FAILED\n");
   }
   return 0;
}
