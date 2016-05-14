CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

DominionRandomGenerator.o: rngs.o DominionRandomGenerator.h DominionRandomGenerator.c
	gcc -c DominionRandomGenerator.c -g $(CFLAGS)
all: playdom player

tests: unittest1.o unittest2.o unittest3.o unittest4.o cardtest1.o cardtest2.o cardtest3.o cardtest4.o randomtestcard1.o randomtestcard2.o randomtestadventurer.o

unittest1.o: dominion.o unittest1.c
	gcc -o unittest1 unittest1.c -g dominion.o rngs.o $(CFLAGS)

unittest2.o: dominion.o unittest2.c
	gcc -o unittest2 unittest2.c -g dominion.o rngs.o $(CFLAGS)

unittest3.o: dominion.o unittest3.c
	gcc -o unittest3 unittest3.c -g dominion.o rngs.o $(CFLAGS)

unittest4.o: dominion.o unittest4.c
	gcc -o unittest4 unittest4.c -g dominion.o rngs.o $(CFLAGS)

cardtest1.o: dominion.o cardtest1.c
	gcc -o cardtest1 cardtest1.c -g dominion.o rngs.o $(CFLAGS)

cardtest2.o: dominion.o cardtest2.c
	gcc -o cardtest2 cardtest2.c -g dominion.o rngs.o $(CFLAGS)

cardtest3.o: dominion.o cardtest3.c
	gcc -o cardtest3 cardtest3.c -g dominion.o rngs.o $(CFLAGS)

cardtest4.o: dominion.o cardtest4.c
	gcc -o cardtest4 cardtest4.c -g dominion.o rngs.o $(CFLAGS)

randomtestcard1.o: dominion.o rngs.o DominionRandomGenerator.o randomtestcard1.c
	gcc -o randomtestcard1 randomtestcard1.c -g dominion.o rngs.o DominionRandomGenerator.o $(CFLAGS)

randomtestcard2.o: dominion.o rngs.o DominionRandomGenerator.o randomtestcard2.c
	gcc -o randomtestcard2 randomtestcard2.c -g dominion.o rngs.o DominionRandomGenerator.o $(CFLAGS)
	
randomtestadventurer.o: dominion.o rngs.o DominionRandomGenerator.o randomtestadventurer.c
	gcc -o randomtestadventurer randomtestadventurer.c -g dominion.o rngs.o DominionRandomGenerator.o $(CFLAGS)

runtests: tests
	rm -f unittestresults.out
	for i in 1 2 3 4; do \
		./unittest$${i} >> unittestresults.out ; \
		gcov dominion.c >> unittestresults.out ; done
	for i in 1 2 3 4; do \
		./cardtest$${i} >> unittestresults.out ; \
		gcov dominion.c >> unittestresults.out ; done

runrandom: randomtestcard1.o randomtestcard2.o randomtestadventurer.o
	rm -f randomtestcard1.out randomtestcard2.out randomtestadventurer.out
	./randomtestcard1 42 >> randomtestcard1.out
	gcov dominion.c >> randomtestcard1.out
	sed -n '651, +10p' dominion.c.gcov >> randomtestcard1.out
	./randomtestcard2 42 >> randomtestcard2.out
	gcov dominion.c >> randomtestcard2.out
	sed -n '961, 1010p' dominion.c.gcov >> randomtestcard2.out
	./randomtestadventurer 42 >> randomtestadventurer.out
	gcov dominion.c  >> randomtestadventurer.out
	sed -n '760, +20p' dominion.c.gcov >> randomtestadventurer.out

clean:
	rm -f *.o playdom.exe playdom test.exe test player unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 randomtestcard1 randomtestcard2 player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so *.a *.dSYM
