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
// https://takeuforward.org/data-structure/unbounded-knapsack-dp-23/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int knapsackUtilMemoization (int item, int n, int wt[n], int val[n], int capacity, int dp[n][capacity + 1])
{
    if (item == 0) {
        return ((int)(capacity/wt[item])) * val[item];
    }

    if (-1 != dp[item][capacity]) {
        return dp[item][capacity];
    }

    int notPick = 0 + knapsackUtilMemoization((item - 1), n, wt, val, capacity, dp);
    int pick = INT_MIN;
    if (wt[item] <= capacity) {
        pick = val[item] + knapsackUtilMemoization(item, n, wt, val, capacity - wt[item], dp);
    }

    dp[item][capacity] = MAX(notPick, pick);
    return dp[item][capacity];
}

int knapsackMemoization(int n, int wt[n], int val[n], int capacity)
{
    int result = 0;
    int dp[n][capacity + 1];
    memset(dp, -1, sizeof(dp));

    result = knapsackUtilMemoization(n-1, n, wt, val, capacity, dp);

    printf("DP Output: \r\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= capacity; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return result;
}

int knapsackTabulation(int n, int wt[n], int val[n], int capacity)
{
    int result = 0;
    int dp[n][capacity + 1];
    memset(dp, 0, sizeof(dp));

    for(int i = wt[0]; i <= capacity; i++) {
        dp[0][i] = ((int) i/wt[0]) * val[0];
    }

    for(int item = 1; item < n; item ++){
        for(int cap = 0; cap <= capacity; cap++){

            int notTaken = 0 + dp[item - 1][cap];

            int taken = 0;
            if(wt[item] <= cap)
                taken = val[item] + dp[item][cap - wt[item]];

            dp[item][cap] = MAX(notTaken, taken);
        }
    }

    result = dp[n-1][capacity];


    printf("DP Output: \r\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= capacity; j++) {
            printf("%4d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return result;
}

int knapsackOptimizedPhase1(int n, int wt[n], int val[n], int capacity)
{
    int result = 0;

    int prev[capacity + 1];
    memset(prev, 0, sizeof(prev));

    int curr[capacity + 1];
    memset(curr, 0, sizeof(curr));

    printf("DP Output: \r\n");
    for(int i = wt[0]; i <= capacity; i++){
        prev[i] = ((int)i/wt[0]) * val[0];
        printf("%4d", prev[i]);
    }
    printf("\r\n");

    for(int item = 1; item < n; item ++){
        for(int cap = 0; cap <= capacity; cap++){

            int notTaken = 0 + prev[cap];

            int taken = 0;
            if(wt[item] <= cap)
                taken = val[item] + curr[cap - wt[item]];

            curr[cap] = MAX(notTaken, taken);
        }

        for (int j = 0; j <= capacity; j++){
            prev[j] = curr[j];
            printf("%4d", curr[j]);
        }
        printf("\r\n");
    }

    result = prev[capacity];
    return result;
}

int knapsackOptimizedPhase2(int n, int wt[n], int val[n], int capacity)
{
    int result = 0;

    int cur[capacity + 1];
    memset(cur, 0, sizeof(cur));

    printf("DP Output: \r\n");
    for(int i = wt[0]; i <= capacity; i++){
        cur[i] = ((int)i/wt[0]) * val[0];
        printf("%4d", cur[i]);
    }
    printf("\r\n");

    for(int item = 1; item < n; item ++){

        for(int cap = 0; cap <= capacity; cap++){

            int notTaken = 0 + cur[cap];

            int taken = INT_MIN;
            if(wt[item] <= cap)
                taken = val[item] + cur[cap - wt[item]];

            cur[cap] = MAX(notTaken, taken);
        }

        for (int j = 0; j <= capacity; j++){
            printf("%4d", cur[j]);
        }
        printf("\r\n");
    }

    result = cur[capacity];
    return result;
}

int main() {

    int wt[] = {2,4,6};
    int val[] = {5,11,13};
    int capacity = 10;
    int n = SIZEOF(wt);
    int result = 0;

    printf("Input [weight, value] : \r\n");
    for (int i = 0; i < n; i++) {
        printf("[%3d, %3d] \r\n", wt[i], val[i]);
    }
    printf("\r\n");

    result = knapsackMemoization(n, wt, val, capacity);
    printf("[Memoization] The Maximum value of items, thief can steal is %d \r\n", result);

    result = knapsackTabulation(n, wt, val, capacity);
    printf("[Tabulation] The Maximum value of items, thief can steal is %d \r\n", result);

    result = knapsackOptimizedPhase1(n, wt, val, capacity);
    printf("[Optimized Phase1] The Maximum value of items, thief can steal is %d \r\n", result);

    result = knapsackOptimizedPhase2(n, wt, val, capacity);
    printf("[Optimized Phase2] The Maximum value of items, thief can steal is %d \r\n", result);

    return 0;
}