CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: testShips

testShips: ships.o testShips.o
	$(CC) $(CFLAGS) -o $@ $^

ships.o: ships.c ships.h 

testShips.o: testShips.c ships.h

clean:
	$(RM) ships *.o