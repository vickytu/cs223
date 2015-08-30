#include <stdio.h>
#include <ctype.h> 
#define START_OFFSET (17)
#define UPPER_OFFSET (5)
#define LOWER_OFFSET (3)
#define ALPHA_LENGTH (26)
#define UPPER_MIN (65)
#define UPPER_MAX (90)
#define LOWER_MIN (97)
#define LOWER_MAX (122)

/* Hours spent: 4 hrs. I'm still confused why isupper worked on my machine but produced problems when I tried
to run it via the Zoo's submit program. Also would appreciate it if assignment instruction did NOT include 
misleading hints */

/* This program only has one function, and that is to encode whatever text it is given. 4 constants are 
defined at the beginning to make this code easily editable if we were using a different alphabet system or
offset */
int main(int argc, char **argv)
{
	int offset = START_OFFSET; 		/* offset variable is changed throughout the program execution so we define it inside the function */
	int c = 0; 		/* initialized to 0 just in case, holds the int of the char */
	while ((c = getchar()) != EOF) 		/* run until end of file */ 
	{ 
		if (c >= 65 && c<=90) 		/* if c is in this range it is a uppercase letter so we subtract 'A' to get it to start from 0 and increase offset by UPPER_OFFSET */ 
		{
			c = c - 'A';
			c = (c + offset) % ALPHA_LENGTH;
			c = c + 'A';
			offset = (offset + 5) % ALPHA_LENGTH;
		}
		else if (c >= 97 && c <=122) 		/* if c is in this range it is a lowercase letter so we subtract 'a' to get it to start from 0 and increase offset by LOWER_OFFSET */
		{
			c = c - 'a';
			c = (c + offset) % ALPHA_LENGTH;
			c = c + 'a';
			offset = (offset + 3) % ALPHA_LENGTH;
		}
		else { 		/* if neither uppercase or lowercase we leave it and offset as is */
		}
		putchar(c); 		/* print encoded letter or char if not letter */
	}
	return 0; 		/* success */
}
/* editted using Sublime SFTP */