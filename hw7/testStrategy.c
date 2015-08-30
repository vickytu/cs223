#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <inttypes.h> /* for PRIx64 */
#include <limits.h>   /* for UCHAR_MAX */

#include "strategy.h"

/* assumes Card is uint64_t */

#define ENCODER (UINT64_C(10914310968679363173))
#define DECODER (UINT64_C(8501684282480127853))
#define OFFSET (UINT64_C(4709516008058455934))

#define CARD_FORMAT "%016" PRIx64

static uint64_t
hash(uint64_t x)
{
    return ENCODER * x + OFFSET;
}

static uint64_t
unhash(uint64_t h)
{
    return DECODER * (h - OFFSET);
}

struct gameState {
    Card numCards;               /* number of cards in deck */
    unsigned char numPiles;      /* how many piles */
    unsigned char *pile;         /* pile[c] = what pile hash(c) is in */
    Card *pileSize;     /* how many cards in this pile */
    Strategy *strategy;          /* strategy for this game */
};

static struct gameState *
gameStateCreate(Card numCards, int numPiles)
{
    struct gameState *s;

    /* we can't handle more piles than fit in a char */
    assert(numPiles <= UCHAR_MAX);

    s = malloc(sizeof(struct gameState));
    assert(s);

    s->numCards = numCards;
    s->numPiles = numPiles;
    s->pile = calloc(numCards, sizeof(char));
    s->pileSize = calloc(numPiles+1, sizeof(Card));
    s->strategy = strategyCreate(numPiles);

    return s;
}

static void
gameStateDestroy(struct gameState *s)
{
    free(s->pile);
    free(s->pileSize);
    strategyDestroy(s->strategy);

    free(s);
}

/* call deal on the next card */
static void
gameStateDeal(struct gameState *s, Card c)
{
    Card h;

    /* can't deal if it's already out there */
    assert(c < s->numCards);
    assert(s->pile[c] == 0);

    h = hash(c);

#ifdef DEBUG_OUTPUT
    printf("Dealing " CARD_FORMAT "\n", h);
#endif

    s->pile[c] = s->numPiles;
    s->pileSize[s->numPiles]++;

    strategyDeal(s->strategy, h);
}

/* call play */
static void
gameStatePlay(struct gameState *s, int pile)
{
    Card h;
    Card card;

    assert(pile > 0);
    assert(pile <= s->numPiles);
    assert(s->pileSize[pile] > 0);

    h = strategyPlay(s->strategy, pile);
    card = unhash(h);

    assert(card < s->numCards);     /* was h an encoding of a real card? */
    assert(s->pile[card] == pile);  /* is that card in this pile? */

#ifdef DEBUG_OUTPUT
    printf("Playing " CARD_FORMAT " from pile %d\n", h, pile);
#endif

    s->pile[card] = pile - 1;
    s->pileSize[pile]--;
    s->pileSize[pile-1]++;
}

/* print minimum value in each pile */
static void
gameStatePrintMinimums(const struct gameState *s)
{
    int i;
    Card c;
    Card h;
    Card *mins;

    mins = malloc(sizeof(Card) * (s->numPiles+1));
    assert(mins);

    for(i = 1; i < s->numPiles+1; i++) {
        mins[i] = UINT64_MAX;
    }

    for(c = 0; c < s->numCards; c++) {
        if(s->pile[c] != 0) {
            h = hash(c);
            if(h < mins[s->pile[c]]) {
                mins[s->pile[c]] = h;
            }
        }
    }

    for(i = s->numPiles; i > 0; i--) {
        if(mins[i] == UINT64_MAX) {
            puts("-");
        } else {
            printf(CARD_FORMAT "\n", mins[i]);
        }
    }

    free(mins);
}

int
main(int argc, char **argv)
{
    //printf("here");
    struct gameState *s;
    Card numCards;
    int numPiles;
    Card c;     /* card we are dealing */
    int pile;
    const char *playRule;

    if(argc != 4) {
        fprintf(stderr, "Usage: %s playRule numCards numPiles\n", argv[0]);
        return 1;
    }

    playRule = argv[1];
    //printf("here");
    /* format string will need to change if Card changes */
    sscanf(argv[2], "%" SCNu64, &numCards);
    numPiles = atoi(argv[3]);
    //printf("%d %c", numPiles, *playRule);

    s = gameStateCreate(numCards, numPiles);

    for(c = 0; c < numCards; c++) {
        //printf("%llu", c);
        gameStateDeal(s, c);

        switch(playRule[0]) {
            case 'h':
                /* "Harmonic" */
                /* this plays from top pile half the time, from next one-third of the time, etc. */
                for(pile = numPiles; pile > 0; pile--) {
                    if(c % (numPiles + 2 - pile) == 0) {
                        gameStatePlay(s, pile);
                    }
                }
                break;
            case 'r':
                /* "Round robin" */
                /* rotates through all piles starting with the top */
                gameStatePlay(s, numPiles - (c % numPiles));
                break;
            case 't':
                /* "Top" */
                /* always plays from top deck */
                gameStatePlay(s, numPiles);
                break;
            case 'a':
                /* "Alternate" */
                /* alternate between top two piles */
                gameStatePlay(s, numPiles - (c % 2));
                break;
            default:
                fprintf(stderr, "Unknown rule %s\n", playRule);
                return 2;
        }
    }

    gameStatePrintMinimums(s);

    gameStateDestroy(s);
        
    return 0;
}