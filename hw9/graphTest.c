#include <stdio.h>
#include <assert.h>

#include "graph.h"

#define TEST_SIZE (37)

/* never call this */
static void
matchSink(Graph g, int source, int sink, void *data)
{
    assert(data && sink == *((int *) data));
}

int
main(int argc, char **argv)
{
    Graph g;
    int i;
    int j;

    g = graphCreate(TEST_SIZE);

    assert(graphVertexCount(g) == TEST_SIZE);

    /* check it's empty */
    for(i = 0; i < TEST_SIZE; i++) {
        for(j = 0; j < TEST_SIZE; j++) {
            assert(graphHasEdge(g, i, j) == 0);
        }
    }

    /* check it's empty again */
    for(i = 0; i < TEST_SIZE; i++) {
        assert(graphOutDegree(g, i) == 0);
        graphForeach(g, i, matchSink, 0);
    }

    /* check edge count */
    assert(graphEdgeCount(g) == 0);

    /* fill in the diagonal */
    for(i = 0; i < TEST_SIZE; i++) {
        graphAddEdge(g, i, i);
    }

    /* check */
    assert(graphEdgeCount(g) == TEST_SIZE);

    for(i = 0; i < TEST_SIZE; i++) {
        for(j = 0; j < TEST_SIZE; j++) {
            assert(graphHasEdge(g, i, j) == (i == j));
        }
    }

    for(i = 0; i < TEST_SIZE; i++) {
        assert(graphOutDegree(g, i) == 1);
        graphForeach(g, i, matchSink, &i);
    }


    /* fill in all the entries */
    for(i = 0; i < TEST_SIZE; i++) {
        for(j = 0; j < TEST_SIZE; j++) {
            if(i != j) graphAddEdge(g, i, j);
        }
    }

    /* test they are all there */
    assert(graphEdgeCount(g) == TEST_SIZE * TEST_SIZE);

    for(i = 0; i < TEST_SIZE; i++) {
        assert(graphOutDegree(g, i) == TEST_SIZE);
        for(j = 0; j < TEST_SIZE; j++) {
            assert(graphHasEdge(g, i, j) == 1);
        }
    }

    graphPrint(g);

    /* free it */
    graphDestroy(g);

    return 0;
}