#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

/*
    The cost of a stock on each day is given in an array, find the max profit that you can
    make by buying and selling in those days. For example, if the given array is
    {100, 180, 260, 310, 40, 535, 695}, the maximum profit can earned by buying on day 0,
    selling on day 3. Again buy on day 4 and sell on day 6. If the given array of prices
    is sorted in decreasing order, then profit cannot be earned at all.

    Example 1:
        Input:
            N = 7
            A[] = {100,180,260,310,40,535,695}
        Output:
        1
        Explanation:
            One possible solution is (0 3) (4 6)
            We can buy stock on day 0,
            and sell it on 3rd day, which will
            give us maximum profit. Now, we buy
            stock on day 4 and sell it on day 6.

    Example 2:
        Input:
            N = 5
            A[] = {4,2,2,2,4}
        Output:
            1
        Explanation:
            There are multiple possible solutions.
            one of them is (3 4)
            We can buy stock on day 3,
            and sell it on 4th day, which will
            give us maximum profit.

    The task is to complete the function stockBuySell() which takes an array A[] and N
    as input parameters and finds the days of buying and selling stock.
    The function must return a 2D list of integers containing all the buy-sell pairs.
    If there is No Profit, return an empty list.
*/

int maxProfit(int price[], int start, int end)
{
    // If the stocks can't be bought
    if (end <= start)
        return 0;

    // Initialise the profit
    int profit = 0;

    // The day at which the stock
    // must be bought
    for (int i = start; i < end; i++) {

        // The day at which the
        // stock must be sold
        for (int j = i + 1; j <= end; j++) {

            // If buying the stock at ith day and
            // selling it at jth day is profitable
            if (price[j] > price[i]) {

                // Update the current profit
                int curr_profit = price[j] - price[i]
                                  + maxProfit(price, start, i - 1)
                                  + maxProfit(price, j + 1, end);

                // Update the maximum profit so far
                profit = max(profit, curr_profit);
            }
        }
    }

    return profit;
}

// Driver code
int main()
{
    int price[] = { 100, 180, 260, 310, 40, 535, 695 };
    int n = sizeof(price) / sizeof(price[0]);

    printf("Max Profit is : %d\r\n", maxProfit(price, 0, n - 1));
    return 0;
}
