#include <stdio.h>
#include <assert.h>

#include "graph.h"
#include "search.h"

#define TEST_SIZE (10)

int
main(int argc, char **argv)
{
    Graph g;
    struct searchInfo *s;
    int i;

    g = graphCreate(TEST_SIZE);

    /* put all nodes on a long path */
    /* and all nodes as children of 0 */
    /* except for node n-1 */
    for(i = 0; i < TEST_SIZE - 2; i++) {
        graphAddEdge(g, i, i);        /* red herring */
        graphAddEdge(g, i, i+1);      /* for DFS */
    };

    /* quick check to make sure we put in the edges we thought we did */
    assert(graphVertexCount(g) == TEST_SIZE);
    assert(graphEdgeCount(g) == (TEST_SIZE-2)*2);
    graphAddEdge(g, 7, 5);
    graphAddEdge(g, 1, 0);
    /* do dfs starting from 0 */
    s = searchInfoCreate(g);
    dfs(s, 0);
    searchInfoPrint(s);
    /*int xthnode = s->reached-1;
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
    while (s->preorder[k] != cycleEnd){
        printf("%d ", s->preorder[k]);
        k++;
    }
    printf("%d and %d again\n", cycleEnd, s->preorder[cycleStartIndex]);*/

    /* what did we learn? */
    assert(s->reached == TEST_SIZE - 1);
    for(i = 0; i < TEST_SIZE - 1; i++) {
        if(i == 0) {
            assert(s->parent[i] == i);
        } else {
            assert(s->parent[i] == i-1);
        } 
        assert(s->preorder[i] == i);
        assert(s->time[i] == i);
        assert(s->depth[i] == i);
    }

    /* now do more dfs from n-1 */
    /* need to reset pointers. Thank god searchInfoDestroy doesn't kill the graph */
    searchInfoDestroy(s);
    s = searchInfoCreate(g);

    dfs(s, TEST_SIZE-1);

    searchInfoPrint(s);

    /*assert(s->reached == TEST_SIZE);
    for(i = 0; i < TEST_SIZE; i++) {
        if(i == 0 || i == TEST_SIZE - 1) {
            assert(s->parent[i] == i);
        } else {
            assert(s->parent[i] == i-1);
        } 
        assert(s->preorder[i] == i);
        assert(s->time[i] == i);
        if(i == TEST_SIZE - 1) {
            assert(s->depth[i] == 0);
        } else {
            assert(s->depth[i] == i);
        }
    }*/

    searchInfoDestroy(s);
    graphDestroy(g);

    /* now try bfs */
    g = graphCreate(TEST_SIZE);

    for(i = 1; i < TEST_SIZE - 1; i++) {
        graphAddEdge(g, i, i);
        graphAddEdge(g, 0, i);
    }

    graphAddEdge(g, 1, TEST_SIZE - 1);

    assert(graphEdgeCount(g) == 2*(TEST_SIZE - 2) + 1);

    s = searchInfoCreate(g);
    bfs(s, 0);

    /* what did we learn? */
    assert(s->reached == TEST_SIZE);
    for(i = 0; i < TEST_SIZE - 1; i++) {
        assert(s->parent[i] == 0);
        assert(s->preorder[s->time[i]] == i);
        if(i == 0) {
            assert(s->depth[i] == 0);
        } else {
            assert(s->depth[i] == 1);
        }
    }

    /* n-1 should be hit last */
    assert(s->parent[TEST_SIZE - 1] == 1);
    assert(s->preorder[TEST_SIZE - 1] == TEST_SIZE - 1);
    assert(s->time[TEST_SIZE - 1] == TEST_SIZE - 1);
    assert(s->depth[TEST_SIZE - 1] == 2);

    searchInfoPrint(s);

    searchInfoDestroy(s);
    graphDestroy(g);

    return 0;
}