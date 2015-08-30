typedef struct tape Tape;

typedef struct machine Machine;

#define TAPE_LEFT (0)
#define TAPE_RIGHT (1)
//#define TAPE_EMPTY (-1)

/* creates the tape with NULL pointers*/
Tape *tapeCreate(void);

/* pushes more 'a' onto the tape in the specified direction */
void tapePush(Tape *t, int direction);

/* changes the tape element at the position pointed at by pos */
void tapeChange(Tape *t, char symbol);

void tapePop(Tape *t, int direction);

/* determines if we've reached end of tape in the specified direction */
int tapeEnd(const Tape *t, int direction);

/* frees space used by tape so that valgrind doesn't complain */
void tapeDestroy(Tape *t);

/* creates the machine */
Machine *machineCreate(void);

/* frees space used by machine so that valgrind doesn't complain */
Machine *machineDestroy(void);




