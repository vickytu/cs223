#include <stdio.h>
#include <assert.h>

#include "graph.h"
#include "search.h"


int
main(int argc, char **argv)
{
	int row;
	int col;
	int c;
	scanf("P5 %d %d 255\n", &col, &row);
	char array[row][col];
	for(int i=0;i<row;i++){
   		for(int j=0;j<col; j++){
 			c = getchar();
 			array[i][j] = c;
 		}
 	}
 	for(int i=0;i<row;i++){
   		for(int j=0;j<col; j++){
			printf("%d", array[i][j]);
		}
		printf("\n");
	}
}
