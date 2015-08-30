CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: compositor

test: compositor
	echo "1 1 ship 3 5 ship 3 19 tree 7 2 ship 13 4 ship 4 22 tree 5 6 cake" | ./compositor

compositor: compositor.o 
	$(CC) $(CFLAGS) -o $@ $^

compositor.o: compositor.c

clean:
	$(RM) compositor.o