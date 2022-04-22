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
// https://youtu.be/vRVfmbCFW7Y

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int mcmUtil(int i, int j, int n, int arr[])
{
    if (i == j){
        return 0;
    }

    int mini = 1e8;
    int steps = 0;
    for (int k = i; k < j; k++){

        steps = (arr[i-1] * arr[k] * arr[j]) +
                mcmUtil(i, k, n, arr) +
                mcmUtil(k+1, j, n, arr);

        if (steps < mini){
            mini = steps;
        }
    }

    return mini;
}

int matrixMultiplication(int n, int arr[])
{
    return mcmUtil(1, n-1, n, arr);
}

int mcmUtilMemoization(int i, int j, int n, int arr[], int row, int col, int dp[row][col])
{
    if (i == j){
        return 0;
    }

    if (-1 != dp[i][j]) {
        return dp[i][j];
    }

    int mini = 1e8;
    int steps = 0;
    for (int k = i; k < j; k++){

        steps = (arr[i-1] * arr[k] * arr[j]) +
                mcmUtilMemoization(i, k, n, arr, row, col, dp) +
                mcmUtilMemoization(k+1, j, n, arr, row, col, dp);

        if (steps < mini){
            mini = steps;
        }
    }

    dp[i][j] = mini;
    return dp[i][j];
}

int matrixMultiplicationMemoization(int n, int arr[])
{
    int dp[n+1][n+1];
    memset(dp, -1, sizeof(dp));

    return mcmUtilMemoization(1, n-1, n, arr, n+1, n+1, dp);
}

int matrixMultiplicationTabulation(int n, int arr[])
{
    int dp[n][n];
    memset(dp, 0, sizeof(dp));

    for (int i = n-1; i >= 1; i--){
        for (int j = i+1; j < n; j++){

            int mini = 1e8;
            int steps = 0;
            for (int k = i; k < j; k++){

                steps = (arr[i-1] * arr[k] * arr[j])
                        + dp[i][k]
                        + dp[k+1][j];

                if (steps < mini){
                    mini = steps;
                }
            }
            dp[i][j] = mini;
        }
    }

    return dp[1][n-1];
}

int main ()
{
    int arr[] = {4, 5, 3, 2};
    int n = SIZEOF(arr);
    int result = 0;

    result = matrixMultiplication(n, arr);
    printf("[recursive] The minimum number of multiplication need is : %d \r\n", result);

    result = matrixMultiplicationMemoization(n, arr);
    printf("[memoization] The minimum number of multiplication need is : %d \r\n", result);

    result = matrixMultiplicationTabulation(n, arr);
    printf("[tabulation] The minimum number of multiplication need is : %d \r\n", result);

    return 0;
}