#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


/*
Coin Change

    Given a value N, if we want to make change for N cents, and we have infinite supply of each of S = { S1, S2, .. , Sm} valued coins, how many ways can we make the change? The order of coins doesn’t matter.
    For example, for N = 4 and S = {1,2,3}, there are four solutions: {1,1,1,1},{1,1,2},{2,2},{1,3}. So output should be 4. For N = 10 and S = {2, 5, 3, 6}, there are five solutions: {2,2,2,2,2}, {2,2,3,3}, {2,2,6}, {2,3,5} and {5,5}. So the output should be 5.

    1) Optimal Substructure
    To count the total number of solutions, we can divide all set solutions into two sets.
    1) Solutions that do not contain mth coin (or Sm).
    2) Solutions that contain at least one Sm.
    Let count(S[], m, n) be the function to count the number of solutions, then it can be written
    as sum of count(S[], m-1, n) and count(S[], m, n-Sm).
    Therefore, the problem has optimal substructure property as the problem can be solved using
    solutions to subproblems.

    2) Overlapping Subproblems
    Following is a simple recursive implementation of the Coin Change problem.
    The implementation simply follows the recursive structure mentioned above.

    3) Approach (Algorithm)

    See, here each coin of a given denomination can come an infinite number of times.
    (Repetition allowed), this is what we call UNBOUNDED KNAPSACK. We have 2 choices for a
    coin of a particular denomination, either i) to include, or ii) to exclude.  But here,
    the inclusion process is not for just once; we can include any denomination any number of
    times until N<0.

    Basically, If we are at s[m-1], we can take as many instances of that coin
    ( unbounded inclusion ) i.e count(S, m, n – S[m-1] ) ; then we move to s[m-2].
    After moving to s[m-2], we can’t move back and can’t make choices for s[m-1]
    i.e count(S, m-1, n ).

    Finally, as we have to find the total number of ways, so we will add these 2 possible choices,
    i.e count(S, m, n – S[m-1] ) + count(S, m-1, n ) ; which will be our required answer.

*/

// row,col
// row array length
// coloumn is sum value
int mem[100][100];

// Returns the count of ways we can
// sum S[0...m-1] coins to get sum n
int count( int S[], int m, int n)
{
    // If n is 0 then there is 1 solution
    // (do not include any coin)
    if (n == 0)
        return 1;

    // If n is less than 0 then no
    // solution exists
    if (n < 0)
        return 0;

    // If there are no coins and n
    // is greater than 0, then no
    // solution exist
    if (m <=0 && n >= 1)
        return 0;

    int a = count( S, m - 1, n );
    int b = count( S, m, n-S[m-1]);

    mem[m][n] =  a + b;
    // count is sum of solutions (i)
    // including S[m-1] (ii) excluding S[m-1]
    return mem[m][n];
}

// Driver program to test above function
int main()
{
    int i, j;
    int arr[] = {1, 2, 3};
    int m = sizeof(arr)/sizeof(arr[0]);
    memset(mem, -1, sizeof(mem));

    printf("There are %d ways\r\n", count(arr, m, 4));
    return 0;
}