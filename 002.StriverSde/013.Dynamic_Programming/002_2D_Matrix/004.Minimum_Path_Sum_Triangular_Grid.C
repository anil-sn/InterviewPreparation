#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// https://takeuforward.org/dynamic-programming/striver-dp-series-dynamic-programming-problems/
// https://takeuforward.org/data-structure/minimum-path-sum-in-triangular-grid-dp-11/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int minSumUtilMemoization(int i, int j, int triangle[4][4],int n, int dp[4][4])
{

    if(i == n-1) {
        return triangle[i][j];
    }

    int down = triangle[i][j] + minSumUtilMemoization (i+1, j, triangle, n, dp);
    int diagonal = triangle[i][j] + minSumUtilMemoization (i+1, j+1, triangle, n, dp);

    dp[i][j] = MIN(down, diagonal);
    printf("The Minimum sum in triangular grid from 0,0 to Last Row %d : %d \r\n", i, dp[i][j]);

    return dp[i][j];
}

int minSumMemoization(int triangle[4][4], int n)
{
    int dp[4][4];
    memset(dp, -1, sizeof(dp));

    return minSumUtilMemoization(0, 0, triangle, n, dp);
}

int minSumUtilTabulation(int triangle[4][4], int n)
{
    int dp[4][4];
    memset(dp, 0, sizeof(dp));

    for (int j = 0; j < n; j++) {
        dp[n-1][j] = triangle[n-1][j];
    }

    for(int i = n-2; i >= 0 ; i--) {
        for(int j = i; j >= 0; j--) {

            int down = triangle[i][j] + dp[i+1][j];
            int diagonal = triangle[i][j] + dp[i+1][j+1];

            dp[i][j] = MIN(down, diagonal);
        }
    }

    return dp[0][0];
}

int minSumTabulation(int triangle[4][4], int n)
{
    return minSumUtilTabulation(triangle, n);

}

int minSumOptimization(int triangle[4][4], int n)
{
    int front[n];
    memset(front, 0, sizeof(front));

    int cur[n];
    memset(cur, 0, sizeof(cur));

    for (int j = 0; j < n; j++) {
        front[j] = triangle[n-1][j];
    }

    for(int i = n-2; i >= 0 ; i--) {
        for(int j = i; j >= 0; j--) {

            int down = triangle[i][j] + front[j];
            int diagonal = triangle[i][j] + front[j+1];

            cur[j] = MIN(down, diagonal);
        }

        for (int k = 0; k < n; k++) {
            front[k] = cur[k];
        }
    }

    return front[0];
}

int main()
{
    int matrix[4][4] = {{1,  0,  0,  0},
                        {2,  3,  0,  0},
                        {3,  6,  7,  0},
                        {8,  9,  6, 10}};

    int m=4;
    int n=4;

    printf("[Memoization] The Minimum sum in triangular grid from 0,0 to Last Row %d : %d \r\n", m-1, minSumMemoization(matrix, m));
    printf("[Tabulation]  The Minimum sum in triangular grid from 0,0 to Last Row %d : %d \r\n", m-1, minSumTabulation(matrix, m));
    printf("[Optimization]  The Minimum sum in triangular grid from 0,0 to Last Row %d : %d \r\n", m-1, minSumOptimization(matrix, m));
}