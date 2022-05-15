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
// https://takeuforward.org/data-structure/minimum-path-sum-in-a-grid-dp-10/


#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int minSumUtilMemoization(int i, int j, int matrix[3][3], int dp[3][3])
{
    if(i == 0 && j == 0) {
        return matrix[0][0];
    }

    if(i < 0 || j < 0){
        return INF;
    }

    if(dp[i][j] != -1) {
        return dp[i][j];
    }

    int up   = matrix[i][j] + minSumUtilMemoization(i-1, j,   matrix, dp);
    int left = matrix[i][j] + minSumUtilMemoization(i,   j-1, matrix, dp);

    dp[i][j] = MIN(up, left);

    printf("The Minimum sum from 0,0 to %d,%d : %d \r\n", i, j, dp[i][j]);
    return dp[i][j];
}

int minSumMemoization(int m, int n, int matrix[3][3])
{
    int dp[m][n];

    memset(dp, -1, sizeof(dp));

    return minSumUtilMemoization(m-1, n-1, matrix, dp);
}

int minSumUtilTabulation(int m, int n, int matrix[3][3], int dp[3][3])
{

    for(int i=0; i<m ;i++){
        for(int j=0; j<n; j++){

            //base condition
            if(i == 0 && j == 0){
                dp[i][j] = matrix[i][j];
                continue;
            }

            int up = matrix[i][j];
            int left = matrix[i][j];

            if(i > 0) {
                up += dp[i-1][j];
            } else {
                up = INF;
            }

            if(j > 0) {
                left += dp[i][j-1];
            } else {
                left = INF;
            }

            dp[i][j] = MIN(up, left);
            printf("The Minimum sum from 0,0 to %d,%d : %d \r\n", i, j, dp[i][j]);
        }
    }

    return dp[m-1][n-1];
}

int minSumTabulation(int m, int n, int matrix[3][3])
{
    int dp[m][n];
    memset(dp, -1, sizeof(dp));

    return minSumUtilTabulation(m, n, matrix, dp);

}

int minSumOptimization(int m, int n, int matrix[3][3])
{
    int prev[n];
    memset(prev, 0, sizeof(prev));

    int temp[n];
    memset(temp, 0, sizeof(temp));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if(i == 0 && j == 0) {
                temp[j] = matrix[i][j];
                continue;
            }

            int up = matrix[i][j];
            int left = matrix[i][j];

            if(i > 0) {
                up += prev[j];
            } else {
                up = INF;
            }

            if(j > 0) {
                left += temp[j-1];
            } else {
                left = INF;
            }

            temp[j] = MIN(up, left);
            printf("The Minimum sum from 0,0 to %d,%d : %d \r\n", i, j, temp[j]);
        }

        for (int k = 0; k < n; k++) {
            prev[k] = temp[k];
        }
    }

    return prev[n-1];
}

int main()
{
    int matrix[3][3] = {{1,  2,  3},
                        {3,  2,  1},
                        {1,  2,  3}};

    int m=3;
    int n=3;

    printf("[Memoization] The Minimum sum from 0,0 to %d,%d : %d \r\n", m-1, n-1, minSumMemoization(m,n, matrix));
    printf("[Tabulation] The Minimum sum from 0,0 to %d,%d : %d \r\n", m-1, n-1, minSumTabulation(m,n, matrix));
    printf("[Optimization] Minimum sum from 0,0 to %d,%d : %d \r\n", m-1, n-1, minSumOptimization(m,n, matrix));
}