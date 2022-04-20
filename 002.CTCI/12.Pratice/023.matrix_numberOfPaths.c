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

// Returns count of possible paths to reach cell at row
// number m and column number n from the topmost leftmost
// cell (cell at 1, 1)
int numberOfPaths(int m, int n)
{
    // If either given row number is first or given column
    // number is first
    if (m == 1 || n == 1) {
        return 1;
    }

    // If diagonal movements are allowed then the last
    // addition is required.
    return numberOfPaths(m - 1, n) + numberOfPaths(m, n - 1);
    // + numberOfPaths(m-1, n-1);
}

int main()
{
    printf("Number of possible paths (3, 3) is %d\r\n", numberOfPaths(3, 3));
    return 0;
}