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

int solveUtilMemoization(int index, int arr[], int dp[])
{
    if(0 > index) {
        return 0;
    }

    if (0 == index) {
        return arr[index];
    }

    if (dp[index] != -1) {
        return dp[index];
    }

    int pick = solveUtilMemoization(index - 2 , arr, dp) + arr[index];
    int nonPick = solveUtilMemoization (index - 1, arr, dp) + 0;

    dp[index] = MAX(pick, nonPick);

    printf("For index %d sum is %d \r\n", index, dp[index]);
    return dp[index];
}

int solveMemoization (int arr[], int n){
    int dp[n+1];
    memset(dp, -1, sizeof(dp));
    return (solveUtilMemoization(n-1, arr, dp));
}


int solveTabulation(int arr[], int n)
{
    int prev = arr[0];
    int prev2 = 0;

    for (int i = 1; i < n; i++)
    {
        int pick = arr[i];
        if(i > 1)
            pick += prev2;
        int nonPick = 0 + prev;

        int cur_i = MAX(pick, nonPick);
        prev2 = prev;
        prev= cur_i;

        printf("For index %d sum is %d \r\n", i, prev);
    }

    return prev;
}

int robStreet(int n, int arr[], bool isMemoization)
{
    int arr1[n];
    int arr2[n];
    int ans1 = 0;
    int ans2 = 0;
    int pos1 = 0;
    int pos2 = 0;

    for(int i = 0; i < n; i++){

        if (i != 0) {
            arr1[pos1++] = (arr[i]);
        }

        if (i != n-1) {
            arr2[pos2++] = (arr[i]);
        }
    }

    printf("Solving for Array 1 \r\n");
    for (int i = 0; i < pos1; i++) {
        printf("[%d] = %d \r\n", i, arr1[i]);
    }

    if (isMemoization) {
        ans1 = solveMemoization(arr1, pos1);
    } else {
        ans1 = solveTabulation(arr1, pos1);
    }

    printf("Solving for Array 2 \r\n");
    for (int i = 0; i < pos2; i++) {
        printf("[%d] = %d \r\n", i, arr2[i]);
    }
    if (isMemoization) {
        ans2 = solveMemoization(arr2, pos2);
    } else {
        ans2 = solveTabulation(arr2, pos2);
    }

    return MAX(ans1,ans2);
}


int main() {

    int arr[] = {1,5,1,2,6};
    int n = SIZEOF(arr);

    printf("[Memoization] Max Robbery: %d \r\n", robStreet(n, arr, true));
    printf("[Tabulation] Max Robbery: %d \r\n", robStreet(n, arr, false));

    return 0;
}