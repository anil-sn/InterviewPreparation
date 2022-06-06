//RECURSIVE:-
#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A recursive function to find nth catalan number 
unsigned long int catalan(unsigned int n) 
{ 
	// Base case 
	if (n <= 1) return 1; 

	// catalan(n) is sum of catalan(i)*catalan(n-i-1) 
	unsigned long int res = 0; 
	for (unsigned int i=0; i<n; i++) 
		res += catalan(i)*catalan(n-i-1); 

	return res; 
} 

// Driver program to test above function 
int main() 
{ 
	for (int i=0; i<10; i++) 
		printf("%ld ", catalan(i)); 

	printf("\r\n");
	return 0; 
} 
