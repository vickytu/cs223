#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define HUN (100)

//Hours spent: 5 hr to get most of it, 1 hr to fix little bugs

//takes in the text array, coefficients, code word, and text length and returns whether the	code word can be found using that combination
int verify (char *a, int b, int c, int d, char *code, int length)
{
	int check = 0;
	for (int i = 3; i<strlen(code); i++) 		//the simple tests in main check for i= 0, 1, 2
	{
		check = b + c*i + d*i*i;
		if (check > length || a[check] != code[i])
		{
			return 1;
		} 
	}
	return 0;
}

int main(int argc, char **argv)
{
	char *a;				//text array
	int c = 0;				//ASCII of char
	int inc = 1;			//accurate length of the text file
	int length = 0;			//number of HUN allocated to text array
	int codelen = 0;		//length of code word
	a = malloc(sizeof(char) * HUN);
	assert(a);
  	while ((c = getchar()) != EOF)		//writes text into pointer array
  	{
  		if (length == HUN * inc)		//increases size of array if text very long
  		{
  			inc++;
  			a = realloc(a, HUN * inc);	
  			assert(a);
  		}
  		a[length] = c;
  		length++;
  	}
	for (int k = 1; k<argc; k++)		//loops for each code word
	{
		codelen = strlen(argv[k]);
		for (int b = 0; b<=length; b++)		//nested for loops to generate all combinations
		{
			if (a[b] != argv[k][0])			//simple tests to speed up the program
			{
				continue;
			}
			for (int c = 0; c<=(length/(codelen-1)); c++)
			{
				for (int d = 0; d<=(length/((codelen-1) * (codelen-1))); d++)
				{
					if ((c ==0 && d==0) || a[d*(codelen-1)*(codelen-1) + c*(codelen-1) + b] > length || a[b+c*2+d*4] != argv[k][2] || a[b+c+d] != argv[k][1])		//simple tests to speed up the program
					{
						continue;
					}
					if(verify(a, b, c, d, argv[k], length) == 0) 		//for the ones that pass the simple tests, they are thoroughly checked
					{
						printf("%d %d %d %s\n", b, c, d, argv[k]); 		//print the coefficients and code word
					}
				}
			}
		}
	}
	free(a); 		//free the pointer array
	return 0; 		//success
}