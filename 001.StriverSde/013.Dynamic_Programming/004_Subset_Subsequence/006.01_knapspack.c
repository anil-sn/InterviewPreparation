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
// https://takeuforward.org/data-structure/0-1-knapsack-dp-19/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int knapsackUtilMemoization (int item, int n, int wt[n], int val[n], int capacity, int dp[n][capacity + 1])
{
    if (item == 0) {
        if (wt[item] <= capacity) {
            return val[item];
        } else {
            return 0;
        }
    }

    if (-1 != dp[item][capacity]) {
        return dp[item][capacity];
    }

    int notPick = 0 + knapsackUtilMemoization((item - 1), n, wt, val, capacity, dp);
    int pick = INT_MIN;
    if (wt[item] <= capacity) {
        pick = val[item] + knapsackUtilMemoization((item - 1), n, wt, val, capacity - wt[item], dp);
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
    return result;
}

int knapsackTabulation(int n, int wt[n], int val[n], int capacity)
{
    int result = 0;
    int dp[n][capacity + 1];
    memset(dp, 0, sizeof(dp));

    for(int i = wt[0]; i <= capacity; i++) {
        dp[0][i] = val[0];
    }

    for(int item = 1; item < n; item ++){
        for(int cap = 0; cap <= capacity; cap++){

            int notTaken = 0 + dp[item - 1][cap];

            int taken = INT_MIN;
            if(wt[item] <= cap)
                taken = val[item] + dp[item - 1][cap - wt[item]];

            dp[item][cap] = MAX(notTaken, taken);
        }
    }

    result = dp[n-1][capacity];
    return result;
}

int knapsackOptimizedPhase1(int n, int wt[n], int val[n], int capacity)
{
    int result = 0;

    int prev[capacity + 1];
    memset(prev, 0, sizeof(prev));

    int curr[capacity + 1];
    memset(curr, 0, sizeof(curr));

    for(int i = wt[0]; i <= capacity; i++){
        prev[i] = val[0];
    }

    for(int item = 1; item < n; item ++){

        for(int cap = capacity; cap >= 0; cap--){

            int notTaken = 0 + prev[cap];

            int taken = INT_MIN;
            if(wt[item] <= cap)
                taken = val[item] + prev[cap - wt[item]];

            curr[cap] = MAX(notTaken, taken);
        }

        for (int j = 0; j <= capacity; j++){
            prev[j] = curr[j];
        }
    }

    result = prev[capacity];
    return result;
}

int knapsackOptimizedPhase2(int n, int wt[n], int val[n], int capacity)
{
    int result = 0;

    int prev[capacity + 1];
    memset(prev, 0, sizeof(prev));

    for(int i = wt[0]; i <= capacity; i++){
        prev[i] = val[0];
    }

    for(int item = 1; item < n; item ++){

        for(int cap = capacity; cap >= 0; cap--){

            int notTaken = 0 + prev[cap];

            int taken = INT_MIN;
            if(wt[item] <= cap)
                taken = val[item] + prev[cap - wt[item]];

            prev[cap] = MAX(notTaken, taken);
        }
    }

    result = prev[capacity];
    return result;
}

int main() {

    int wt[] = {1,2,4,5};
    int val[] = {5,4,8,6};
    int capacity = 5;
    int n = SIZEOF(wt);
    int result = 0;

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