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
// https://youtu.be/nGJmxkUJQGs

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int getMaxProfitUtil(int ind, bool buy, int n, int stockPrice[n])
{
    if (n == ind) {
        return 0;
    }

    int profit = 0;

    if (buy){
        profit = MAX((getMaxProfitUtil(ind+1, false, n, stockPrice) - (stockPrice[ind])),
                     (getMaxProfitUtil(ind+1, true, n, stockPrice) - 0));
    } else {
        profit = MAX((getMaxProfitUtil(ind+1, true, n, stockPrice) + (stockPrice[ind])),
                     (getMaxProfitUtil(ind+1, false, n, stockPrice) + 0));
    }

    return profit;
}

int getMaxProfit(int n, int stockPrice[n])
{
    return (getMaxProfitUtil(0, true, n, stockPrice));
}

int getMaxProfitUtilMemoization(int ind, bool buy, int n, int stockPrice[n], int dp[][2])
{
    if (n == ind) {
        return 0;
    }

    if (-1 != dp[ind][buy]) {
        return dp[ind][buy];
    }

    if (buy){
        dp[ind][buy] = MAX((getMaxProfitUtilMemoization(ind+1, false, n, stockPrice, dp) - (stockPrice[ind])),
                     (getMaxProfitUtilMemoization(ind+1, true, n, stockPrice, dp) - 0));
    } else {
        dp[ind][buy] = MAX((getMaxProfitUtilMemoization(ind+1, true, n, stockPrice, dp) + (stockPrice[ind])),
                     (getMaxProfitUtilMemoization(ind+1, false, n, stockPrice, dp) + 0));
    }

    return dp[ind][buy];
}

int getMaxProfitMemoization(int n, int stockPrice[n])
{
    int dp[n][2];
    memset(dp, -1, sizeof(dp));

    return (getMaxProfitUtilMemoization(0, true, n, stockPrice, dp));
}

int getMaxProfitTabulation(int n, int stockPrice[n])
{
    int dp[n+1][2];
    memset(dp, 0, sizeof(dp));

    dp[n][0] = dp[n][1] = 0;
    for (int ind = n-1; ind >= 0; ind--){
        for (int buy = 0; buy <= 1; buy++){
            if (buy){
                dp[ind][buy] = MAX((dp[ind + 1][0] - stockPrice[ind]), (dp[ind+1][1] - 0));
            } else {
                dp[ind][buy] = MAX((dp[ind+1][1] + stockPrice[ind]), (dp[ind+1][0] + 0));
            }
        }
    }

    return dp[0][1];
}

int getMaxProfitOptimized(int n, int stockPrice[n])
{
    int ahead[2];
    memset(ahead, 0, sizeof(ahead));

    int curr[2];
    memset(curr, 0, sizeof(curr));

    ahead[0] = ahead[1] = 0;

    for (int ind = n-1; ind >= 0; ind--){
        for (int buy = 0; buy <= 1; buy++){
            if (buy){
                curr[buy] = MAX((ahead[0] - stockPrice[ind]), (ahead[1] - 0));
            } else {
                curr[buy] = MAX((ahead[1] + stockPrice[ind]), (ahead[0] + 0));
            }
        }
        ahead[0] = curr[0];
        ahead[1] = curr[1];
    }

    return ahead[1];
}

// Multiple Buy and Multiple Sell, condition is you have to sell first before buying new one
int main (void)
{
    int stockPrice [] = {7, 1, 5, 3, 6, 4};
    int n = SIZEOF(stockPrice);

    int profit = 0;

    profit = getMaxProfit(n, stockPrice);
    printf("[recursion] The maximim profit is : %d \r\n", profit);

    profit = getMaxProfitMemoization(n, stockPrice);
    printf("[memoization] The maximim profit is : %d \r\n", profit);

    profit = getMaxProfitTabulation(n, stockPrice);
    printf("[tabulation] The maximim profit is : %d \r\n", profit);

    profit = getMaxProfitOptimized(n, stockPrice);
    printf("[optimized] The maximim profit is : %d \r\n", profit);

    return 0;
}