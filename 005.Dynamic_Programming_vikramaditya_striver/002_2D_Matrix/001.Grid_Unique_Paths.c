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
// https://takeuforward.org/data-structure/grid-unique-paths-dp-on-grids-dp8/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)

int countWaysUtilMemoization(int i, int j, int dp[3][3])
{
    if(i == 0 && j == 0) {
        return 1;
    }

    if(i < 0 || j < 0){
        return 0;
    }

    if(dp[i][j] != -1) {
        return dp[i][j];
    }

    int up   = countWaysUtilMemoization(i-1, j,   dp);
    int left = countWaysUtilMemoization(i,   j-1, dp);

    dp[i][j] = up+left;

    printf("The Number of unique ways from 0,0 to %d,%d : %d \r\n", i, j, dp[i][j]);
    return dp[i][j];
}

int countWaysMemoization(int m, int n)
{
    int dp[m][n];

    memset(dp, -1, sizeof(dp));

    return countWaysUtilMemoization(m-1, n-1, dp);
}

int countWaysUtilTabulation(int m, int n, int dp[3][3])
{

    for(int i=0; i<m ;i++){
        for(int j=0; j<n; j++){

            //base condition
            if(i==0 && j==0){
                dp[i][j]=1;
                continue;
            }

            int up =0;
            int left = 0;

            if(i > 0) {
                up = dp[i-1][j];
            }

            if(j > 0) {
                left = dp[i][j-1];
            }

            dp[i][j] = up + left;
            printf("The Number of unique ways from 0,0 to %d,%d : %d \r\n", i, j, dp[i][j]);
        }
    }

    return dp[m-1][n-1];
}

int countWaysTabulation(int m, int n)
{
    int dp[m][n];
    memset(dp, -1, sizeof(dp));

    return countWaysUtilTabulation(m, n, dp);

}

int countWaysOptimization(int m, int n)
{
    int prev[n];
    memset(prev, 0, sizeof(prev));

    int temp[n];
    memset(temp, 0, sizeof(temp));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if(i == 0 && j == 0) {
                temp[j]=1;
                continue;
            }

            int up = 0;
            int left = 0;

            if(i>0)
                up = prev[j];
            if(j>0)
                left = temp[j-1];

            temp[j] = up + left;
            printf("The Number of unique ways from 0,0 to %d,%d : %d \r\n", i, j, temp[j]);
        }

        for (int k = 0; k < n; k++) {
            prev[k] = temp[k];
        }
    }

    return prev[n-1];
}

int main()
{
    int m=3;
    int n=3;

    printf("[Memoization] The Number of unique ways from 0,0 to %d,%d : %d \r\n", m-1, n-1, countWaysMemoization(m,n));
    printf("[Tabulation] The Number of unique ways from 0,0 to %d,%d : %d \r\n", m-1, n-1, countWaysTabulation(m,n));
    printf("[Optimization] The Number of unique ways from 0,0 to %d,%d : %d \r\n", m-1, n-1, countWaysOptimization(m,n));
}