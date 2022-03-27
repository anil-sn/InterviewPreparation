#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define ROW 2
#define COL 3

#define ROW_1 3
#define COL_1 3
/* mat:  Pointer to the starting of mXn matrix
   i, j: Current position of the robot (For the first call use 0,0)
   m, n: Dimensions of given the matrix
   pi:   Next index to be filed in path array
   *path[0..pi-1]: The path traversed by robot till now (Array to hold the
                  path need to have space for at least m+n elements) */
void printAllPathsWithOutObstaclesUtil(int *mat,
                                        int i,
                                        int j,
                                        int m,
                                        int n,
                                        int *path,
                                        int pi)
{
    // Reached the bottom of the matrix so we are left with
    // only option to move right
    if (i == m - 1)
    {
        for (int k = j; k < n; k++)
            path[pi + k - j] = *((mat + i*n) + k);

        for (int l = 0; l < pi + n - j; l++)
            printf("%d ",path[l]);

        printf("\r\n");
        return;
    }

    // Reached the right corner of the matrix we are left with
    // only the downward movement.
    if (j == n - 1)
    {
        for (int k = i; k < m; k++)
            path[pi + k - i] = *((mat + k*n) + j);

        for (int l = 0; l < pi + m - i; l++)
            printf("%d ",path[l]);

        printf("\r\n");
        return;
    }

    // Add the current cell to the path being generated
    path[pi] = *((mat + i*n) + j);

    // Print all printAllPathsWithOutObstaclesUtil paths that are possible after moving down
    printAllPathsWithOutObstaclesUtil(mat, i+1, j, m, n, path, pi + 1);

    // Print all the paths that are possible after moving right
    printAllPathsWithOutObstaclesUtil(mat, i, j+1, m, n, path, pi + 1);

    // Print all the paths that are possible after moving diagonal
    printAllPathsWithOutObstaclesUtil(mat, i+1, j+1, m, n, path, pi + 1);
}

// The main function that prints all paths from top left to bottom right
// in a matrix 'mat' of size mXn
void printAllPathsWithOutObstacles(int *mat, int m, int n)
{
    int path[m][n];
    memset(path, 0, sizeof(int)*m*n);
    printAllPathsWithOutObstaclesUtil((int *)mat, 0, 0, m, n, (int *)path, 0);
}

int  UniquePathHelper(int i, int j, int r, int c, int A[ROW_1][COL_1]){
    // boundary condition or constraints
    if(i == r || j == c){
      return 0 ;
    }

    if(A[i][j] == 1){
      return 0 ;
    }

    // base case
    if(i == r-1 && j == c-1){
      return 1 ;
    }

    return  UniquePathHelper(i+1, j, r, c, A) +
            UniquePathHelper(i, j+1, r, c, A) ;
}

int printAllPathsWithObstacles(int A[ROW_1][COL_1], int m, int n)
{
    int r = m;
    int c = n;
    return UniquePathHelper(0, 0, r, c, A) ;
}
// Driver program to test above functions
int main()
{
    int mat[ROW][COL] = { {1, 2, 3}, {4, 5, 6} };
    int obt[ROW_1][COL_1] = {{0,0,0}, {0,1,0},{0,0,0}};

    printf("Matrix = \r\n 1\t 2\t 3\r\n 4\t 5\t 6\r\n");
    printAllPathsWithOutObstacles((int *)mat, ROW, COL);

    printf("Number of unique path with Obstacle is : %d\r\n", printAllPathsWithObstacles(obt, ROW_1, COL_1));
    return 0;
}