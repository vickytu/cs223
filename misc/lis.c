#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* returns the longest increasing subsequence of s
 * as a freshly-malloc'd string that you must free */
char *
lis(const char *s)
{
    size_t len;
    size_t i;
    size_t j;

    struct entry {
        size_t len;           /* length of lis including this character */
        size_t predecessor;   /* previous position in lis if len > 1 */
    };
    
    struct entry *table;
    struct entry best;    /* best sequence so far for i */

    char *ss;  /* actual LIS string */
    char *top; /* next position in ss to write */

    len = strlen(s);
    table = malloc(sizeof(struct entry) * len);

    /* fill in table according to dynamic programming rule */
    for(i = 0; i < len; i++) {
        best.len = 1;
        best.predecessor = i;   /* maybe not necessary */

        /* consider all possible longer sequences using some other j */
        for(j = 0; j < i; j++) {
            if(s[j] <= s[i] && table[j].len + 1 > best.len) {
                best.len = table[j].len + 1;
                best.predecessor = j;
            }
        }

        table[i] = best;
    }
    
    /* now find LIS and extract it */
    best.len = 0;
    best.predecessor = 0;   /* last element in LIS */

    for(i = 1; i < len; i++) {
        if(table[i].len > best.len) {
            best.len = table[i].len;
            best.predecessor = i;
        }
    }

    /* now best.len = length of LIS */
    /* best.predecessor = last position in LIS */
    ss = malloc(best.len + 1);
    top = ss + best.len;

    *top-- = '\0';

    for(i = best.predecessor; table[i].predecessor != i; i = table[i].predecessor) {
        *top-- = s[i];
    }

    *top = s[i];

    free(table); /* remember to free things */

    return ss;
}

int
main(int argc, char **argv)
{
    char *ss;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s string\n", argv[0]);
        return 1;
    }

    ss = lis(argv[1]);
    puts(ss);
    free(ss);

    return 0;
}
