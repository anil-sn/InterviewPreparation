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

    Approach:

    If we are allowed to buy and sell only once, then we can use following algorithm.
    Maximum difference between two elements. Here we are allowed to buy and sell multiple times.
    Following is the algorithm for this problem.

    Find the local minima and store it as starting index. If not exists, return.
    Find the local maxima. and store it as an ending index.
    If we reach the end, set the end as the ending index.
    Update the solution (Increment count of buy-sell pairs)
    Repeat the above steps if the end is not reached.
*/
// Function that return
int stockBuySell(int* prices, int size)
{
    // maxProfit adds up the difference between
    // adjacent elements if they are in increasing order
    int maxProfit = 0;
    // The loop starts from 1
    // as its comparing with the previous
    for (int i = 1; i < size; i++) {
        if (prices[i] > prices[i - 1]) {
            maxProfit += prices[i] - prices[i - 1];
            printf("i:%d, i-1:%d, diff:%d, MaxProfit:%d\r\n",
                    i, i-1, prices[i] - prices[i - 1], maxProfit);
        }
    }

    return maxProfit;
}

// Driver program to test above functions
int main()
{
    // stock prices on consecutive days
    int price[] = { 100, 180, 260, 310, 40, 535, 695 };
    int n = sizeof(price) / sizeof(price[0]);

    // function call
    printf("100, 180, 260, 310, 40, 535, 695\r\n");    ;
    printf("Max Profit is : %d\r\n", stockBuySell(price, n));

    return 0;
}