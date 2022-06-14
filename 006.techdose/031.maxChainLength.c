#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

//https://www.youtube.com/watch?v=dgNdDMMb8Rk&list=PLEJXowNB4kPxQIN2dCUAnQ_92HIziG4x6&index=46&ab_channel=TECHDOSE


// This function assumes that arr[] 
// is sorted in increasing order 
// according the first (or smaller) values in Pairs. 
int maxChainLength( int arr[][2], int n) 
{ 
	int i, j, max = 0; 
	int *mcl =  calloc(n, sizeof(int));
	
	/* Initialize MCL (max chain length) 
	values for all indexes */
	for ( i = 0; i < n; i++ ) {
		mcl[i] = 1; 
    }
	
	/* Compute optimized chain length values in bottom up manner */
	for ( i = 1; i < n; i++ ) 
		for ( j = 0; j < i; j++ ) 
			if ( arr[i][0] > arr[j][1] && mcl[i] < mcl[j] + 1) 
				mcl[i] = mcl[j] + 1; 
	
	// mcl[i] now stores the maximum chain length ending with Pair i 
	
	/* Pick maximum of all MCL values */
	for ( i = 0; i < n; i++ ) 
		if ( max < mcl[i] ) 
			max = mcl[i]; 
	
	/* Free memory to avoid memory leak */
	
	return max; 
} 
	

/* Driver code */
int main() 
{ 
	int arr[][2] = { {5, 24}, {15, 25}, {27, 40}, {50, 60} }; 
	int n = sizeof(arr)/sizeof(arr[0]); 
	printf("Length of maximum size chain is %d \r\n", maxChainLength( arr, n )); 
	return 0; 
} 