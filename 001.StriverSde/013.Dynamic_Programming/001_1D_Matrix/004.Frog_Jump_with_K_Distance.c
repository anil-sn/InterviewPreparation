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

#define MIN(a,b) (a < b ? a : b)

int solveMemoization(int ind, int height[], int dp[], int k)
{
    if (ind == 0) {
        return 0;
    }

    if(dp[ind] != -1) {
        return dp[ind];
    }

    int mmSteps = INT_MAX;

    for(int j=1; j<=k; j++) {
        if(ind-j >= 0){
            int jump = solveMemoization(ind-j, height, dp, k)+ abs(height[ind]- height[ind-j]);
            mmSteps= MIN(jump, mmSteps);
        }
    }

    return dp[ind]= mmSteps;

}

int solveTabulation(int ind, int height[], int n, int dp[], int k)
{
    dp[0]=0;

    for(int i=1; i<n; i++){

        int mmSteps = INT_MAX;
        for(int j=1; j <= k; j++) {
            if(i-j>=0){
                int jump = dp[i-j]+ abs(height[i]- height[i-j]);
                mmSteps= MIN(jump, mmSteps);
            }
        }

        dp[i]= mmSteps;
    }

    return dp[n-1];
}

int main() {

    int height[] = {30, 10, 60, 10, 60, 50};
    int n = sizeof(height)/sizeof(height[0]);
    int k=2;

    int dp[n];
    memset(dp, -1, sizeof(dp));

    for (int i = 0; i < n; i++) {
        printf("Stair %d Energy Needed  %d \r\n", i, height[i]);
    }

    printf("[Memoization] Energy need to reach %d stairs is : %d with %d Energy \r\n", n, k, solveMemoization(n-1, height, dp, k));
    printf("[Tabulation] Energy need to reach %d stairs is : %d with %d Energy \r\n", n, k, solveTabulation(n-1, height, n, dp, k));
}