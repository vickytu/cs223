#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


// an element in the stack
struct elt {
    struct elt *next;
    int value;
};

typedef struct elt *Stack; //defining Stack as a point to an element in the stack

#define STACK_EMPTY (0)

/* push a new value onto top of stack */
void
stackPush(Stack *s, int value)
{
    struct elt *e; // create pointer to element in stack

    e = malloc(sizeof(struct elt)); // give it enough space
    assert(e);

    e->value = value; 
    e->next = *s; // the new element points to where *Stack points to
    *s = e; // the *Stack pointer points to the newly added element
}

// returns 1 if s points to nothing. returns 0 otherwise
int
stackEmpty(const Stack *s)
{
    return (*s == 0); /*f a pointer is being compared to the constant literal 0, then this is a check to see if the pointer is 
    a null pointer. This 0 is then referred to as a null pointer constant. But in general, NUL = '\0' = '0' =/= NULL */
}

// returns the val of the element at the top of the stack, and removes the element from the stack
int
stackPop(Stack *s)
{
    int ret; // to hold the val of the element to be popped
    struct elt *e; /* we need to free the space held by the first element in the stack so we have a pointer to point to it so 
    after s moves, we can still free the space */

    assert(!stackEmpty(s)); // bad things happen if we find the stack to be empty

    ret = (*s)->value;

    /* patch out first element */
    e = *s;
    *s = e->next;

    free(e); // frees the space pointed to by e, not e itself

    return ret;
}

/* print contents of stack on a single line, then newLine */
void
stackPrint(const Stack *s)
{
    struct elt *e;

    for(e = *s; e != 0; e = e->next) { // e pointing at what s is pointing at, e not a null pointer
        printf("%d ", e->value);
    }
    
    putchar('\n');
}

int
main(int argc, char **argv)
{
    int i;
    Stack s; // creates a stack pointer. Since we typedef'ed it as *Stack, the compiler will know that this is a pointer

    s = STACK_EMPTY; // sets it as a NULL pointer

    for(i = 0; i < 5; i++) {
        printf("push %d\n", i);
        stackPush(&s, i); // need &s or else this would have looked like stackPush(*s, i) to the compiler
        stackPrint(&s);
    }

    while(!stackEmpty(&s)) {
        printf("pop gets %d\n", stackPop(&s));
        stackPrint(&s);
    }

    return 0;
}