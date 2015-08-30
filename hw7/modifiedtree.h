#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "strategy.h"

#define NUM_CHILDREN (2)
#define LEFT (0)
#define RIGHT (1)

#define EMPTY_TREE (0)

struct tree {
    Card key;
    struct tree *child[NUM_CHILDREN];
};

/* prints out a tree */
void treePrint(const struct tree *root);

/* insert a new element into a tree */
void treeInsert(struct tree **root, Card key);

/* return and delete smallest element */
Card treeDeleteMin(struct tree **root);