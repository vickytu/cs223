#include "safe.h"
#include "openSafe.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define UNSIGNED_LONG_LONG_MAX (18446744073709551615ULL)

/* Hours Spent: 5 hours for coming up with algorithm, 4 hours for making sure it works, 2 hours for dealing with big safes */

/* a special power function that returns base^exp, unless base^exp is too big for even an unsigned long long int, in which it 
returns the max number unsigned long long will hold */
unsigned long long power(int base, int exp)
{
    unsigned long long result = 1;
    for (int i = 0; i < exp; i++) {
      result *= base;
      if (result > (UNSIGNED_LONG_LONG_MAX / exp) && i < (exp - 1)) {
        result = UNSIGNED_LONG_LONG_MAX;
        break;
      }
    }
  return result;
}

int openSafe(Safe *s)
{
  /* declaring and initializing all my variables */
  int positions = numPositions(s);
  int tumblers = numTumblers(s);
  unsigned long long totComb = power(positions, tumblers);
  int k = 0;
  int rem = 0;
  int quotient = 0;
  int ver = 0;
  int *a;
  /* intitializing my malloc'd array to all 0s */
  a = malloc(sizeof(int) * tumblers);
  assert(a);
  for (int j = 0; j< tumblers; j++) {
    a[j] = 0;
  }
  /* iteratively going through all possible combinations, building each combination from the end to a[0] using rem and quotient */
  for (unsigned long long i = 0; i<totComb; i++) {
    k = tumblers-1;               /* this is redundant because I could have tested my initialized array but I did not want to */
    rem = i % positions;
    quotient = i / positions;
    a[k] = rem;
    k--;
    while (quotient > 0)
    {
      rem = quotient % positions;
      quotient = quotient / positions;
      a[k] = rem;
      k--;
    }
    /* trying each combination and depending on the return, breaks the loop, frees the array and returns SAFE_SELF_DESTRUCTED or the amount in
    the safe, or loops again with i++ */
    ver = tryCombination(s, a);
    if (ver != SAFE_BAD_COMBINATION)
    {
      break;
    }
  }
  free(a);
  return ver;  
}
  



	      
