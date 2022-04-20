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
// https://takeuforward.org/data-structure/minimum-maximum-falling-path-sum-dp-12/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999


int minSumUtilMemoization(int i, int j, int matrix[4][4],int n, int dp[4][4])
{
    // Base Conditions
    if (j < 0 || j >= n) {
        return INF;
    }

    if (0 == i) {
        return matrix[i][j];
    }

    if (-1 != dp[i][j]) {
        return dp[i][j];
    }

    int up = matrix[i][j] + minSumUtilMemoization (i-1, j, matrix, n, dp);
    int leftDiagonal = matrix[i][j] + minSumUtilMemoization (i-1, j-1, matrix, n, dp);
    int rightDiagonal = matrix[i][j] + minSumUtilMemoization (i-1, j+1, matrix, n, dp);

    dp[i][j] = MIN(up, MIN(leftDiagonal, rightDiagonal));
    return dp[i][j];
}

int maxSumUtilMemoization(int i, int j, int matrix[4][4],int n, int dp[4][4])
{
    // Base Conditions
    if (j < 0 || j >= n) {
        return -1;
    }

    if (0 == i) {
        return matrix[i][j];
    }

    if (-1 != dp[i][j]) {
        return dp[i][j];
    }

    int up = matrix[i][j] + maxSumUtilMemoization (i-1, j, matrix, n, dp);

    int leftDiagonal = matrix[i][j];
    if (j-1 >= 0) {
        leftDiagonal += maxSumUtilMemoization (i-1, j-1, matrix, n, dp);
    } else {
        leftDiagonal = -1;
    }

    int rightDiagonal = matrix[i][j];
    if (j+1 < n) {
        rightDiagonal += maxSumUtilMemoization (i-1, j+1, matrix, n, dp);
    } else {
        rightDiagonal = -1;
    }

    dp[i][j] = MAX(up, MAX(leftDiagonal, rightDiagonal));
    return dp[i][j];
}

