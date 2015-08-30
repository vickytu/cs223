#include "avlTree.h"
#include "orderedSet.h"

/* Time spent: 2 days of changing Aspnes's implementation of AVL tree storing ints into AVL tree storing strings. Things I learned
included 1) gnu99 is the best thing EVER 2) I understand pointer logic! 3) always free when using strdup and most importantly 4)
NEVER GIVE UP! */

/*
 * Ordered set data structure.
 */
 struct orderedSet{
 	size_t numElts;
 	struct tree *node;
 }; 

/* Make a new empty set by mallocing space */
struct orderedSet *orderedSetCreate(void){
	struct orderedSet *s;
	s = malloc(sizeof(struct orderedSet));
	assert(s);
	s->numElts = 0;
	s->node = NULL;
	return s;
}

/* Destroy a set using treeDestroy */
void orderedSetDestroy(struct orderedSet *s){
	treeDestroy(&s->node);
	free(s);
}

/* How many elements in this set? ask s->numElts */
size_t orderedSetSize(const struct orderedSet *s){
	return s->numElts;
}

/* Insert a new element. Has no effect if element is already present. */
void orderedSetInsert(struct orderedSet *s, const char *c){
	if (treeContains(s->node, c) == 0){
		treeInsert(&s->node, c); /* treeInsert doesn't check for duplicates so we have to */
		s->numElts += 1;
	}
	return;
}

/* Delete an element.  Has no effect if element is not already present. */
void orderedSetDelete(struct orderedSet *s, const char *c){
	/* printf("DEBUG INFO: The set has %zu elements, and we are trying to delete %s\n", s->numElts, c); */
	if (s->numElts != 0){
		if (treeContains(s->node, c) == 1){
			treeDelete(&s->node, c); /* delete node after making sure the tree contains this elt */
			s->numElts = s->numElts - 1;
		}
	}
}

/* Return a new ordered set containing all elements e
 * for which predicate(arg, x) != 0.
 * The predicate function should be applied to the elements in increasing order. */
struct orderedSet *orderedSetFilter(const struct orderedSet *s, int (*predicate)(void *arg, const char *), void *arg){
	size_t i;
	struct orderedSet *s2 = orderedSetCreate();
	char* x;
	size_t max = treeSize(s->node);
	for (i = 0; i<max; i++){
		x = treeUnrank(s->node, i);
		if (predicate(arg, x) != 0) {
			orderedSetInsert(s2, x);
		}
	}
	/* printf("DEBUG INFO: %zu\n", s2->numElts);*/ 
	return s2;
}