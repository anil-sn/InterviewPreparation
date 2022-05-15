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
//https://takeuforward.org/data-structure/dynamic-programming-frog-jump-dp-3/

#define MIN(a,b) (a < b ? a : b)
int solve(int ind, int height[],  int dp[])
{
    if (ind == 0) {
        return 0;
    }

    if(dp[ind] != -1) {
        return dp[ind];
    }

    int jumpTwo = INT_MAX;
    int jumpOne = solve(ind-1, height, dp) + abs( height[ind] - height[ind-1]);

    if(ind > 1) {
        jumpTwo = solve(ind-2, height, dp) + abs( height[ind] - height[ind-2]);
    }

    dp[ind] = MIN(jumpOne, jumpTwo);
    printf("Energy consumed to move %d th stair is %d \r\n", ind, dp[ind]);
    return dp[ind];
}

int solveTabulation(int height[],  int n)
{
    int dp[n];
    memset(dp, -1, sizeof(dp));

    dp[0]=0;
    for (int ind = 1; ind < n; ind++ )
    {
        int jumpTwo = INT_MAX;
        int jumpOne= dp[ind - 1] + abs( height[ind] - height[ind-1] );
        if(ind > 1) {
            jumpTwo = dp[ind - 2] + abs( height[ind] - height[ind-2] );
        }

        dp[ind] = MIN(jumpOne, jumpTwo);
    }

    return (dp[n-1]);
}

int solveOptimized(int height[],  int n)
{
    int prev = 0;
    int prev2 = 0;
    int cur = 0;

    for (int ind = 1; ind < n; ind++ )
    {
        int jumpTwo = INT_MAX;
        int jumpOne= prev + abs( height[ind] - height[ind-1] );
        if(ind > 1) {
            jumpTwo = prev2 + abs( height[ind] - height[ind-2] );
        }

        cur = MIN(jumpOne, jumpTwo);
        prev2 = prev;
        prev = cur;
    }

    return (prev);
}

int main() {

    int height[] = {30, 10, 60, 10, 60, 50};
    int n = sizeof(height)/sizeof(height[0]);

    int dp[n+1];
    memset(dp, -1, sizeof(dp));

    for (int i = 0; i < n; i++) {
        printf("Stair %d Energy Needed  %d \r\n", i, height[i]);
    }

    printf("[Memoization] Energy need to reach %d stairs is : %d \r\n", n, solve(n-1, height, dp));
    printf("[Tabulation] Energy need to reach %d stairs is : %d \r\n", n, solveTabulation(height, n));
    printf("[Optimized] Energy need to reach %d stairs is : %d \r\n", n, solveOptimized(height, n));

    printf("\r\n");

    return 0;
}