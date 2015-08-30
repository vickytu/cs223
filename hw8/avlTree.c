#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h> /* need for size_t */
#include <string.h>
#include <stdlib.h>

#include "avlTree.h"

/* every tree has a char * key, int height, size_t size, and two child pointers */

/* return height of tree */
int
treeHeight(const struct tree *root)
{
    if(root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        return root->height;
    }
}

/* recompute height from height of kids */
static int
treeComputeHeight(const struct tree *root)
{
    int childHeight;
    int maxChildHeight;
    int i;
    if(root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        maxChildHeight = TREE_EMPTY_HEIGHT;
        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            childHeight = treeHeight(root->child[i]);
            if(childHeight > maxChildHeight) {
                maxChildHeight = childHeight;
            }
        }
        return maxChildHeight + 1;
    }
}

/* return size of tree */
size_t
treeSize(const struct tree *root)
{
    if(root == 0) {
        return 0;
    } else {
        return root->size;
    }
} 

/* recompute size from size of kids */
static int
treeComputeSize(const struct tree *root)
{
    int size;
    int i;
    if(root == 0) {
        return 0;
    } else {
        size = 1;
        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            size += treeSize(root->child[i]);
        }
        return size;
    }
}

/* fix aggregate data in root */
/* assumes children are correct */
static void
treeFix(struct tree *root)
{
    if(root) {
        root->height = treeComputeHeight(root);
        root->size = treeComputeSize(root);
    }
}

/* rotate child in given direction to root */
static void
treeRotate(struct tree **root, int direction)
{
    struct tree *x;
    struct tree *y;
    struct tree *b;

    /*
     *      y           x 
     *     / \         / \
     *    x   C  <=>  A   y
     *   / \             / \
     *  A   B           B   C
     */

    y = *root;                  assert(y);
    x = y->child[direction];    assert(x);
    b = x->child[!direction];

    /* do the rotation */
    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;

    /* fix y then x */
    treeFix(y);
    treeFix(x);
}

/* restore AVL property at *root after an insertion or deletion */
static void
treeRebalance(struct tree **root)
{
    int tallerKid;
    if(*root) {
        for(tallerKid = 0; tallerKid < TREE_NUM_CHILDREN; tallerKid++) {
            if(treeHeight((*root)->child[tallerKid]) >= treeHeight((*root)->child[!tallerKid]) + 2) {
                /* check if zig-zag: opposite-direction nephew is the tall one */
                /* this also covers case where both nephews are too tall */
                if(treeHeight((*root)->child[tallerKid]->child[!tallerKid]) 
                            >= treeHeight((*root)->child[tallerKid]) - 1) {
                    /* zig zag case */
                    treeRotate(&(*root)->child[tallerKid], !tallerKid);
                }
                /* fall through to zig zig case */
                treeRotate(root, tallerKid);
                /* don't bother with other kid */
                break;
            }
        }
    }
}

/* free all elements of a tree, replacing it with TREE_EMPTY */
void 
treeDestroy(struct tree **root)
{
    int i;
    if(*root) {
        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            treeDestroy(&(*root)->child[i]);
        }
        free((*root)->key); /* since we used strdup */
        free(*root);
        *root = NULL;
    }
}

/* every comparison of keys requires strcmp since the keys are strings, not integers */
/* insert an element into a tree pointed to by root */
void
treeInsert(struct tree **root, const char* newElement)
{
    struct tree *e;
    if(*root == 0) {
        /* not already there, put it in */
        e = malloc(sizeof(*e));
        assert(e);
        e->key = strdup(newElement);
        assert(e->key);
        e->child[LEFT] = e->child[RIGHT] = 0;
        *root = e;
    } else {
        /* do this recursively so we can fix data on the way back out */
        if(strcmp((*root)->key, newElement) < 0){
            treeInsert(&(*root)->child[1], newElement);
        } else {
            treeInsert(&(*root)->child[0], newElement);
        }
    }
    /* fix the aggregate data */
    treeFix(*root);
    treeRebalance(root);
}

/* return 1 if target is in tree, 0 otherwise */
int
treeContains(const struct tree *t, const char* target)
{
    while(t && strcmp(t->key, target) != 0) {
        /* printf("DEBUG INFO: current key is %s\n", t->key); */
        if (strcmp(t->key, target) < 0){
            t = t->child[1];
        } else {
            t = t->child[0];
        }
    }
    return t != 0;
}

/* delete minimum element from the tree and return its key */
/* do not call this on an empty tree */
char*
treeDeleteMin(struct tree **root)
{
    struct tree *toFree;
    char* retval;
    assert(*root);  /* can't delete min from empty tree */
    if((*root)->child[LEFT]) {
        /* recurse on left subtree */
        retval = treeDeleteMin(&(*root)->child[LEFT]);
    } else {
        /* delete the root */
        toFree = *root;
        retval = strdup(toFree->key);
        assert(retval);
        *root = toFree->child[RIGHT];
        free(toFree->key);
        free(toFree);
    }
    /* fix the aggregate data */
    treeFix(*root);
    treeRebalance(root);
    return retval;
}

