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

int fibonacci(int n, DP *d){
    if(n <= 1) {
        return n;
    }

    if(d->dp[n] != -1) {
        return d->dp[n];
    }

    d->dp[n] = fibonacci(n-1, d) + fibonacci(n-2, d);
    return d->dp[n];
}


int fibonacciSpaceOptimized(int n) {
    if (n <= 1) {
        return n;
    }

    int prev1 = 1;
    int prev2 = 0;
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
    int result = fibonacci(n, dp);

    printf("[Recursion] The fibonacci value for %d is %d \r\n", n, result);

    result = fibonacciSpaceOptimized(n);
    printf("[Iterative] The fibonacci value for %d is %d \r\n", n, result);

    return 0;
}