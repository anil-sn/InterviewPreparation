#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/* lis() returns the length of the longest increasing 
subsequence in arr[] of size n */
int lis( int arr[], int n ) 
{ 
	int lis[n]; 

	lis[0] = 1; 

	/* Compute optimized LIS values in bottom up manner */
	for (int i = 1; i < n; i++ ) 
	{ 
		lis[i] = 1; 
		for (int j = 0; j < i; j++ ) {
			if ( arr[i] > arr[j] && lis[i] < lis[j] + 1)  {
				lis[i] = lis[j] + 1; 
			}
		}
	} 

	// Return maximum value in lis[] 
	int max = INT_MIN;
	for (int i = 0; i < n; i ++) {
		printf("%d ", lis[i]);
		max = max > lis[i] ? max : lis[i];
	}
	printf("\r\n");

	return max; 
} 
	
/* Driver program to test above function */
int main() 
{ 
	int arr[] = { 10, 22, 9, 33, 21, 50, 41, 60 }; 
	int n = sizeof(arr)/sizeof(arr[0]); 
	printf("Length of lis is %d\n", lis( arr, n ) ); 
	return 0; 
}