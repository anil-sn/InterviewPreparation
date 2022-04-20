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
    Dont practice until you get it right
    practice until you can't get it worng
*/

/*
    Count all possible paths from top left to bottom right of a mXn matrix

    The problem is to count all the possible paths from top left to bottom right of a mXn matrix
    with the constraints that from each cell you can either move only to right or down

    Examples :

        Input :  m = 2, n = 2;
        Output : 2
            There are two paths
            (0, 0) -> (0, 1) -> (1, 1)
            (0, 0) -> (1, 0) -> (1, 1)

        Input :  m = 2, n = 3;
        Output : 3
            There are three paths
            (0, 0) -> (0, 1) -> (0, 2) -> (1, 2)
            (0, 0) -> (0, 1) -> (1, 1) -> (1, 2)
            (0, 0) -> (1, 0) -> (1, 1) -> (1, 2)
*/

// Returns count of possible paths to reach
// cell at row number m and column number n from
// the topmost leftmost cell (cell at 1, 1)
int numberOfPaths(int m, int n)
{
    // Create a 1D array to store results of subproblems
    int dp[n];
    memset(dp, 0, sizeof(int)*n);
    dp[0] = 1;

    printf("Initilization :  \r\n");
    for (int i = 0; i < n; i++) {
        printf("%2d ", dp[i]);
    }
    printf("\r\n");

    for (int i = 0; i < m; i++) {
        for (int j = 1; j < n; j++) {
            printf("dp[%d]:%d = dp[%d]:%d + dp[%d-1]:%d\r\n", j, dp[j], j, dp[j], j, dp[j-1]);
            dp[j] += dp[j - 1];
        }
    }

    printf("Result :  \r\n");
    for (int i = 0; i < n; i++) {
        printf("%2d ", dp[i]);
    }
    printf("\r\n");

    return dp[n - 1];
}

int main()
{
    printf("Number of possible paths (3, 3) is %d\r\n", numberOfPaths(3, 3));
    return 0;
}