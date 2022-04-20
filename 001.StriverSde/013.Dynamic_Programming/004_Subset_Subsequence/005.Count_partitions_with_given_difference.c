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
// https://takeuforward.org/data-structure/count-partitions-with-given-difference-dp-18/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int findWaysUtilMemoization(int ind, int target, int n, int nums[n], int row, int col, int dp[row][col])
{
    if (0 == ind) {

        if ((target == 0) && (nums[0] == 0)) {
            return 2;
        }

        if ((target == 0) || (nums[0] == target)) {
            return 1;
        }

        return (0);
    }

    if (-1 != dp[ind][target]) {
        return dp[ind][target];
    }

    int notPick = findWaysUtilMemoization((ind - 1), target, n, nums, row, col, dp);
    int pick = 0;
    if (nums[ind] <= target) {
        pick = findWaysUtilMemoization((ind - 1), (target - nums[ind]), n, nums, row, col, dp);
    }

    dp[ind][target] = notPick + pick;

    return dp[ind][target];
}

int findWaysMemoization(int n , int num[n], int k)
{
    int dp[n][k+1];
    memset(dp, -1, sizeof(dp));

    int ways = 0;
    ways = findWaysUtilMemoization(n-1, k, n, num,n, (k+1), dp);
    return ways;
}

int findWaysTabulation(int n , int num[n], int k)
{
    int dp[n][k+1];
    memset(dp, 0, sizeof(dp));

    for (int i = 0; i <= n; i++){
        dp[i][0] = 1;
    }

    if (num[0] <= k) {
        dp[0][num[0]] = 1;
    }

    for (int ind = 1; ind < n; ind++) {
        for (int target = 1; target <= k; target++) {

            int notPick = dp[ind - 1][target];
            int pick = 0;
            if (num[ind] <= target) {
                pick = dp[ind - 1][target - num[ind]];
            }

            dp[ind][target] = pick + notPick;
        }
    }

    return dp[n-1][k];
}

int findWaysOptimized(int n , int num[n], int k)
{
    int prev[k+1];
    memset(prev, 0, sizeof(prev));

    int curr[k+1];
    memset(curr, 0, sizeof(curr));

    prev[0] = 1;

    if (num[0] <= k) {
        prev[num[0]] = 1;
    }

    for (int ind = 1; ind < n; ind++) {
        curr[0] = 1;
        for (int target = 1; target <= k; target++) {

            int notPick = prev[target];
            int pick = 0;
            if (num[ind] <= target) {
                pick = prev[target - num[ind]];
            }

            curr[target] = pick + notPick;
        }

        for (int j = 0; j <= k; j++) {
            prev[j] = curr[j];
        }
    }

    return prev[k];
}

int main()
{
    int arr[] = {5,2,6,4};
    int n = SIZEOF(arr);
    int d = 3;
    int subsets = 0;
    int totSum = 0;
    for (int i = 0; i < n; i++) {
        totSum += arr[i];
    }
    //Checking for edge cases
    if (totSum - d < 0 || (totSum - d) % 2) {
        printf("The number of subsets found are : %d \r\n", 0);
        return 0;
    }

    /*
        totalSum = s1 + s2;
        s1 = totalSum - s2;
        s2 = totalSum - s1;

        if (s1 > s2) {
            s1 - s2 = d
            (totalSum - s2) -s2 = d
            totalSum - 2 * s2 = d
            2 * s2 = totalSum - d
            s1 = (totalSum - d) /2
        }
    */

    int k = ((totSum - d) / 2);

    subsets = findWaysMemoization(n, arr, k);
    printf("[Memoization] The number of subsets found are : %d \r\n",  subsets);
    subsets = findWaysTabulation(n, arr, k);
    printf("[Tabulation] The number of subsets found are : %d \r\n",  subsets);
    subsets = findWaysOptimized(n, arr, k);
    printf("[Optimized] The number of subsets found are : %d \r\n",  subsets);
}