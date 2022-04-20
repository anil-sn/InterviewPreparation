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
// https://takeuforward.org/data-structure/maximum-sum-of-non-adjacent-elements-dp-5/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)

int solveUtilMemoization(int ind, int arr[], int dp[])
{
    if(ind == 0) {
        return arr[ind];
    }

    if(ind < 0) {
        return 0;
    }

    if (dp[ind] != -1) {
        return dp[ind];
    }

    int pick = arr[ind] + solveUtilMemoization (ind-2, arr, dp);
    int nonPick = 0 + solveUtilMemoization (ind-1, arr, dp);

    dp[ind] = MAX(pick, nonPick);

    printf("For index %d sum is %d \r\n", ind, dp[ind]);
    return dp[ind];
}

int solveMemoization(int n, int arr[])
{
    int dp[n];

    memset(dp, -1, sizeof(dp));

    return solveUtilMemoization(n-1, arr, dp);
}

int solveUtilTabulation(int arr[], int n, int dp[])
{
    dp[0]= arr[0];

    for(int i = 1; i < n; i++) {

        int pick = arr[i];

        if(i > 1)
            pick += dp[i-2];

        int nonPick = 0 + dp[i-1];

        dp[i]= MAX(pick, nonPick);
    }

    return dp[n-1];
}

int solveTabulation(int n, int arr[])
{
    int dp[n];

    memset(dp, -1, sizeof(dp));

    return solveUtilTabulation(arr, n, dp);
}

int main()
{
    int arr[] = {2,1,4,9};
    int n = SIZEOF(arr);

    for (int i = 0; i < n; i++) {
        printf("[%d] = %d \r\n", i, arr[i]);
    }

    printf("[Memoization] Max Sum: %d \r\n", solveMemoization(n, arr));
    printf("[Tabulation] Max Sum: %d \r\n", solveTabulation(n, arr));

    return 0;
}