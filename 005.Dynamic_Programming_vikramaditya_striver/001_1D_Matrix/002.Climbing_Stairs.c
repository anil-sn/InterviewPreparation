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
// https://takeuforward.org/data-structure/dynamic-programming-climbing-stairs/


typedef struct dp_ {
    int n;
    int *dp;
} DP;

void intiDpMemory(DP *d)
{
    memset(d->dp, -1, (d->n + 1) * (sizeof(int) ));
}

DP * initDP(int n)
{
    DP *d = calloc(1, sizeof(DP));

    d->n = n;
    d->dp = calloc(n+1, sizeof(int));

    intiDpMemory(d);

    return d;
}

int stairs(int n, DP *d)
{
    d->dp[0]= 1;
    d->dp[1]= 1;

    for(int i=2; i<=n; i++){
        d->dp[i] = d->dp[i-1] + d->dp[i-2];
    }

    return d->dp[n];
}


int stairsSpaceOptimized(int n) {

    int prev1 = 1;
    int prev2 = 1;
    int curr = 0;

    for (int i = 2; i <= n; i++) {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}

int main()
{
    int n = 6;
    DP *dp = initDP(n);

    intiDpMemory(dp);
    int result = stairs(n, dp);

    printf("[Recursion] The number of steps for %d stairs is %d \r\n", n, result);

    result = stairsSpaceOptimized(n);
    printf("[Iterative] The number of steps for %d stairs is %d \r\n", n, result);

    return 0;
}