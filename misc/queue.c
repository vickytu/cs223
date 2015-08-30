#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// an element in the queue
struct elt {
    struct elt *next;
    int value;
};

// the queue, storing pointers to the head and tail elements of the queue
struct queue {
    struct elt *head;
    struct elt *tail;
};

typedef struct queue *q; // pointer to queue

// returns a pointer to a queue
struct queue *
queueCreate(void) // if function takes no arguments, use void in the fcn definition
{
    struct queue *q; // creates pointer to queue

    q = malloc(sizeof(struct queue)); // allocates the appropriate space

    q->head = q->tail = 0; //sets the head and tail to NULL

    return q; // returns the pointer to queue
}

/* push a new value onto top of Queue */
void
enq(struct queue *q, int value)
{
    struct elt *e; // creates pointer to new queue element

    e = malloc(sizeof(struct elt)); // allocates the appropriate space
    assert(e);

    e->value = value;

    /* Because I will be the tail, nobody is behind me */
    e->next = 0; // tail is signified by NULL

    if(q->head == 0) {
        /* If the queue was empty, I become the head */
        q->head = e;
    } else {
        /* Otherwise I get in line after the old tail */
        q->tail->next = e;
    }

    /* I become the new tail */
    q->tail = e;
}

//returns 1 if queue is empty, 0 if queue is nonempty
int
queueEmpty(const struct queue *q)
{
    return (q->head == 0); // head is NULL only if queue is empty
}

// returns the value of the element at the head of the queue, and removes it from the queue
int
deq(struct queue *q)
{
    int ret;
    struct elt *e;

    assert(!queueEmpty(q)); // bad things happen if queue is already empty

    ret = q->head->value; // sets ret to the value of the element at the head of queue

    /* patch out first element */
    e = q->head; // makes e point to the head of queue
    q->head = e->next; // makes the queue head pointer point to the next thing in the queue

    free(e); // frees the head of the queue

    return ret;
}

/* print contents of queue on a single line, head first, then newLine */
void
queuePrint(struct queue *q)
{
    struct elt *e;

    for(e = q->head; e != 0; e = e->next) { // set e to the head of the queue, then keep iterating until e reaches NULL of the tail pointer
        printf("%d ", e->value);
    }
    
    putchar('\n');
}

/* free a queue and all of its elements */
void
queueDestroy(struct queue *q)
{
    while(!queueEmpty(q)) { // keep dequeing until the queue is empty
        deq(q); 
    }

    free(q); // then free the queue and its pointers
}

int
main(int argc, char **argv)
{
    int i;
    struct queue *q;

    q = queueCreate(); // since arguement was void, pass in nothing

    for(i = 0; i < 5; i++) {
        printf("enq %d\n", i);
        enq(q, i);
        queuePrint(q);
    }

    while(!queueEmpty(q)) {
        printf("deq gets %d\n", deq(q));
        queuePrint(q);
    }

    queueDestroy(q);

    return 0;
}