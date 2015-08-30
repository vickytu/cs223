int
main(int argc, char **argv)
{
	Strategy *s;
    Card temp;
    int i;
    int j;
    const int n = 16;
    struct tree handBuiltroot;
    struct tree handBuiltLeft;

    struct tree *root = EMPTY_TREE;

    handBuiltroot.key = 3;
    handBuiltroot.child[LEFT] = &handBuiltLeft;
    handBuiltroot.child[RIGHT] = 0;

    handBuiltLeft.key = 1;
    handBuiltLeft.child[LEFT] = handBuiltLeft.child[RIGHT] = 0;

    treePrint(&handBuiltroot);

    /* new improved tree using treeInsert */
    for(i = 0; i < n; i++) {
        treeInsert(&root, 5*i % n);
        treePrint(root);
    }

    /* get rid of root */
    for(i = 0; i <= n; i++) {
        treeDeleteMin(&root);
        treePrint(root);
    }
    s = strategyCreate(4);
    for (j = 0; j < 10; j++){
    	strategyDeal(s, j);
    	if (j % 2 == 0){
    		temp = strategyPlay(s, 3);
    	}
    }
    treePrint(s->pile[3]);
    treePrint(s->pile[2]);
    return 0;
}