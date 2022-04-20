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

    // If obstacle is at starting position
    if (A[0][0])
        return 0;

    //  Initializing starting position
    A[0][0] = 1;

    // first row all are '1' until obstacle
    for (int j = 1; j < c; j++) {

        if (A[0][j] == 0) {
            A[0][j] = A[0][j - 1];
        }
        else {
            // No ways to reach at this index
            A[0][j] = 0;
        }
    }

    // first column all are '1' until obstacle
    for (int i = 1; i < r; i++) {

        if (A[i][0] == 0) {
            A[i][0] = A[i - 1][0];
        }
        else {
            // No ways to reach at this index
            A[i][0] = 0;
        }
    }

    for (int i = 1; i < r; i++) {

        for (int j = 1; j < c; j++) {
            // If current cell has no obstacle
            if (A[i][j] == 0) {

                A[i][j] = A[i - 1][j] + A[i][j - 1];
            }
            else {
                // No ways to reach at this index
                A[i][j] = 0;
            }
        }
    }

    // returning the bottom right
    // corner of Grid
    return A[r - 1][c -1];
}
// Driver program to test above functions
int main()
{
    int obt[ROW_1][COL_1] = {{0,0,0}, {0,1,0},{0,0,0}};

    printf("Number of unique path with Obstacle is : %d\r\n", printAllPathsWithObstacles(obt));
    return 0;
}