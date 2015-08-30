CC=gcc
CFLAGS=-std=c99 -g3 -Wall -pedantic

EXECUTABLES=graphTest searchTest maze outFile

all: $(EXECUTABLES)

test: all
	valgrind -q --leak-check=yes ./graphTest
	valgrind -q --leak-check=yes ./searchTest

graphTest: graphTest.o graph.o
	$(CC) $(CFLAGS) -o $@ $^

searchTest: searchTest.o search.o graph.o
	$(CC) $(CFLAGS) -o $@ $^

graphTest.o: graphTest.c graph.h

searchTest.o: searchTest.c search.h graph.h

graph.o: graph.c graph.h

search.o: search.c search.h

maze: maze.o search.o graph.o

maze.o: maze.c search.h graph.h

outFile: outFile.o search.o graph.o

outFile.o: outFile.c search.h graph.h

clean:
	$(RM) *.o $(EXECUTABLES) 