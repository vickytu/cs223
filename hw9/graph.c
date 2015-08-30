#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "graph.h"

/* basic directed graph type */
/* the implementation uses adjacency lists
 * represented as variable-length arrays */

/* these arrays may or may not be sorted: if one gets long enough
 * and you call graphHasEdge on its source, it will be */

struct graph {
    int n;              /* number of vertices */
    int m;              /* number of edges */
    struct successors {
        int d;          /* number of successors */
        int len;        /* number of slots in array */
        int isSorted; /* true = 1 if list is already sorted, false = 0 if not */
        int list[1];    /* actual list of successors */
    } *alist[1];        /* alist[1] is pointer to struct sucessors */
};

/* create a new graph with n vertices labeled 0..n-1 and no edges */
Graph
graphCreate(int n)
{
    Graph g;    /* pointer to struct graph */
    int i;

    g = malloc(sizeof(struct graph) + sizeof(struct successors *) * (n-1));
    assert(g);

    g->n = n;
    g->m = 0;

    for(i = 0; i < n; i++) {
        g->alist[i] = malloc(sizeof(struct successors));
        assert(g->alist[i]);

        g->alist[i]->d = 0;
        g->alist[i]->len = 1;
        g->alist[i]->isSorted= 1;
    }
    
    return g;
}

/* free all space used by graph */
void
graphDestroy(Graph g)
{
    int i;

    for(i = 0; i < g->n; i++){
        free(g->alist[i]);
    }

    free(g);
}

/* add an edge to an existing graph */
void
graphAddEdge(Graph g, int u, int v)
{
    /* assert complains if user trying to add edges between vertices that don't exist in the graph */ 
    assert(u >= 0);
    assert(u < g->n);
    assert(v >= 0);
    assert(v < g->n);

    /* do we need to grow the list? */
    while(g->alist[u]->d >= g->alist[u]->len) {
        g->alist[u]->len *= 2;
        g->alist[u] =
            realloc(g->alist[u], 
                sizeof(struct successors) + sizeof(int) * (g->alist[u]->len - 1));
    }

    /* now add the new sink */
    g->alist[u]->list[g->alist[u]->d++] = v;
    g->alist[u]->isSorted = 0;

    /* bump edge count */
    g->m++;
}

/* return the number of vertices in the graph */
int
graphVertexCount(Graph g)
{
    return g->n;
}

/* return the number of vertices in the graph */
int
graphEdgeCount(Graph g)
{
    return g->m;
}

/* return the out-degree of a vertex; how many edges LEAVE source */
int
graphOutDegree(Graph g, int source)
{
    assert(source >= 0);
    assert(source < g->n);

    return g->alist[source]->d;
}

/* when we are willing to call bsearch */
#define BSEARCH_THRESHOLD (10)

static int
intcmp(const void *a, const void *b)
{
    return *((const int *) a) - *((const int *) b);
}

/* return 1 if edge (source, sink) exists), 0 otherwise */
int
graphHasEdge(Graph g, int source, int sink)
{
    int i;

    assert(source >= 0);
    assert(source < g->n);
    assert(sink >= 0);
    assert(sink < g->n);

    /* brought time from 16sec to 12sec but still not fast enough*/
    /*if(graphOutDegree(g, source) >= BSEARCH_THRESHOLD) {*/
        /* make sure it is sorted */
        /*if(! g->alist[source]->isSorted) {
            qsort(g->alist[source]->list,
                    g->alist[source]->d,
                    sizeof(int),
                    intcmp);
        }*/
        
        /* call bsearch to do binary search for us */
        /*return 
            bsearch(&sink,
                    g->alist[source]->list,
                    g->alist[source]->d,
                    sizeof(int),
                    intcmp)
            != 0;
    } else {*/
        /* just do a simple linear search */
        /* we could call lfind for this, but why bother? */
        for(i = 0; i < g->alist[source]->d; i++) {
            if(g->alist[source]->list[i] == sink) return 1;
        }
        /* else */
        return 0;
   /* }*/
}

/* invoke f on all edges (u,v) with source u */
/* supplying data as final parameter to f */
void
graphForeach(Graph g, int source,
    void (*f)(Graph g, int source, int sink, void *data),
    void *data)
{
    int i;

    assert(source >= 0);
    assert(source < g->n);

    for(i = 0; i < g->alist[source]->d; i++) {
        f(g, source, g->alist[source]->list[i], data);
    }
}

/* print the graph */
void graphPrint(Graph g){
    printf("Graph g has %d vertices and %d edges\n", g->n, g->m);
    for (int i = 0; i<g->n; i++){
        for (int j = 0; j< g->alist[i]->d; j++){ /* = ? */
            printf("Vertex %d has edges to %d\n", i, g->alist[i]->list[j]);
        }
    }
}