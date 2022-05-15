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
// https://takeuforward.org/data-structure/rod-cutting-problem-dp-24/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int cutRodUtilMemoization(int ind, int n, int price[], int row, int col, int dp[row][col])
{
    if(ind == 0){
        return n * price[0];
    }

    if(dp[ind][n]!=-1)
        return dp[ind][n];

    int notTaken = 0 + cutRodUtilMemoization(ind-1, n, price, row, col,dp);

    int taken = INT_MIN;
    int rodLength = ind + 1;
    if(rodLength <= n)
        taken = price[ind] + cutRodUtilMemoization(ind, n-rodLength, price,row,col,dp);

    return dp[ind][n] = MAX(notTaken,taken);
}

int cutRodMemoization (int n, int price[n])
{
    int dp[n][n+1];
    memset(dp, -1, sizeof(dp));

    return cutRodUtilMemoization(n-1, n, price, n, n+1, dp);
}

int cutRodTabulation (int n, int price[n])
{
    int dp[n][n+1];
    memset(dp, -1, sizeof(dp));

    for (int length = 0; length <= n; length++){
        dp[0][length] = length * price[0];
    }

    for(int ind = 1; ind < n; ind++){
        for(int length = 0; length <= n; length++){

             int notTaken = 0 + dp[ind-1][length];

             int taken = INT_MIN;
             int rodLength = ind + 1;
             if(rodLength <= length)
                taken = price[ind] + dp[ind][length-rodLength];

             dp[ind][length] = MAX(notTaken,taken);
        }
    }

    return dp[n-1][n];
}

int cutRodOptimized (int n, int price[n]) {

    int cur[n+1];
    memset(cur, 0, sizeof(cur));

    for(int length = 0; length <= n; length++) {
        cur[length] = length * price[0];
    }

    for(int ind = 1; ind < n; ind++) {
        for(int length = 0; length <= n; length++) {

             int notTaken = 0 + cur[length];

             int taken = INT_MIN;
             int rodLength = ind + 1;
             if(rodLength <= length)
                taken = price[ind] + cur[length-rodLength];

             cur[length] = MAX(notTaken,taken);
        }
    }

    return cur[n];
}

int main() {

    int price[] = {2,5,7,8,10};
    int n = SIZEOF(price);

    printf("[Memoization] The Maximum price generated is : %d \r\n", cutRodMemoization(n, price));
    printf("[Tabulation] The Maximum price generated is : %d \r\n", cutRodTabulation(n, price));
    printf("[Optimized] The Maximum price generated is : %d \r\n", cutRodOptimized(n, price));
}