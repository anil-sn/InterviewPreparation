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
// https://takeuforward.org/data-structure/partition-equal-subset-sum-dp-15/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

#define TARGET 10

bool canPartition(int arr[], int n, int *k){

    int totSum = 0;

    for(int i = 0; i < n; i++) {
        totSum += arr[i];
    }

    if (totSum %2 == 1) {
        return false;
    } else {
        *k = totSum / 2;
        return true;
    }
}

bool subsetSumUtilMemoization(int ind, int arr[], int n, int target, int dp[][TARGET + 1])
{
    bool pick = false;
    bool notPick = false;

    if (target == 0) {
        return true;
    }

    if (ind < 0) {
        return false;
    }

    if (0 == ind) {
        if (target == arr[ind]) {
            return true;
        } else {
            return false;
        }
    }

    if (-1 != dp[ind][target]) {
        return dp[ind][target];
    }

    pick = false;
    if (target >= arr[ind]) {
        pick = subsetSumUtilMemoization((ind - 1), arr, n, (target - arr[ind]), dp);
    }
    notPick = subsetSumUtilMemoization((ind - 1), arr, n, (target), dp);

    if (pick || notPick) {
        dp[ind][target] = 1;
    } else {
        dp[ind][target] = 0;
    }

    printf("Ind : %d, Target : %d, Result : %d \r\n", ind, target, dp[ind][target]);
    return dp[ind][target];
}

bool subsetSumToKMemoization(int arr[], int n, int k)
{
    int dp[n][k+1];
    bool result = false;
    memset(dp, -1, sizeof(dp));

    result = subsetSumUtilMemoization(n-1, arr, n, k, dp);

    printf("DP output : \r\n");
    for (int i = 0; i < n; i++){
        for (int j = 0; j <= k; j++){
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return result;
}

bool subsetSumUtilTabulation(int arr[], int n, int k, int dp[][TARGET + 1])
{
    for (int i = 0; i < n; i++) {
        dp[i][0] = true;
    }

    if(arr[0] <= k) {
        dp[0][arr[0]] = true;
    }

    for (int ind = 1; ind < n; ind++) {
        for (int target = 1; target <= k; target++) {

            bool notPick = dp[ind - 1][target];
            bool pick = false;

            if (arr[ind] <= target) {
                pick = dp[ind - 1][target - arr[ind]];
            }

            dp[ind][target] = (pick || notPick);
            printf("Ind : %d, Target : %d, Result : %d \r\n", ind, target, dp[ind][target]);
        }
    }

    return dp[n-1][k];
}

bool subsetSumToKTabulation(int arr[], int n, int k)
{
    int dp[n][k+1];
    bool result = false;
    memset(dp, 0, sizeof(dp));

    result = subsetSumUtilTabulation(arr, n, k, dp);

    printf("DP output : \r\n");
    for (int i = 0; i < n; i++){
        for (int j = 0; j <= k; j++){
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return result;
}

bool subsetSumToKOptimized(int arr[], int n, int k)
{
    bool prev[k+1];
    memset(prev, false, sizeof(prev));

    bool cur[k+1];
    memset(cur, false, sizeof(cur));

    prev[0] = true;

    if(arr[0] <= k)
        prev[arr[0]] = true;

    for(int j = 0; j <= k; j++) {
        printf("%4d", prev[j]);
    }
    printf("\r\n");

    for(int ind = 1; ind < n; ind++) {
        cur[0] = true;
        for(int target = 1; target <= k; target++) {

            bool notTaken = prev[target];
            bool taken = false;
            if(arr[ind] <= target) {
                taken = prev[target - arr[ind]];
            }

            cur[target] = notTaken||taken;
        }

        for(int j = 0; j <= k; j++) {
            prev[j] = cur[j];
            printf("%4d", prev[j]);
        }
        printf("\r\n");
    }

    return prev[k];
}

int main()
{
    int arr[] = {2,3,3,3,4,5};
    int n = SIZEOF(arr);
    bool result = false;
    int k = TARGET;

    printf("Input Array for target sum %d\r\n", k);
    for (int i = 0; i < n; i++){
        printf("Index : [%d] Value : %d\r\n", i , arr[i]);
    }
    printf("\r\n");

    result = canPartition(arr, n, &k);
    if (false == result) {
        printf("The Array cannot be partitioned into two equal subsets \r\n");
        return 0;
    }

    if (TARGET != k){
        printf("System will crash \r\n");
        return 0;
    }

    result = subsetSumToKMemoization(arr, n, k);
    printf("[Memoization] The Array %s be partitioned into two equal subsets with sum %d\r\n", result ? "can" : "cannot", k);

    result = subsetSumToKTabulation(arr, n, k);
    printf("[Memoization] The Array %s be partitioned into two equal subsets with sum %d\r\n", result ? "can" : "cannot", k);

    result = subsetSumToKOptimized(arr, n, k);
    printf("[Optimized] The Array %s be partitioned into two equal subsets with sum %d\r\n", result ? "can" : "cannot", k);

    return 0;
}