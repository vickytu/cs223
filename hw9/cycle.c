#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "graph.h"
#include "search.h"
#include "cycle.h"

/* tried to abstract to functions but always hit segfault */

/*void printArray(int **array, int row, int col){
	for(int i=0;i<row;i++){
   		for(int j=0;j<col; j++){
			printf("%d", array[i][j]);
		}
		printf("\n");
	}
}

Graph arraytoGraph(int **array, int row, int col){
	Graph g;
	g = graphCreate(row * col);
	for(int i=0;i<row;i++){
   		for(int j=0;j<col; j++){
   			if (array[i][j] == 1){
   				if (i-1>=0 && j-1>=0 && array[i-1][j-1] == 1){
   					graphAddEdge(g, i*col+j, (i-1)*col+j-1);
   				}
   				if (i-1>=0 && j>=0 && array[i-1][j]== 1){
   					graphAddEdge(g, i*col+j, (i-1)*col+j);
   				}
   				if (i-1>=0 && j+1<col && array[i-1][j+1]== 1){
   					graphAddEdge(g, i*col+j, (i-1)*col+j+1);
   				}
   				if (i>=0 && j-1>=0 && array[i][j-1]== 1){
   					graphAddEdge(g, i*col+j, (i)*col+j-1);
   				}
   				if (i>=0 && j+1<col && array[i][j+1]== 1){
   					graphAddEdge(g, i*col+j, (i)*col+j+1);
   				}
   				if (i+1<row && j-1>=0 && array[i+1][j-1]== 1){
   					graphAddEdge(g, i*col+j, (i+1)*col+j-1);
   				}
   				if (i+1<row && j>=0 && array[i+1][j]== 1){
   					graphAddEdge(g, i*col+j, (i+1)*col+j);
   				}
   				if (i+1<row && j+1<col && array[i+1][j+1]== 1){
   					graphAddEdge(g, i*col+j, (i+1)*col+j+1);
   				}
   			}
   		}
   	}
   	return g;
}

void cycleFinder (Graph g, int row, int col, int **array){
	struct searchInfo *s = searchInfoCreate(g);
	dfs(s, 0);
   	int xthnode = s->reached-1;
    int cycleStartIndex;
    int cycleEnd;
    
    while (s->preorder[xthnode] != 0){
        for(int j = 0; j<xthnode; j++){
            if (s->preorder[j] != s->parent[s->preorder[xthnode]] && graphHasEdge(g, s->preorder[xthnode], s->preorder[j])){
                printf("cycle found from vertex %d to vertex %d\n", s->preorder[xthnode], s->preorder[j]);
                cycleStartIndex = j;
                cycleEnd = s->preorder[xthnode];
                break;
            }
        }
        xthnode--;
    }
    printf("The cycle goes from ");
    int k = cycleStartIndex;
    int a;
    int b;
    while (s->preorder[k] != cycleEnd){
        printf("%d ", s->preorder[k]);
        a = s->preorder[k] % col;
        b = s->preorder[k] - a * col;
        array[a][b] = 2;
        k++;
    }

    a = cycleEnd % col;
    b = cycleEnd - a * col;
    array[a][b] = 2;
    printf("%d and %d again\n", cycleEnd, s->preorder[cycleStartIndex]);
}*/
