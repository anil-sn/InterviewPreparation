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

int  UniquePathHelper(int i, int j, int r, int c, int A[ROW_1][COL_1], int paths[ROW_1][COL_1]){
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

    if (paths[i][j] != -1) {
        return paths[i][j];
    }

    return UniquePathHelper(i + 1, j, r, c, A, paths)
           + UniquePathHelper(i, j + 1, r, c, A, paths);
}

int printAllPathsWithObstacles(int A[ROW_1][COL_1], int m, int n)
{
    int r = m;
    int c = n;
    int paths[m][n];
    memset(paths, -1, sizeof(int)*m*n);

    return UniquePathHelper(0, 0, r, c, A, paths) ;
}
// Driver program to test above functions
int main()
{
    int obt[ROW_1][COL_1] = {{0,0,0}, {0,1,0},{0,0,0}};

    printf("Number of unique path with Obstacle is : %d\r\n", printAllPathsWithObstacles(obt, ROW_1, COL_1));
    return 0;
}