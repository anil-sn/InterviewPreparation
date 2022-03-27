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
    Minimum Cost Path
        Given a square grid of size N, each cell of which contains integer cost which represents a cost to traverse through that cell, we need to find a path from top left cell to bottom right cell by which the total cost incurred is minimum.
        From the cell (i,j) we can go (i,j-1), (i, j+1), (i-1, j), (i+1, j).

        Note: It is assumed that negative cost cycles do not exist in the input matrix.

        Example 1:
            Input: grid = {{9,4,9,9},{6,7,6,4}, {8,3,3,7},{7,4,9,10}}
            Output: 43
            Explanation: The grid is-
                9  4  9  9
                6  7  6  4
                8  3  3  7
                7  4  9 10
                The minimum cost is-
                9 + 4 + 7 + 3 + 3 + 7 + 10 = 43.

        Example 2:
            Input: grid = {{4,4},{3,7}}
            Output: 14
            Explanation: The grid is-
                4 4
                3 7
                The minimum cost is- 4 + 3 + 7 = 14.
*/

int minCost(int cost[3][3], int row, int col) {

    // for 1st column
    for (int i=1 ; i<row ; i++){
        cost[i][0] += cost[i-1][0];
    }

    // for 1st row
    for (int j=1 ; j<col ; j++){
        cost[0][j] += cost[0][j-1];
    }

    // for rest of the 2d matrix
    for (int i=1 ; i<row ; i++) {
        for (int j=1 ; j<col ; j++) {
            cost[i][j] += min(cost[i-1][j-1], min(cost[i-1][j], cost[i][j-1]));
        }
    }

    for (int i=0 ; i<row ; i++) {
        for (int j=0 ; j<col ; j++) {
            printf("%3d", cost[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    // returning the value in last cell
    return cost[row-1][col-1];
}
int main(int argc, char const *argv[])
{
    int row = 3;
    int col = 3;

    int cost[3][3] = {  {1, 2, 3},
                              {4, 8, 2},
                              {1, 5, 3} };

    for (int i=0 ; i<row ; i++) {
        for (int j=0 ; j<col ; j++) {
            printf("%3d", cost[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    printf("Minimum cost to reach end is : %d\r\n",minCost(cost, row, col));
    return 0;
}