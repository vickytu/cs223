#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* return larger of two value */
/* ONLY works on ints */
int max(int x, int y)
{
    if(x > y) {
        return x;
    } else {
        return y;
    }
}

/* works on anything that supports < */
#define Max(x,y) (((x) > (y)) ? (x) : (y))

/* inline version of max for ints */
static inline int
maxInline(int x, int y)
{
    if(x > y) {
        return x;
    } else {
        return y;
    }
}


/* why parentheses are important */
#define BadTimes(x,y) (x*y)
#define Times(x,y) ((x)*(y))

/* variable arguments */
#define ErrPrintf(formatString, ...) (fprintf(stderr, (formatString), __VA_ARGS__))

/* arguments as strings */
#define BadEval(expr) (printf("expr = %d\n", expr))

/* #expr expands to quoted version of expr */
/* "foo" "bar" is equivalent to "foobar" */
#define BadEval2(expr) (printf(#expr " = %d\n", expr))

/* get expr out of the format string! */
#define Eval(expr) (fputs(#expr, stdout), printf(" = %d\n", expr))

/* ## concatenates identifiers */
#define MakeBigger(Prefix, Type) \
Type  \
Prefix ## Bigger (Type x, Type y) \ 
{ \
    if(x > y) {  \
        return x;  \
    } else {  \
        return y;  \
    } \
}


MakeBigger(int, int)
MakeBigger(float, float)

/* non-syntactic macros */
/* DON'T DO THIS */
#define Oppan if
#define Gangnam {
#define Style }

/* more justified, still a bad idea */
#define ForLoop for(i = 0; i < n; i++) {
#define EndForLoop }

/* looks like a statement */
#define ForEach(var, from, to, body)  \
        do {  \
            for(var = from; var < to; var++) { body }  \
        } while(0)

/* wrapper for malloc */
/* malloc appropriate thing and assign it to x */
#define Alloc(x) ((x = malloc(sizeof(*(x)))), assert(x))

int
main(int argc, char **argv)
Gangnam
    int d = 3;
    int i;
    int n = 10;
    struct foo {
        int n;
        int z;
    };

    struct foo *PSY;

    Alloc(PSY);
    free(PSY);

    ForLoop
        Eval(i);
    EndForLoop

    Oppan(2 == 3) Gangnam
        puts("disaster!");
    Style

    printf("max(2,3) = %d\n", max(2,3));
    printf("maxInline(2,3) = %d\n", maxInline(2,3));
    printf("Max(2,3) = %d\n", Max(2,3));
    printf("max(2.5,3.5) = %d\n", max(2.5,3.5));
    printf("Max(2.5,3.5) = %g\n", Max(2.5,3.5));
    Max(puts("Zoo"), puts("there"));

    printf("BadTimes(2+3,4) = %d\n", BadTimes(2+3,4));
    printf("Times(2+3,4) = %d\n", Times(2+3,4));

    ErrPrintf("This is an error: %d+%d\n", 127, 128);

    BadEval(2+2);
    BadEval2(2+2);
    BadEval2(5%d);

    Eval(d);
    Eval(5%d);

    Eval(intBigger(5, 3));
    Eval((int) floatBigger(5.5, 3.5));

    ForEach(i, 0, 10, Eval(i+3); Eval(i+5); );

#ifdef DEBUG
    fputs("almost done!\n", stderr);
#else
    fputs("You didn't compile with -DDEBUG\n", stderr);
#endif

#ifndef DEBUG
    fputs("please compile with -DDEBUG\n", stderr);
#endif

#if 0
    This is is terrible block of non-syntactic garbage.
#endif 

#if defined(DEBUG_LEVEL) && DEBUG_LEVEL > 3
    explode();
#endif

    return 0;
Style