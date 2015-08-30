/* Implement a priority queue */

typedef int elt;

/* make a new priority queue */
struct pq *pqCreate(void);

/* insert a new element in the queue */
void pqInsert(struct pq *, elt);

/* delete and return smallest element */
elt pqDeleteMin(struct pq *);

/* destory the priority queue */
void pqDestroy(struct pq *);