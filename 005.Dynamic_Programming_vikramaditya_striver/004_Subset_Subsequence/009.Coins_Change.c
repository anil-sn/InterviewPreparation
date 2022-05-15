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
// https://takeuforward.org/data-structure/coin-change-2-dp-22/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int countWaysToMakeChangeUtilMemoization(int ind, int n, int arr[n], int target, int row, int col, int dp[row][col])
{
    if (0 == ind) {
        return (0 == target % arr[ind]);
    }

    if (-1 != dp[ind][target]){
        return dp[ind][target];
    }

    int notPick = countWaysToMakeChangeUtilMemoization((ind - 1), n, arr, target, row, col, dp);
    int pick = 0;
    if (arr[ind] <= target) {
        pick = countWaysToMakeChangeUtilMemoization(ind, n, arr, target - arr[ind], row, col, dp);
    }

    dp[ind][target] = pick  + notPick;
    //printf("ind %d, target %d : %d \r\n", ind, target, dp[ind][target]);
    return dp[ind][target];
}

int countWaysToMakeChangeMemoization(int n, int arr[n], int target)
{
    int dp[n][target+1];
    memset(dp, -1, sizeof(dp));

    int ans =  countWaysToMakeChangeUtilMemoization(n-1, n, arr, target, n, (target + 1), dp);

    printf("DP Output: \r\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= target; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return ans;
}

int countWaysToMakeChangeTabulation(int n, int arr[n], int target)
{
    int dp[n][target+1];
    memset(dp, 0, sizeof(dp));

    for(int i = 0; i <= target; i++){
        if(i % arr[0] == 0) {
            dp[0][i] = 1;
        }
    }

    for (int ind = 1; ind < n; ind++){
        for (int tar = 0; tar <= target; tar++) {

            int notPick = dp[ind - 1][tar];
            int pick = 0;
            if (arr[ind] <= tar) {
                pick = dp[ind][tar - arr[ind]];
            }

            dp[ind][tar] = pick + notPick;
            //printf("ind %d, target %d : %d \r\n", ind, tar, dp[ind][tar]);
        }
    }

    printf("DP Output: \r\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= target; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return dp[n-1][target];
}

int countWaysToMakeChangeOptimization(int n, int arr[n], int target)
{
    int prev[target+1];
    memset(prev, 0, sizeof(prev));

    int cur[target+1];
    memset(prev, 0, sizeof(prev));

    for(int i = 0; i <= target; i++){
        if(i % arr[0] == 0) {
            prev[i] = 1;
        }
    }

    printf("DP Output: \r\n");
    for (int j = 0; j <= target; j++) {
        printf("%4d", prev[j]);
    }
    printf("\r\n");

    for (int ind = 1; ind < n; ind++){
        for (int tar = 0; tar <= target; tar++) {

            int notPick = prev[tar];
            int pick = 0;
            if (arr[ind] <= tar) {
                pick = cur[tar - arr[ind]];
            }

            cur[tar] = pick + notPick;
            //printf("ind %d, target %d : %d \r\n", ind, tar, dp[ind][tar]);
        }

        for(int j = 0; j <= target; j++) {
            prev[j] = cur[j];
            printf("%4d", prev[j]);
        }
        printf("\r\n");
    }

    return prev[target];
}

int main() {

    int arr[] = {1,2,3};
    int n = SIZEOF(arr);
    int target = 4;
    int ways = 0;

    ways = countWaysToMakeChangeMemoization(n, arr, target);
    printf("[memoization] The minimum number of ways required to form the target sum is %d \r\n", ways);

    ways = countWaysToMakeChangeTabulation(n, arr, target);
    printf("[tabulation] The minimum number of ways required to form the target sum is %d \r\n", ways);

    ways = countWaysToMakeChangeOptimization(n, arr, target);
    printf("[optimization] The minimum number of ways required to form the target sum is %d \r\n", ways);

    return 0;
}