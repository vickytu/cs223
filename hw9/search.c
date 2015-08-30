#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "graph.h"
#include "search.h"

/* create an array of n ints initialized to SEARCH_INFO_NULL */
static int *
createEmptyArray(int n)
{
    int *a;
    int i;

    a = malloc(sizeof(*a) * n); /* space for array of n ints*/
    assert(a);

    for(i = 0; i < n; i++) {
        a[i] = SEARCH_INFO_NULL;    /* declared in search.h, SEARCH_INFO_NULL = -1 */
    }

    return a;
}

/* allocate and initialize search results structure */
/* you need to do this before passing it to dfs or bfs */
struct searchInfo *
searchInfoCreate(Graph g)
{
    struct searchInfo *s;
    int n;

    s = malloc(sizeof(*s)); /* space for struct searchInfo, s is pointer to this space */
    assert(s);

    s->graph = g;
    s->reached = 0;

    n = graphVertexCount(g);

    s->preorder = createEmptyArray(n);
    s->time = createEmptyArray(n);
    s->parent = createEmptyArray(n);
    s->depth = createEmptyArray(n);

    return s;
} 

/* free searchInfo data---does NOT free graph pointer (why? b/c we may still need it) */
void
searchInfoDestroy(struct searchInfo *s)
{
    free(s->depth);
    free(s->parent);
    free(s->time);
    free(s->preorder);
    free(s);
}

/* used inside search routines */
struct edge {
    int u;          /* source */
    int v;          /* sink */
};

/* stack/queue */
struct queue {
    struct edge *e;
    int bottom;
    int top;
};

static void
pushEdge(Graph g, int u, int v, void *data)
{
    struct queue *q;

    q = data;

    assert(q->top < graphEdgeCount(g) + 1);

    q->e[q->top].u = u;
    q->e[q->top].v = v;
    q->top++;
}

/* this rather horrible function implements dfs if useQueue == 0 */
/* and bfs if useQueue == 1 */
static void
genericSearch(struct searchInfo *r, int root, int useQueue)
{
    /* queue/stack */
    struct queue q;

    /* edge we are working on */
    struct edge cur;

    /* start with empty q */
    /* we need one space per edge */
    /* plus one for the fake (root, root) edge */
    q.e = malloc(sizeof(*q.e) * (graphEdgeCount(r->graph) + 1));
    assert(q.e);

    q.bottom = q.top = 0;

    /* push the root */
    pushEdge(r->graph, root, root, &q);

    /* while q.e not empty */
    while(q.bottom < q.top) {
        if(useQueue) {
            cur = q.e[q.bottom++];
        } else {
            cur = q.e[--q.top];
        }


        /* did we visit sink already? */
        if(r->parent[cur.v] != SEARCH_INFO_NULL) continue; 
        
        /* no */
        assert(r->reached < graphVertexCount(r->graph));
        r->parent[cur.v] = cur.u;
        r->time[cur.v] = r->reached;
        r->preorder[r->reached++] = cur.v;
        if(cur.u == cur.v) {
            /* we could avoid this if we were certain SEARCH_INFO_NULL */
            /* would never be anything but -1 */
            r->depth[cur.v] = 0;
        } else {
            r->depth[cur.v] = r->depth[cur.u] + 1;
        }

        /* push all outgoing edges */
        graphForeach(r->graph, cur.v, pushEdge, &q);
    }

    free(q.e);
}

/* returns endpoints of a cycle */
static int *
genericSearchModified(struct searchInfo *r, int root, int useQueue)
{
    /* queue/stack */
    struct queue q;

    /* edge we are working on */
    struct edge cur;

    int node;

    int *cycle;
    cycle = malloc(sizeof(int) * 2);

    /* start with empty q */
    /* we need one space per edge */
    /* plus one for the fake (root, root) edge */
    q.e = malloc(sizeof(*q.e) * (graphEdgeCount(r->graph) + 1));
    assert(q.e);

    q.bottom = q.top = 0;

    /* push the root */
    pushEdge(r->graph, root, root, &q);

    /* while q.e not empty */
    while(q.bottom < q.top) {
        if(useQueue) {
            cur = q.e[q.bottom++];
        } else {
            cur = q.e[--q.top];
        }
        node = 0;

        /* did we visit sink already? */
        if(r->parent[cur.v] != SEARCH_INFO_NULL) {
            /* yes. we know that cur.u is the not the parent of cur.v. but is this a cycle? */
            while(r->preorder[node] != cur.v) {
            //if(r->preorder[node] != r->parent[cur.u] && r->preorder[node] != cur.v) {
                if(graphHasEdge(r->graph, cur.u, r->preorder[node])) {
                /*printf("cycle found containing %d and %d\n", cur.u, r->preorder[node]);*/
                cycle[0] = cur.u;
                cycle[1] = r->preorder[node];
                goto out;
            }
           // }
            node++;
        }
            continue; 
        }
        

        /* no */
        assert(r->reached < graphVertexCount(r->graph));
        r->parent[cur.v] = cur.u;
        r->time[cur.v] = r->reached;
        r->preorder[r->reached++] = cur.v;
        if(cur.u == cur.v) {
            /* we could avoid this if we were certain SEARCH_INFO_NULL */
            /* would never be anything but -1 */
            r->depth[cur.v] = 0;
        } else {
            r->depth[cur.v] = r->depth[cur.u] + 1;
        }

        /* push all outgoing edges */
        graphForeach(r->graph, cur.v, pushEdge, &q);
    }
    out:
    free(q.e);
    return cycle;
}

/* returns endpoints of a cycle */
int *
dfsModified(struct searchInfo *results, int root)
{
    int *cycle;
    //cycle = malloc(sizeof(int) * 2);
    cycle = genericSearchModified(results, root, 0);
    return cycle;
}

/* depth first search */
void
dfs(struct searchInfo *results, int root)
{
    genericSearch(results, root, 0);
}

/* breath first search */
void
bfs(struct searchInfo *results, int root)
{
    genericSearch(results, root, 1);
}

/* print searchInfo */
void searchInfoPrint(struct searchInfo *results){
    int nodeInfo;
    graphPrint(results->graph);

    printf("%d vertices reached, in this order: ", results->reached);
    for (int i = 0; i<results->reached; i++){
        nodeInfo = results->preorder[i];
        printf("node %d, child of node %d, took %d steps to reach, and is %d from the root, \n", nodeInfo, results->parent[nodeInfo], results->time[nodeInfo], results->depth[nodeInfo]);
    }
}