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
    Grid Unique Paths | Count paths from left-top to the right bottom of a matrix
    Problem Statement: Given a matrix m X n, count paths from left-top to the right
    bottom of a matrix with the constraints that from each cell you can either only
    move to the rightward direction or the downward direction.

    Example 1:

        Input Format: m = 2, n= 2
        Output: 2
        Explanation: From the top left corner there are total 2 ways to reach
                     the bottom right corner:

        Step 1: Right ->> Down
        Step 2: Down ->> Right

    Example 2:

        Input Format: m = 2, n= 3
        Ouput: 3
        Explanation:  From the top left corner there is a total of 3 ways to
                      reach the bottom right corner.

        Step 1: Right ->> Right ->> Down
        Step 2: Right ->> Down ->> Right
        Step 3: Down ->> Right->> Right
*/


int countPaths(int i,int j,int n,int m)
{
    if((i == (n-1)) && (j == (m-1))) {
        return 1;
    }

    if(i >= n || j >= m) {
        return 0;
    } else {
        return countPaths(i+1, j, n, m) + countPaths(i, j+1, n, m);
    }
}

int uniquePaths(int m, int n) {
    return countPaths(0,0,m,n);
}

int main()
{
    int totalCount= uniquePaths(3,3);
    printf("The total number of Unique Paths are %d \r\n", totalCount);
}