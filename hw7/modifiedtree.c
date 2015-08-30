#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "modifiedtree.h"

/* recursive function to deal with indentation */
/* unnecessary since we don't need to print tree */
/*static void
treePrintIndented(const struct tree *root, int depth)
{
    int i;

    if(root) {
        treePrintIndented(root->child[LEFT], depth+1);*/
        
        /* print root */
        /*for(i = 0; i < 4*depth; i++) { putchar(' '); }
        printf("%llu %p\n", root->key, (void *) root);

        treePrintIndented(root->child[RIGHT], depth+1);
    }
}*/

/* prints out a tree */
/* unnecessary since we don't need to print tree */
/*void
treePrint(const struct tree *root)
{
    treePrintIndented(root, 0);
    puts("===");
}*/

/* insert a new element into a tree */
void
treeInsert(struct tree **root, Card key)
{
    int i;

    if(*root) {
        /* since each Card is unique, we don't need to check for dupliate keys
        if((*root)->key == key) {
            return;
        } else {*/
            treeInsert(&(*root)->child[(*root)->key < key], key);
        //}
    /* insert a new key by mallocing a struct tree for it */        
    } else {
        *root = malloc(sizeof(**root));
        (*root)->key = key;
        for(i = 0; i < NUM_CHILDREN; i++) { (*root)->child[i] = 0; }
    }
}

/* return 1 if target is in tree, 0 otherwise */
/* unnecessary since we don't need to search for any particular key */
/*int
treeContains(const struct tree *root, int target)
{
    if(root == 0) {
        return 0;
    } else if(root->key == target) {
        return 1;
    } else {
        return treeContains(root->child[root->key < target], target);
    }
}*/

/* return and delete smallest element */
Card
treeDeleteMin(struct tree **root)
{
    Card retval;
    struct tree *child;
    struct tree **leftmost;
    if(*root == 0) {
        /* no root */
        retval = -1;
        //return -1;
    } else 
    if((*root)->child[LEFT] == 0) {
        /* no left child, promote right child */
        retval = (*root)->key;
        child = (*root)->child[RIGHT];
        free(*root);
        *root = child;
    } else {
        /* have a left child, find pointer to leftmost descendant */
        for(leftmost = &(*root)->child[LEFT];
                (*leftmost)->child[LEFT] != 0;
                leftmost = &(*leftmost)->child[LEFT]);

        /* *leftmost is now leftmost descendant */
        retval = (*leftmost)->key;    
        //(*root)->key = (*leftmost)->key;
        child = *leftmost;
        *leftmost = child->child[RIGHT];
        free(child);
    }
    //printf("%llu\n", retval);
    return retval;
}