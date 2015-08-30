#include <stdio.h>

int f(int *a, int **b) { 
	return *(b[0] = a) + 1; 
}

int main(int argc, char **argv) {
    int a = 1;   int b = 2;    int c = 3;
    int *x = &a; int *y = &b;
    *x = f(&c, &y);
    printf("%d %d %d\n", a, b, c);
    *y = f(x,  &x);
    printf("%d %d %d\n", a, b, c);
    *x = f(&b, &y);
    printf("%d %d %d\n", a, b, c);
    return 0;
}