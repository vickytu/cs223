#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

/* compute the nth Fibonacci number */
/* this sucks b/c it uses an exponential number of function calls */
uint64_t
fib(int n)
{
    if(n < 2) {
        return 1;
    } else {
        return fib(n-1) + fib(n-2);
    }
}

/* print the first n Fibonacci numbers */
void
printFib(int n)
{
    uint64_t *t;   /* table of values */
    int i;
    
    assert(n >= 2);

    t = malloc(sizeof(*t) * n);

    t[0] = t[1] = 1;

    for(i = 2; i < n; i++) {
        t[i] = t[i-1] + t[i-2];

        printf("f(%d) == %" PRIu64 "\n", i, t[i]);
    }

    free(t);
}

int
main(int argc, char **argv)
{
    int i;

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

#ifdef DUMB_WAY
    for(i = 0; i < 1000; i++) {
        printf("fib(%d) == %" PRIu64 "\n", i, fib(i));
    }
#else
    printFib(100);
#endif

    return 0;
}
