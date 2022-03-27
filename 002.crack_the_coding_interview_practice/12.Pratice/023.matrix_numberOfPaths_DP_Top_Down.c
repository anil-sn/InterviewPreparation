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

// Returns count of possible paths to reach cell at
// row number m and column  number n from the topmost
// leftmost cell (cell at 1, 1)
int numberOfPaths(int n, int m, int DP[4][4])
{
    if(n == 1 || m == 1) {
        return DP[n][m] = 1;
    }

    // Add the element in the DP table
    // If it is was not computed before
    if(DP[n][m] == 0) {
        DP[n][m] = numberOfPaths(n - 1, m,DP) + numberOfPaths(n, m - 1,DP);
    }

    return DP[n][m];
}

int main()
{
    // Create an empty 2D table
    int DP[4][4] = {0};
    memset(DP, 0, sizeof(DP));

    printf("Number of possible paths (4, 4) is %d\r\n", numberOfPaths(3, 3, DP));
    return 0;
}