CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: turing

test: turing
	./turing b+2a-0 a-1a-1
	./turing b+2a-0c-3 b-3c+2b-2 b-1a+2c-1

turing: turing.o 
	$(CC) $(CFLAGS) -o $@ $^

turing.o: turing.c

clean:
	$(RM) turing.o