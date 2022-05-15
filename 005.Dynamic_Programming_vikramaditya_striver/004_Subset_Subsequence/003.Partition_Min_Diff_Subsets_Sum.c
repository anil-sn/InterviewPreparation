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
// https://takeuforward.org/data-structure/partition-set-into-2-subsets-with-min-absolute-sum-diff-dp-16/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

bool subsetSumUtilMemoization(int ind, int target, int n, int arr[n], int row, int col, int dp[row][col])
{
    if (0 == target) {
        dp[ind][target] = true;
        return true;
    }

    if (0 == ind) {
        if (arr[ind] == target) {
            dp[ind][target] = true;
        }
    }

    if (ind < 0) {
        return false;
    }

    if (-1 != dp[ind][target]) {
        return dp[ind][target];
    }

    bool notTaken = subsetSumUtilMemoization(ind - 1, target, n, arr, row, col, dp);

    bool taken = false;
    if (arr[ind] <= target) {
        taken = subsetSumUtilMemoization(ind - 1, target - arr[ind], n, arr, row, col, dp);
    }

    if (notTaken || taken) {
        dp[ind][target] = true;
    } else {
        dp[ind][target] = false;
    }

    return dp[ind][target];
}

int minSubsetSumDifferenceMemoization(int n, int arr[n])
{
    int totSum = 0;
    for (int i = 0; i < n; i++) {
        totSum += arr[i];
    }

    int dp[n][totSum + 1];
    memset(dp, -1, sizeof(dp));

    for (int i = 0; i <= totSum; i++) {
        bool dummy = subsetSumUtilMemoization(n - 1, i, n, arr, n, (totSum + 1), dp);
    }

    int mini = 1e9;
    int diff = 0;
    for (int i = 0; i <= totSum; i++) {
        printf("%3d", i);
    }
    printf("\r\n\r\n");
    for (int i = 0; i <= totSum; i++) {
        printf("%3d", dp[n - 1][i]);
        if (dp[n - 1][i] == true) {
            diff = abs(i - (totSum - i));
            mini = MIN(mini, diff);
        }
    }
    printf("\r\n");

    return mini;
}

int minSubsetSumDifferenceTabulation(int n, int arr[n])
{
    int totSum = 0;

    for (int i = 0; i < n; i++) {
        totSum += arr[i];
    }

    bool dp[n][totSum + 1];
    memset(dp, false, sizeof(dp));

    for (int i = 0; i < n; i++) {
        dp[i][0] = true;
    }

    if (arr[0] <= totSum) {
        dp[0][arr[0]] = true;
    }

    for (int ind = 1; ind < n; ind++) {
        for (int target = 1; target <= totSum; target++) {

            bool notTaken = dp[ind - 1][target];

            bool taken = false;
            if (arr[ind] <= target)
                taken = dp[ind - 1][target - arr[ind]];

            if (notTaken || taken) {
                dp[ind][target] = true;
            } else {
                dp[ind][target] = false;
            }
        }
    }

    int mini = 1e9;
    for (int i = 0; i <= totSum; i++) {
            printf("%3d", dp[n - 1][i]);
            if (dp[n - 1][i] == true) {
            int diff = abs(i - (totSum - i));
            mini = MIN(mini, diff);
        }
    }
    printf("\r\n");

    return mini;
}

int minSubsetSumDifferenceOptimized(int n, int arr[n])
{
    int totSum = 0;

    for (int i = 0; i < n; i++) {
        totSum += arr[i];
    }

    bool prev[totSum + 1];
    memset(prev, false, sizeof(prev));

    bool curr[totSum + 1];
    memset(curr, false, sizeof(curr));

    prev[0] = true;

    if (arr[0] <= totSum) {
        prev[arr[0]] = true;
    }

    for (int ind = 1; ind < n; ind++) {
        curr[0] = true;
        for (int target = 1; target <= totSum; target++) {

            bool notTaken = prev[target];

            bool taken = false;
            if (arr[ind] <= target)
                taken = prev[target - arr[ind]];

            if (notTaken || taken) {
                curr[target] = true;
            } else {
                curr[target] = false;
            }
        }

       for (int j = 1; j <= totSum; j++) {
           prev[j] = curr[j];
       }
    }

    int mini = 1e9;
    for (int i = 0; i <= totSum; i++) {
            printf("%3d", prev[i]);
            if (prev[i] == true) {
            int diff = abs(i - (totSum - i));
            mini = MIN(mini, diff);
        }
    }
    printf("\r\n");

    return mini;
}


int main()
{
    int arr[] = {2,5,5};
    int n = SIZEOF(arr);
    int minDiff = 0;

    printf("Input :\r\n");
    for (int i = 0; i < n; i++){
        printf("arr[%d] = %d \r\n", i, arr[i]);
    }
    printf("\r\n");

    minDiff = minSubsetSumDifferenceMemoization(n, arr);
    printf("[memoization] The minimum absolute difference is: %d \r\n", minDiff);

    minDiff = minSubsetSumDifferenceTabulation(n, arr);
    printf("[tabulation] The minimum absolute difference is: %d \r\n", minDiff);

    minDiff = minSubsetSumDifferenceOptimized(n, arr);
    printf("[optimized] The minimum absolute difference is: %d \r\n", minDiff);
}