#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "skiplist.h"

int
main(int argc, char **argv)
{
    int n;
    Skiplist s;
    int i;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s number-of-elements\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    s = skiplistCreate();

    for(i = 0; i < n; i += 2) {
        skiplistInsert(s, i);
    }

    assert(skiplistSearch(s, -1) == INT_MIN);

    for(i = 0; i < n; i += 2) {
        assert(skiplistSearch(s, i) == i);
        assert(skiplistSearch(s, i+1) == i);
    }

    for(i = 0; i < n; i += 4) {
        skiplistDelete(s, i);
    }

    assert(skiplistSearch(s, 0) == INT_MIN);

    for(i = 2; i < n; i += 4) {
        assert(skiplistSearch(s, i) == i);
        assert(skiplistSearch(s, i+1) == i);
        assert(skiplistSearch(s, i+2) == i);
        assert(skiplistSearch(s, i+3) == i);
    }

    /* make sure insert/delete works with duplicates */
    for(i = 0; i < n; i++) {
        skiplistInsert(s, 0);
        assert(skiplistSearch(s, 0) == 0);
        assert(skiplistSearch(s, 1) == 0);
    }

    for(i = 0; i < n; i++) {
        assert(skiplistSearch(s, 0) == 0);
        assert(skiplistSearch(s, 1) == 0);
        skiplistDelete(s, 0);
    }

    assert(skiplistSearch(s, 0) == INT_MIN);

    skiplistDestroy(s);

    return 0;
}