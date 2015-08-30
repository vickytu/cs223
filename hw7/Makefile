CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: testStrategy

testStrategy: strategy.o testStrategy.o modifiedtree.o
	$(CC) $(CFLAGS) -o $@ $^

strategy.o: strategy.c modifiedtree.o

testStrategy.o: testStrategy.c strategy.o

modifiedtree.o: modifiedtree.c modifiedtree.h strategy.h

clean:
	$(RM) *.o

