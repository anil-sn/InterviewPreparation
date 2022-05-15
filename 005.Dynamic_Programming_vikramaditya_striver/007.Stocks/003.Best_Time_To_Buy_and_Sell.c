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
// https://youtu.be/-uQGzhYj8BQ

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999
#define UNUSED_ARG(x) (void)x

int getMaxProfitUtil(int ind, bool buy, int n, int stockPrice[n], int cap)
{
    if ((n == ind) || (0 == cap)) {
        return 0;
    }

    int profit = 0;

    if (buy){
        profit = MAX((getMaxProfitUtil(ind+1, false, n, stockPrice, cap) - (stockPrice[ind])),
                     (getMaxProfitUtil(ind+1, true, n, stockPrice, cap) - 0));
    } else {
        profit = MAX((getMaxProfitUtil(ind+1, true, n, stockPrice, (cap - 1)) + (stockPrice[ind])),
                     (getMaxProfitUtil(ind+1, false, n, stockPrice, cap) + 0));
    }

    return profit;
}

int getMaxProfit(int n, int stockPrice[n], int transactionCap)
{
    return (getMaxProfitUtil(0, true, n, stockPrice, transactionCap));
}

int getMaxProfitUtilMemoization(int ind, bool buy, int n, int stockPrice[n], int dp[][2], int cap)
{
    if ((n == ind) || (0 == cap)) {
        return 0;
    }

    if (-1 != dp[ind][buy]) {
        return dp[ind][buy];
    }

    if (buy){
        dp[ind][buy] = MAX((getMaxProfitUtilMemoization(ind+1, false, n, stockPrice, dp, cap) - (stockPrice[ind])),
                     (getMaxProfitUtilMemoization(ind+1, true, n, stockPrice, dp, cap) - 0));
    } else {
        dp[ind][buy] = MAX((getMaxProfitUtilMemoization(ind+1, true, n, stockPrice, dp, (cap - 1)) + (stockPrice[ind])),
                     (getMaxProfitUtilMemoization(ind+1, false, n, stockPrice, dp, cap) + 0));
    }

    return dp[ind][buy];
}

int getMaxProfitMemoization(int n, int stockPrice[n], int transactionCap)
{
    int dp[n][2];
    memset(dp, -1, sizeof(dp));

    return (getMaxProfitUtilMemoization(0, true, n, stockPrice, dp, transactionCap));
}

int getMaxProfitTabulation(int n, int stockPrice[n], int transactionCap)
{
    int dp[n + 1][2][transactionCap + 1];
    memset(dp, 0, sizeof(dp));
    UNUSED_ARG(transactionCap);

    for (int ind = n-1; ind >= 0; ind--){
        for (int buy = 0; buy <= 1; buy++){
            for (int cap = 1; cap <= transactionCap; cap++){
                if (buy){
                    dp[ind][buy][cap] = MAX((dp[ind + 1][0][cap] - stockPrice[ind]),
                                            (dp[ind+1][1][cap] - 0));
                } else {
                    dp[ind][buy][cap] = MAX((dp[ind+1][1][cap - 1] + stockPrice[ind]),
                                            (dp[ind+1][0][cap] + 0));
                }
            }
        }
    }

    return dp[0][1][transactionCap];
}

int getMaxProfitOptimized(int n, int stockPrice[n], int transactionCap)
{
    int after[2][transactionCap + 1];
    memset(after, 0, sizeof(after));

    int curr[2][transactionCap + 1];
    memset(curr, 0, sizeof(curr));

    for (int ind = n-1; ind >= 0; ind--){
        for (int buy = 0; buy <= 1; buy++){
            for (int cap = 1; cap <= transactionCap; cap++){
                if (buy){
                    curr[buy][cap] = MAX((after[0][cap] - stockPrice[ind]), (after[1][cap] - 0));
                } else {
                    curr[buy][cap] = MAX((after[1][cap - 1] + stockPrice[ind]), (after[0][cap] + 0));
                }
            }
        }

        for (int i = 0; i < 2; i++){
            for (int j = 0; j <= transactionCap; j++){
                after[i][j] = curr[i][j];
            }
        }
    }

    return after[1][transactionCap];
}

// At max only two transaction,
// condition is you have to sell first before buying new one
int main (void)
{
    int stockPrice [] = {3,3,5,0,0,3,1,4};
    int n = SIZEOF(stockPrice);
    int transactionCap = 2;

    int profit = 0;

    profit = getMaxProfit(n, stockPrice, transactionCap);
    printf("[recursion] The maximim profit is : %d \r\n", profit);

    profit = getMaxProfitMemoization(n, stockPrice, transactionCap);
    printf("[memoization] The maximim profit is : %d \r\n", profit);

    profit = getMaxProfitTabulation(n, stockPrice, transactionCap);
    printf("[tabulation] The maximim profit is : %d \r\n", profit);

    profit = getMaxProfitOptimized(n, stockPrice, transactionCap);
    printf("[optimized] The maximim profit is : %d \r\n", profit);

    return 0;
}