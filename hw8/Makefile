CC=gcc
CFLAGS=-std=gnu99 -Wall -pedantic -g3

all: testOrderedSet

testOrderedSet: orderedSet.o testOrderedSet.o avlTree.o
	$(CC) $(CFLAGS) -D_GNU_SOURCE -o $@ $^

orderedSet.o: orderedSet.c avlTree.o

testOrderedSet.o: testOrderedSet.c orderedSet.o

avlTree.o: avlTree.c avlTree.h

clean:
	$(RM) *.o