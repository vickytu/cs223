/* Typical usage:
 *
 *    struct searchInfo *s;
 *    int n;
 *
 *    s = searchInfoCreate(g);
 *
 *    n = graph_vertices(g);
 *    for(i = 0; i < n; i++) {
 *        dfs(s, i);
 *    }
 *
 *    ... use results in s ...
 *
 *    searchInfoDestroy(s);
 *
 */

/* summary information per node for dfs and bfs */
/* this is not intended to be opaque---user can read it */
/* (but should not write it!) */

#define SEARCH_INFO_NULL (-1) /* for empty slots */

struct searchInfo {
    Graph graph;
    int reached;        /* count of reached nodes */
    int *preorder;      /* list of nodes in order first reached */
    int *time;          /* time[i] == position of node i in preorder list */
    int *parent;        /* parent in DFS or BFS forest */
    int *depth;         /* distance from root */
};

/* allocate and initialize search results structure */
/* you need to do this before passing it to dfs or bfs */
struct searchInfo *searchInfoCreate(Graph g);

/* free searchInfo data---does NOT free graph pointer */
void searchInfoDestroy(struct searchInfo *);

int *
dfsModified(struct searchInfo *results, int root);

/* perform depth-first search starting at root, updating results */
void dfs(struct searchInfo *results, int root);

/* perform breadth-first search starting at root, updating results */
void bfs(struct searchInfo *results, int root);

/* print searchInfo */
void searchInfoPrint(struct searchInfo *results);