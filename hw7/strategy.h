/*
 * Interface for card-playing strategy.
 *
 * The deal function supplies a new card to the strategy.  Each possible card will only be dealt once.
 *
 * The play function should return a card that has been dealt previously but not yet played.
 * If asked for a card when the hand is empty, its behavior is undefined.
 */

#include <stdint.h>

typedef uint64_t Card;   /* representation of a card */

/* opaque type for strategy data */
typedef struct strategy Strategy;

/* set up a new strategy for numPiles many piles */
Strategy *strategyCreate(int numPiles);

/* clean up all space used by a strategy */
void strategyDestroy(Strategy *);

/* add a card to the current hand */
void strategyDeal(Strategy *, Card);

/* play a card from pile k */
Card strategyPlay(Strategy *, int k);