/* delete target from the tree */
/* has no effect if target is not in tree */
void 
treeDelete(struct tree **root, const char* target)
{
    struct tree *toFree;
    char* temp1;
    char* temp2;
    /* do nothing if target not in tree */
    if(*root) {
        /* printf("DEBUG INFO: current key is %s, target is %s\n", (*root)->key, target); */
        if(strcmp((*root)->key, target) == 0) {
            /* printf("DEBUG INFO: target found!\n"); */
            if((*root)->child[RIGHT]) {
                /* replace root with min value in right subtree */
                /* but when replacing we need to free the original key */
                temp1 = treeDeleteMin(&(*root)->child[RIGHT]);
                assert(temp1);
                temp2 = (*root)->key;
                (*root)->key = temp1;
                temp1 = temp2;
                free(temp1); 
                /* printf("DEBUG INFO: deleted key is %s\n", (*root)->key); */
            } else {
                /* patch out root */
                toFree = *root;
                *root = toFree->child[LEFT];
                free(toFree->key);
                free(toFree);
            }
        } else {
            if(strcmp((*root)->key, target) < 0){
                treeDelete(&(*root)->child[1], target);
            } else {
                treeDelete(&(*root)->child[0], target);
            }
        }
        /* fix the aggregate data */
        treeFix(*root);
        treeRebalance(root);
    }
    /*PROBLEM: Valgrind wants us to free eveyrthing we strdup, but we don't know where to do that/how*/
}

/* how far to indent each level of the tree */
#define INDENTATION_LEVEL (2)

/* print contents of a tree, indented by depth */
/* not static, may cause problems*/
void
treePrintIndented(const struct tree *root, int depth)
{
    int i;
    if(root != 0) {
        treePrintIndented(root->child[LEFT], depth+1);
        for(i = 0; i < INDENTATION_LEVEL*depth; i++) {
            putchar(' ');
        }
        printf("%s Height: %d Size: %zu (%p)\n", root->key, root->height, root->size, (void *) root);
        treePrintIndented(root->child[RIGHT], depth+1);
    }
}

/* print the contents of a tree */
void 
treePrint(const struct tree *root)
{
    treePrintIndented(root, 0);
}

/* return the number of elements in tree less than target */
size_t
treeRank(const struct tree *t, char* target)
{
    size_t rank = 0;
    while(t && strcmp(t->key, target) != 0) {
        if(strcmp(t->key, target) < 0) {
            /* go right */
            /* root and left subtree are all less than target */
            rank += (1 + treeSize(t->child[LEFT]));
            t = t->child[RIGHT];
        } else {
            /* go left */
            t = t->child[LEFT];
        }
    }

    /* we must also count left subtree */
    return rank + treeSize(t->child[LEFT]);
}

/* return an element with the given rank */
/* rank must be less than treeSize(root) */
char*
treeUnrank(const struct tree *t, size_t rank)
{
    size_t leftSize;
    /* basic idea: if rank < treeSize(child[LEFT]), recurse in left child */
    /* if it's equal, return the root */
    /* else recurse in right child with rank = rank - treeSize(child[LEFT]) - 1 */
    while(rank != (leftSize = treeSize(t->child[LEFT]))) {
        if(rank < leftSize) {
            t = t->child[LEFT];
        } else {
            t= t->child[RIGHT];
            rank -= (leftSize + 1);
        }
    }
    return t->key;
}
#define TEST_MAIN (1)
#define PRINT_AFTER_OPERATIONS (1)

/*#ifdef TEST_MAIN*/
/* check that aggregate data is correct throughout the tree */
/* not static may cause problems */
/*void
treeSanityCheck(const struct tree *root)
{
    int i;

    if(root) {
        assert(root->height == treeComputeHeight(root));
        assert(root->size == treeComputeSize(root));

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            treeSanityCheck(root->child[i]);
        }
    }
}

int
main(int argc, char **argv)
{
    int i;
    size_t rank;
    const int n = 10;
    char *str[10];
    str[0] = "potato";
    str[1] = "ham";
    str[2] = "apple";
    str[3] = "madness";
    str[4] = "zebra";
    str[5] = "ginger";
    str[6] = "carrot";
    str[7] = "nympho";
    str[8] = "vacuum";
    str[9] = "liest";

    struct tree *root = TREE_EMPTY;

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    for(i = 0; i < n; i++) {
        assert(!treeContains(root, str[i]));
        treeInsert(&root, str[i]);
        assert(treeContains(root, str[i]));
#ifdef PRINT_AFTER_OPERATIONS
        treePrint(root);
        puts("---");
#endif
    }*/
    /* check ranks */
    /*for(i = 0; i < n; i++) {*/
        /*assert(treeRank(root, str[i]) == i);
        assert(treeUnrank(root, i) == i);*/
        /*rank = treeRank(root, str[i]);
        printf("DEBUG INFO: %s has rank %zu, which has %s\n", str[i], rank, treeUnrank(root, rank));
    }

    treeSanityCheck(root);*/

    /* now delete everything */
    /*for(i = 0; i < n; i++) {
        assert(treeContains(root, str[i]));
        treeDelete(&root, str[i]);
        assert(!treeContains(root, str[i]));
#ifdef PRINT_AFTER_OPERATIONS
        treePrint(root);
        puts("---");
#endif
    }

    treeSanityCheck(root);

    treeDestroy(&root);

    return 0;
}
#endif*/