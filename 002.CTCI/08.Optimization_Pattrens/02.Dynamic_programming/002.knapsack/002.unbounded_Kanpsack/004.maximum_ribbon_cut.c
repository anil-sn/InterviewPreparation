#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
Ribbon cut
    Maximum number of segments of lengths a, b and c

    Given a positive integer N, find the maximum number of segments of lengths a, b and c that can be formed from N .
        Examples :

        Input : N = 7, a = 5, b, = 2, c = 5
        Output : 2
            N can be divided into 2 segments of lengths
            2 and 5. For the second example,

        Input : N = 17, a = 2, b = 1, c = 3
        Output : 17
            N can be divided into 17 segments of 1
            or 8 segments of 2 and 1 segment of 1.
            But 17 segments of 1 is greater than 9 segments of 2 and 1.

*/

// function to find the maximum
// number of segments
int maximumSegments(int n, int a, int b, int c)
{
    // stores the maximum number of
    // segments each index can have
    int dp[n + 1];

    // initialize with -1
    memset(dp, -1, sizeof(dp));

    // 0th index will have 0 segments
    // base case
    dp[0] = 0;

    for (int i = 0; i <n+1; i++) {
        printf("%3d ", dp[i]);
    }
    printf("\r\n");

    // traverse for all possible segments till n
    for (int i = 0; i < n; i++) {
        if (dp[i] != -1) {
            // conditions
            //avoid buffer overflow
            if(i + a <= n ) {
                dp[i + a] = max(dp[i] + 1, dp[i + a]);
            }

            if(i + b <= n ) {
                dp[i + b] = max(dp[i] + 1, dp[i + b]);
            }

            if(i + c <= n ) {
                dp[i + c] = max(dp[i] + 1, dp[i + c]);
            }
        }
    }
    for (int i = 0; i <n+1; i++) {
        printf("%3d ", dp[i]);
    }
    printf("\r\n");

    return dp[n];
}

int maxCut(int n, int a, int b, int c)
{
    int max = 0 ;
    for ( int i = 0 ; i <= n ; ++i ){
        for ( int j = 0 ; j <= n ; ++j ){
            int k = ( n - ( i * a ) - ( j * b ) ) / c ;
            //printf("i:%d, j:%d, k: %d max:%d\r\n", i, j, k, max);
            if ( ( i * a + j * b + c * k ) == n
                 && i >= 0
                 && j >= 0
                 && k >= 0 ) {
                max = max > i + j + k ? max : i + j + k ;
                //printf("Max : %d\r\n", max);
            }
        }
    }

    return max;
}

// Driver code
int main()
{
    int n = 10, a = 2, b = 8, c = 5;
    printf ("Length of the ribbon: %d \r\nSegments : %d, %d, %d \r\n", n, a, b, c);
    printf("Maximum number of cuts : %d\r\n",maximumSegments(n, a, b, c));
    printf("Maximum number of cuts : %d\r\n",maxCut(n, a, b, c));
    return 0;
}