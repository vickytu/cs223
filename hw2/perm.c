#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

long power(int base, int exp)
{
    int i;
    int result = 1;
    for (i = 0; i < exp; i++) {
        result *= base;
      }
    return result;
 }

int openSafe(int tum, int pos)
{
  int tumblers = tum;
  printf("%d\n", tum);
  int positions = pos;
  printf("%d\n", pos);
  int quotient = 0;
  int rem = 0;
  int *a;
  int k = tumblers-1;
  long totComb = power(positions, tumblers);
  printf("%ld\n", totComb);
  a = malloc(sizeof(int) * tumblers);
  assert(a);
  for (int j = 0; j< tumblers; j++) {
    a[j] = 0;
    //printf("%d", a[j]);
  }
  for (long i = 0; i<totComb; i++) {
    rem = i % positions;
    //printf("%ld", i);
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
    for (int i = 0; i<tumblers; i++) {
      printf("%d", a[i]);
    }
    printf("\n");
    k= tumblers-1;
  }
  free(a);
  return 0;
}

int main(int argc, char **argv)
{
  int tum = atoi(argv[1]);
  //printf("%d\n", tum);
  int pos = atoi(argv[2]);
  //printf("%d\n", pos);
  openSafe(tum, pos);
}