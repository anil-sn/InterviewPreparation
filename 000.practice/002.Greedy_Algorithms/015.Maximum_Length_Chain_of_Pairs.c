#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}
/*
    You are given n pairs of numbers. In every pair, the first number is always smaller
    than the second number. A pair (c, d) can follow another pair (a, b) if b < c.
    Chain of pairs can be formed in this fashion. Find the longest chain which can be formed
    from a given set of pairs.

    For example, if the given pairs are
    {{5, 24}, {39, 60}, {15, 28}, {27, 40}, {50, 90} },
    then the longest chain that can be formed is of length 3,
    and the chain is {{5, 24}, {27, 40}, {50, 90}}

    This problem is a variation of standard Longest Increasing Subsequence problem.
    Following is a simple two step process.
    1) Sort given pairs in increasing order of first (or smaller) element.
        Why do not need sorting? Consider the example {{6, 8}, {3, 4}} to understand the need
        of sorting. If we proceed to second step without sorting,
        we get output as 1. But the correct output is 2.

    2) Now run a modified LIS process where we compare the second element of
        already finalized LIS with the first element of new LIS being constructed.
*/

// Structure for a pair
struct pair
{
  int a;
  int b;
};

void
printArray(struct pair p[], int arr[], int n){
    for (int i = 0; i < n; i++){
        printf("(%d,%d) %d ", p[i].a, p[i].b, arr[i]);
    }
    printf("\r\n");
}

// This function assumes that
// arr[] is sorted in increasing order
// according the first
// (or smaller) values in pairs.
int maxChainLength( struct pair arr[], int n)
{
   int i, j, max = 0;
   int *mcl = (int*) malloc ( sizeof( int ) * n );

   /* Initialize MCL (max chain
     length) values for all indexes */
   for ( i = 0; i < n; i++ )
      mcl[i] = 1;

    printArray(arr, mcl, n);
    /* Compute optimized chain length values in bottom up manner */
    for ( i = 1; i < n; i++ ){
        for ( j = 0; j < i; j++ ) {
            if ( arr[i].a > arr[j].b && mcl[i] < mcl[j] + 1) {
                mcl[i] = mcl[j] + 1;
            }
            printf("i=%d, j=%d  ", i, j);
            printArray(arr, mcl, n);
        }
    }

   // mcl[i] now stores the maximum
   // chain length ending with pair i

   /* Pick maximum of all MCL values */
   for ( i = 0; i < n; i++ )
      if ( max < mcl[i] )
         max = mcl[i];

   /* Free memory to avoid memory leak */
   free( mcl );

   return max;
}

/* Driver program to test above function */
int main()
{
    struct pair arr[] = { {5, 24}, {15, 25}, {27, 40}, {50, 60} };
    int n = sizeof(arr)/sizeof(arr[0]);
    printf("\r\n Length of maximum size chain is %d\n", maxChainLength( arr, n ));
    return 0;
}