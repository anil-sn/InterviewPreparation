#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/* Returns 1 if n is a lucky no. ohterwise returns 0*/
bool isLucky(int n) 
{ 
	static int counter = 2; 
	
	if(counter > n) 
		return 1; 
	if(n%counter == 0)
		return 0;	 
	
	/*variable next_position is just for readability of the program we can remove it and use n only */
	/*calculate next position of input no*/
	int next_position = n; 
	next_position -= next_position/counter; 
	
	counter++; 
	return isLucky(next_position); 
} 

/*Driver function to test above function*/
int main() 
{ 
	int x = 13; 
	if( isLucky(x) ) 
		printf("%d is a lucky no.", x); 
	else
		printf("%d is not a lucky no.", x); 
	getchar(); 
} 