#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "graph.h"
#include "search.h"
/*#include "cycle.h"*/

int
main(int argc, char **argv)
{
  int row;
  int col;
  int c;
  Graph g;
  struct searchInfo *s;
  int startingnode;
  /*int cycleStart;
  int cycleEnd;*/
  int a;
  int b;
  int currnode;
  int *cycle;
  scanf("P5 %d %d 255\n", &col, &row);
  char array[row][col];
  /* builds array */
  for(int i=0;i<row;i++){
      for(int j=0;j<col; j++){
      c = getchar();
      array[i][j] = c;
      if(array[i][j] == 0){
        startingnode++;
      }
    }
  }

  /*for(int i=0;i<row;i++){
      for(int j=0;j<col; j++){
      printf("%d", array[i][j]);
    }
    printf("\n");
  }*/
  
  /* turn the array into a graph */
  g = graphCreate(row * col);
  for(int i=0;i<row;i++){
      for(int j=0;j<col; j++){
        if (array[i][j]){
          /*if (i-1>=0 && j-1>=0 && array[i-1][j-1]){
            graphAddEdge(g, i*col+j, (i-1)*col+j-1);
          }
          if (i-1>=0 && j>=0 && array[i-1][j]){
            graphAddEdge(g, i*col+j, (i-1)*col+j);
          }
          if (i-1>=0 && j+1<col && array[i-1][j+1]){
            graphAddEdge(g, i*col+j, (i-1)*col+j+1);
          }
          if (i>=0 && j-1>=0 && array[i][j-1]){
            graphAddEdge(g, i*col+j, (i)*col+j-1);
          }
          if (i>=0 && j+1<col && array[i][j+1]){
            graphAddEdge(g, i*col+j, (i)*col+j+1);
          }
          if (i+1<row && j-1>=0 && array[i+1][j-1]){
            graphAddEdge(g, i*col+j, (i+1)*col+j-1);
          }
          if (i+1<row && j>=0 && array[i+1][j]){
            graphAddEdge(g, i*col+j, (i+1)*col+j);
          }
          if (i+1<row && j+1<col && array[i+1][j+1]){
            graphAddEdge(g, i*col+j, (i+1)*col+j+1);
          }*/
          if (i-1>=0 && j-1>=0){
            if(array[i-1][j-1]){
            graphAddEdge(g, i*col+j, (i-1)*col+j-1);
            }
          }
          if (i-1>=0 && j>=0){
            if (array[i-1][j]){
            graphAddEdge(g, i*col+j, (i-1)*col+j);
            }
          }
          if (i-1>=0 && j+1<col){
            if (array[i-1][j+1]){
            graphAddEdge(g, i*col+j, (i-1)*col+j+1);
            }
          }
          if (i>=0 && j-1>=0){
            if (array[i][j-1]){
            graphAddEdge(g, i*col+j, (i)*col+j-1);
          }
          }
          if (i>=0 && j+1<col){
            if(array[i][j+1]){
            graphAddEdge(g, i*col+j, (i)*col+j+1);
            }
          }
          if (i+1<row && j-1>=0){
            if(array[i+1][j-1]){
            graphAddEdge(g, i*col+j, (i+1)*col+j-1);
          }
          }
          if (i+1<row && j>=0){
            if (array[i+1][j]){
            graphAddEdge(g, i*col+j, (i+1)*col+j);
          }
          }
          if (i+1<row && j+1<col){
            if(array[i+1][j+1]){
            graphAddEdge(g, i*col+j, (i+1)*col+j+1);
          }
          }
        }
      }
    }
  

    /* graphPrint(g); */

    /* decide which node to start dfs on, since we do not want to start it at a wall */ 
    startingnode = 0;
    for(int i=0;i<row;i++){
      for(int j=0;j<col; j++){
      if(array[i][j] == 0){
        startingnode++;
      }else{
        goto out;
      }
    }
  }
  out:

    /* dfs */
    s = searchInfoCreate(g);
    /*dfs(s, startingnode);
    xthnode = s->reached-1;*/
    /* printf("%d", xthnode); */
    /*searchInfoPrint(s); */
    /* ignoring trivial cycles, find a node that has an edge back to some other node before it on the tree */
    /*while (s->preorder[xthnode] != 0){
        for(int j = 0; j<xthnode; j++){
            if (s->preorder[j] != s->parent[s->preorder[xthnode]] && graphHasEdge(g, s->preorder[xthnode], s->preorder[j])){
                printf("cycle found from vertex %d to vertex %d\n", s->preorder[xthnode], s->preorder[j]);
                cycleStart = s->preorder[j];
                cycleEnd = s->preorder[xthnode];
                break;
            }
        }
        xthnode--;
    }*/

    cycle = dfsModified(s, startingnode);
    /*searchInfoPrint(s);*/
    /*cycleStart = cycle[1];
    cycleEnd = cycle[0];*/
    /* printf("The cycle goes from ");*/
    /*printf("%d and %d", cycleEnd, cycleStart);*/

    /* finds cycle by chasing parents and changes their array rep to 2 */
    currnode = cycle[0];
    while (s->parent[currnode] != s->parent[cycle[1]]){
        currnode = s->parent[currnode];
        /* printf("%d ", currnode);*/
        a = currnode % col;
        b = currnode - a * col;
        array[a][b] = 2;
    }

    a = cycle[0] % col;
    b = cycle[0] - a * col;
    array[a][b] = 2;
    /*printf("%d again\n", cycleEnd);*/

    /* print modified array */
    printf("P5 %d %d 255\n", col, row);
    for(int i=0;i<row;i++){
      for(int j=0;j<col; j++){
      printf("%c", array[i][j]);
      /* putchar(array[i][j]);*/
    }
     /*printf("\n");*/
  }

   /*printf("\n");*/
    /*printArray(array, row, col);

    g = arraytoGraph(array, row, col);

    cycleFinder (g, row, col, array);

    printArray(array, row, col);*/
    free(cycle);
    searchInfoDestroy(s);
    graphDestroy(g);
}
