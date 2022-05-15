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
// https://takeuforward.org/data-structure/minimum-coins-dp-20/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int minimumElementsUtilMemoization(int ind, int n, int arr[n], int target, int dp[n][target+1])
{
    if (0 == ind) {
        if (0 == target % arr[ind]){
            return (target/arr[ind]);
        } else {
            return 1e8;
        }
    }

    if (-1 != dp[ind][target]){
        return dp[ind][target];
    }

    int notPick = 0 + minimumElementsUtilMemoization((ind - 1), n, arr, target, dp);
    int pick = 1e8;
    if (arr[ind] <= target) {
        pick = 1 + minimumElementsUtilMemoization(ind, n, arr, target - arr[ind], dp);
    }

    dp[ind][target] = MIN(pick, notPick);
    //printf("ind %d, target %d : %d \r\n", ind, target, dp[ind][target]);
    return dp[ind][target];
}

int minimumElementsMemoization(int n, int arr[n], int target)
{
    int dp[n][target+1];
    memset(dp, -1, sizeof(dp));

    int ans =  minimumElementsUtilMemoization(n-1, n, arr, target, dp);

    if(ans >= 1e9) {
        return -1;
    }

    return ans;
}

int minimumElementsTabulation(int n, int arr[n], int target)
{
    int dp[n][target+1];
    memset(dp, 0, sizeof(dp));

    for(int i = 0; i <= target; i++){
        if(i % arr[0] == 0) {
            dp[0][i] = i / arr[0];
        } else {
            dp[0][i] = 1e8;
        }
    }

    for (int ind = 1; ind < n; ind++){
        for (int tar = 0; tar <= target; tar++) {

            int notPick = 0 + dp[ind - 1][tar];
            int pick = 1e8;
            if (arr[ind] <= tar) {
                pick = 1 + dp[ind][tar - arr[ind]];
            }

            dp[ind][tar] = MIN(pick, notPick);
            //printf("ind %d, target %d : %d \r\n", ind, tar, dp[ind][tar]);
        }
    }

    int ans = dp[n-1][target];

    if (ans >= 1e8) {
        return -1;
    }  else {
        return ans;
    }
}

int minimumElementsOptimization(int n, int arr[n], int target)
{
    int prev[target+1];
    memset(prev, 0, sizeof(prev));

    int cur[target+1];
    memset(prev, 0, sizeof(prev));

    for(int i = 0; i <= target; i++){
        if(i % arr[0] == 0) {
            prev[i] = i / arr[0];
        } else {
            prev[i] = 1e8;
        }
    }

    for (int ind = 1; ind < n; ind++){
        for (int tar = 0; tar <= target; tar++) {

            int notPick = 0 + prev[tar];
            int pick = 1e8;
            if (arr[ind] <= tar) {
                pick = 1 + cur[tar - arr[ind]];
            }

            cur[tar] = MIN(pick, notPick);
            //printf("ind %d, target %d : %d \r\n", ind, tar, dp[ind][tar]);
        }

        for(int j = 0; j <= target; j++) {
            prev[j] = cur[j];
        }
    }

    int ans = prev[target];

    if (ans >= 1e8) {
        return -1;
    }  else {
        return ans;
    }
}

int main() {

    int arr[] = {1,2,3};
    int n = SIZEOF(arr);
    int target = 7;
    int coins = 0;

    coins = minimumElementsMemoization(n, arr, target);
    printf("[memoization] The minimum number of coins required to form the target sum is %d \r\n", coins);

    coins = minimumElementsTabulation(n, arr, target);
    printf("[tabulation] The minimum number of coins required to form the target sum is %d \r\n", coins);

    coins = minimumElementsOptimization(n, arr, target);
    printf("[optimization] The minimum number of coins required to form the target sum is %d \r\n", coins);

    return 0;
}