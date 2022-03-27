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
int numberOfPaths(int m, int n)
{
    // Create a 2D table to store results of subproblems
    int count[m][n];

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            count[i][j] = 0;
        }
    }

    // Count of paths to reach any cell in first column is 1
    for (int i = 0; i < m; i++)
        count[i][0] = 1;

    // Count of paths to reach any cell in first row is 1
    for (int j = 0; j < n; j++)
        count[0][j] = 1;

    printf("Initilization :  \r\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%2d ", count[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    // Calculate count of paths for other cells in
    // bottom-up manner using the recursive solution
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {

            // By uncommenting the last part the code calculates the total
            // possible paths if the diagonal Movements are allowed
            count[i][j] = count[i - 1][j] + count[i][j - 1]; //+ count[i-1][j-1];
            printf ("count[%d][%d]:%d = count[%d - 1][%d]:%d + count[%d][%d - 1]:%d\r\n",
                    i,j,i,j,i,j,
                    count[i][j], count[i - 1][j], count[i][j - 1]);
        }

        printf("\r\nIntermediate Result:  \r\n");
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                printf("%2d ", count[i][j]);
            }
            printf("\r\n");
        }
        printf("\r\n");
    }

    printf("Final Result :  \r\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%2d ", count[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return count[m - 1][n - 1];
}

int main()
{
    printf("Number of possible paths (4, 4) is %d\r\n", numberOfPaths(4, 4));
    return 0;
}