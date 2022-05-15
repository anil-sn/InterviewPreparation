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
// https://youtu.be/ekcwMsSIzVc

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int lisUtil(int ind, int prev, int n, int arr[])
{
    if (ind == n) {
        return 0;
    }

    int len = 0 + lisUtil(ind + 1, prev, n, arr);
    if ((prev == -1) || (arr[ind] > arr[prev])) {
        len = MAX(len, 1 + lisUtil(ind + 1, ind, n, arr));
    }

    return len;
}

int lis(int len, int arr[])
{
    return lisUtil(0, -1, len, arr);
}

int lisUtilMemoization(int ind, int prev, int n, int arr[], int row, int col, int dp[row][col])
{
    if (ind == n) {
        return 0;
    }

    if (-1 != dp[ind][prev + 1]){
        return dp[ind][prev + 1];
    }

    int len = 0 + lisUtilMemoization(ind + 1, prev, n, arr, row, col, dp);
    if ((prev == -1) || (arr[ind] > arr[prev])) {
        len = MAX(len, 1 + lisUtilMemoization(ind + 1, ind, n, arr, row, col, dp));
    }

    dp[ind][prev + 1] = len;
    return dp[ind][prev + 1];
}

int lisMemoization(int len, int arr[])
{
    int dp[len][len+1];
    memset(dp, -1, sizeof(dp));

    return lisUtilMemoization(0, -1, len, arr, len, len+1, dp);
}

int lisTabulation(int n, int arr[])
{
    int dp[n + 1][n + 1];
    memset(dp, 0, sizeof(dp));

    for (int ind = n-1; ind >= 0; ind--) {
        for (int prev = ind-1; prev >= -1; prev--) {

            int len = 0 + dp[ind + 1][prev + 1];
            if ((prev == -1) || (arr[ind] > arr[prev])) {
                len = MAX(len, 1 + dp[ind + 1][ind + 1]);
            }

            dp[ind][prev + 1] = len;
        }
    }
    return dp[0][0];
}

int lisOptimized(int n, int arr[])
{
    int next[n + 1];
    memset(next, 0, sizeof(next));

    int curr[n + 1];
    memset(curr, 0, sizeof(curr));

    for (int ind = n-1; ind >= 0; ind--) {
        for (int prev = ind-1; prev >= -1; prev--) {

            int len = 0 + next[prev + 1];
            if ((prev == -1) || (arr[ind] > arr[prev])) {
                len = MAX(len, 1 + next[ind + 1]);
            }

            curr[prev + 1] = len;
        }

        for (int i = 0; i <= n; i++){
            next[i] = curr[i];
        }
    }

    return next[0];
}


int lisIterative(int n, int arr[])
{
    int dp[n];
    int hash[n];
    for (int i = 0; i < n; i++){
        dp[i] = 1;
        hash[i] = i;
    }

    int maxi = 1;
    int last = 0;
    for (int i = 0; i < n; i++){
        for (int prev = 0; prev < i; prev++) {
            if ((arr[prev] < arr[i]) && (1 + dp[prev] > dp[i])){
                dp[i] = 1 + dp[prev];
                hash[i] = prev;
            }
        }
        if (dp[i] > maxi){
            last = i;
            maxi = dp[i];
        }
    }

    int lis[maxi];

    int pos = 0;
    lis[pos++] = arr[last];

    while (hash[last] != last){
        last = hash[last];
        lis[pos++] = arr[last];
    }

    printf("LIS : \r\n");
    for (int i = pos-1; i >= 0; i--){
        printf("%3d", lis[i]);
    }
    printf("\r\n");

    return maxi;
}

int main ()
{
    int arr[] = {5, 4, 11, 1, 16, 8};
    int len = SIZEOF(arr);

    printf("The Given Sequence of numbers : \r\n");
    for (int i = 0; i < len; i++) {
        printf("%4d", arr[i]);
    }
    printf("\r\n");

    int result = lis(len, arr);
    printf("[recursion] The Longest Increasing Subsequence : %d \r\n", result);

    result = lisMemoization(len, arr);
    printf("[memoization] The Longest Increasing Subsequence : %d \r\n", result);

    result = lisTabulation(len, arr);
    printf("[tabulation] The Longest Increasing Subsequence : %d \r\n", result);

    result = lisOptimized(len, arr);
    printf("[optimized] The Longest Increasing Subsequence : %d \r\n", result);

    result = lisIterative(len, arr);
    printf("[iterative] The Longest Increasing Subsequence : %d \r\n", result);

    return 0;
}