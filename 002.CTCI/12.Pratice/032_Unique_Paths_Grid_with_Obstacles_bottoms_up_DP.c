#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define ROW_1 3
#define COL_1 3

int printAllPathsWithObstacles(int A[ROW_1][COL_1])
{
    int r = ROW_1;
    int c = COL_1;
    int paths[ROW_1][COL_1];
    memset(paths, 0, sizeof(int)*ROW_1*COL_1);

    // Initializing the left corner if
    // no obstacle there
    if (A[0][0] == 0)
        paths[0][0] = 1;

    // Initializing first column of
    // the 2D matrix
    for(int i = 1; i < r; i++) {

        // If not obstacle
        if (A[i][0] == 0)
            paths[i][0] = paths[i-1][0];
    }

    // Initializing first row of the 2D matrix
    for(int j = 1; j < c; j++) {

        // If not obstacle
        if (A[0][j] == 0)
            paths[0][j] = paths[0][j - 1];
    }

    for(int i = 1; i < r; i++) {
        for(int j = 1; j < c; j++) {

            // If current cell is not obstacle
            if (A[i][j] == 0)
                paths[i][j] = paths[i - 1][j] + paths[i][j - 1];
        }
    }

    // Returning the corner value
    // of the matrix
    return paths[r - 1][c -1];
}
// Driver program to test above functions
int main()
{
    int obt[ROW_1][COL_1] = {{0,0,0}, {0,1,0},{0,0,0}};

    printf("Number of unique path with Obstacle is : %d\r\n", printAllPathsWithObstacles(obt));
    return 0;
}