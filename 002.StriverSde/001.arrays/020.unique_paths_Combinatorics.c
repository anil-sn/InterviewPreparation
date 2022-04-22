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

int uniquePaths(int m, int n)
{
    int N = n + m - 2;
    int r = m - 1;
    double res = 1;

    for (int i = 1; i <= r; i++) {
        res = res * (N - r + i) / i;
    }
    return (int)res;
}

int main()
{
    int totalCount= uniquePaths(3,3);
    printf("The total number of Unique Paths are %d \r\n", totalCount);
}