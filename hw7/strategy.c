#include "modifiedtree.h"

// don't need to include strategy.h since it is included in modifiedtree.h

/* opaque type for strategy data */
struct strategy {
	struct tree **pile;	//pointer to struct tree *
	int numpiles;
};

/* set up a new strategy for numPiles many piles */
Strategy *strategyCreate(int numPiles){
	Strategy *s;
	int i;
	s = malloc(sizeof(Strategy));
	assert(s);
	s->numpiles=numPiles+1; //TRICKY: need numPiles + 1 since we have discard pile
	s->pile = malloc(s->numpiles * sizeof(struct tree *));
	assert(s->pile);
	for (i = 0; i < s->numpiles; i++){
		s->pile[i] = NULL;
	}
	return s; 
}

/* clean up all space used by a strategy */
void strategyDestroy(Strategy *s){
	int i;
	Card tempCard;
	for (i = 0; i < s->numpiles; i++){
		tempCard = treeDeleteMin(&s->pile[i]); 
		while (tempCard != -1) { //delete until the binary tree is no more
			tempCard = treeDeleteMin(&(s->pile[i]));
		}
	}
	free(s->pile);
	free(s);
}

/* add a card to the current hand */
void strategyDeal(Strategy *s, Card key){
	treeInsert(&(s->pile[s->numpiles-1]), key);
}

/* play a card from pile k */
Card strategyPlay(Strategy *s, int k){
	Card move;
	move = treeDeleteMin(&(s->pile[k]));
	treeInsert(&(s->pile[k-1]), move);
	return move;
}