int minSumMemoization(int matrix[4][4], int m, int n)
{
    int minimum = INF;
    int result = INF;
    int dp[4][4];
    memset(dp, -1, sizeof(dp));

    for (int i = 0; i < n-1; i++) {
        result = minSumUtilMemoization(m - 1, i, matrix, n, dp);
        minimum = MIN(result, minimum);
    }

    printf("DP output: \r\n");
    for(int i = 0; i < m ; i++) {
        for(int j = 0; j < n; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return minimum;
}

int maxSumMemoization(int matrix[4][4], int m, int n)
{
    int maximum = -1;
    int result = -1;
    int dp[4][4];
    memset(dp, -1, sizeof(dp));

    for (int i = 0; i < n-1; i++) {
        result = maxSumUtilMemoization(m - 1, i, matrix, n, dp);
        maximum = MAX(result, maximum);
    }

    printf("DP output: \r\n");
    for(int i = 0; i < m ; i++) {
        for(int j = 0; j < n; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return maximum;
}

int minSumUtilTabulation(int matrix[4][4], int m, int n)
{
    int minimum = INF;
    int dp[4][4];
    memset(dp, 0, sizeof(dp));

    for (int j = 0; j < n; j++) {
        dp[0][j] = matrix[0][j];
    }

    for(int i = 1; i < m ; i++) {
        for(int j = 0; j < n; j++) {

            int up = matrix[i][j] + dp[i-1][j];
            int leftDiagonal = matrix[i][j] + dp[i-1][j-1];
            int rightDiagonal = matrix[i][j] + dp[i-1][j+1];

            dp[i][j] = MIN(up, MIN(leftDiagonal, rightDiagonal));
        }
    }

    for(int j = 0; j < n; j++){
        minimum = MIN(minimum,dp[n-1][j]);
    }

    printf("DP output: \r\n");
    for(int i = 0; i < m ; i++) {
        for(int j = 0; j < n; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return minimum;
}

int maxSumUtilTabulation(int matrix[4][4], int m, int n)
{
    int maximum = -1;
    int dp[4][4];
    memset(dp, 0, sizeof(dp));

    for (int j = 0; j < n; j++) {
        dp[0][j] = matrix[0][j];
    }

    for(int i = 1; i < m ; i++) {
        for(int j = 0; j < n; j++) {

            int up = matrix[i][j] + dp[i-1][j];

            int leftDiagonal = matrix[i][j];
            if (j-1 >= 0) {
                leftDiagonal += dp[i-1][j-1];
            } else {
                leftDiagonal = -1;
            }

            int rightDiagonal = matrix[i][j];
            if (j+1 < n) {
                rightDiagonal += dp[i-1][j+1];
            } else {
                rightDiagonal = -1;
            }

            dp[i][j] = MAX(up, MAX(leftDiagonal, rightDiagonal));
        }
    }

    for(int j = 0; j < n; j++){
        maximum = MAX(maximum,dp[n-1][j]);
    }

    printf("DP output: \r\n");
    for(int i = 0; i < m ; i++) {
        for(int j = 0; j < n; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return maximum;
}

int minSumTabulation(int matrix[4][4], int m, int n)
{
    return minSumUtilTabulation(matrix, m, n);
}

int maxSumTabulation(int matrix[4][4], int m, int n)
{
    return maxSumUtilTabulation(matrix, m, n);
}

int minSumOptimization(int matrix[4][4], int m, int n)
{
    int minimum = INF;
    int prev[n];
    memset(prev, 0, sizeof(prev));

    int cur[n];
    memset(cur, 0, sizeof(cur));

    for (int j = 0; j < n; j++) {
        prev[j] = matrix[0][j];
    }

    for(int i = 1; i < m ; i++) {
        for(int j = 0; j < n; j++) {

            int up = matrix[i][j] + prev[j];

            int leftDiagonal = matrix[i][j];
            if (j-1 >= 0) {
                leftDiagonal += prev[j-1];
            } else {
                leftDiagonal = INF;
            }

            int rightDiagonal = matrix[i][j];
            if (j+1 < n) {
                rightDiagonal += prev[j+1];
            } else {
                rightDiagonal = INF;
            }

            cur[j] = MIN(up, MIN(leftDiagonal, rightDiagonal));
        }

        for (int k = 0; k < n; k++) {

            prev[k] = cur[k];

            printf("%4d", prev[k]);
        }
        printf("\r\n");
    }

    for(int j = 0; j < n; j++){
        minimum = MIN(minimum,prev[j]);
    }

    return minimum;
}

int maxSumOptimization(int matrix[4][4], int m, int n)
{
    int maximum = -1;
    int prev[n];
    memset(prev, 0, sizeof(prev));

    int cur[n];
    memset(cur, 0, sizeof(cur));

    for (int j = 0; j < n; j++) {
        prev[j] = matrix[0][j];
    }

    for(int i = 1; i < m ; i++) {
        for(int j = 0; j < n; j++) {

            int up = matrix[i][j] + prev[j];

            int leftDiagonal = matrix[i][j];
            if (j-1 >= 0) {
                leftDiagonal += prev[j-1];
            } else {
                leftDiagonal = -1;
            }

            int rightDiagonal = matrix[i][j];
            if (j+1 < n) {
                rightDiagonal += prev[j+1];
            } else {
                rightDiagonal = -1;
            }

            cur[j] = MAX(up, MAX(leftDiagonal, rightDiagonal));
        }

        for (int k = 0; k < n; k++) {

            prev[k] = cur[k];

            printf("%4d", prev[k]);
        }
        printf("\r\n");
    }

    for(int j = 0; j < n; j++){
        maximum = MAX(maximum, prev[j]);
    }

    return maximum;
}

int main()
{
    int matrix[4][4] = {{1,     2,      10,     4},
                        {100,   3,       2,     1},
                        {1,     1,      20,     2},
                        {1,     2,      2,      1}};

    int m=4;
    int n=4;

    printf("[Memoization] The Minimum sum from first row to last row  : %d \r\n\r\n",   minSumMemoization(matrix, m, n));
    printf("[Tabulation]  The Minimum sum from first row to last row  : %d \r\n\r\n",   minSumTabulation(matrix, m, n));
    printf("[Optimization]  The Minimum sum from first row to last row  : %d \r\n\r\n", minSumOptimization(matrix, m, n));

    printf("[Memoization] The Maximum sum from first row to last row  : %d \r\n\r\n",   maxSumMemoization(matrix, m, n));
    printf("[Tabulation]  The Maximum sum from first row to last row  : %d \r\n\r\n",   maxSumTabulation(matrix, m, n));
    printf("[Optimization]  The Maximum sum from first row to last row  : %d \r\n\r\n", maxSumOptimization(matrix, m, n));

    return 0;
}
