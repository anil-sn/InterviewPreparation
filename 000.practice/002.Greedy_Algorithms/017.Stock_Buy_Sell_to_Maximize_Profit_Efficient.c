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
// solution structure
typedef struct Interval {
    int buy;
    int sell;
}Interval;

// This function finds the buy sell schedule for maximum profit
void stockBuySell(int price[], int n)
{
    // Prices must be given for at least two days
    if (n == 1)
        return;

    int count = 0; // count of solution pairs

    // solution vector
    Interval sol[n / 2 + 1];

    // Traverse through given price array
    int i = 0;
    while (i < n - 1) {
        // Find Local Minima. Note that the limit is (n-2) as we are
        // comparing present element to the next element.
        while ((i < n - 1) && (price[i + 1] <= price[i]))
            i++;

        // If we reached the end, break as no further solution possible
        if (i == n - 1)
            break;

        // Store the index of minima
        sol[count].buy = i++;

        // Find Local Maxima.  Note that the limit is (n-1) as we are
        // comparing to previous element
        while ((i < n) && (price[i] >= price[i - 1]))
            i++;

        // Store the index of maxima
        sol[count].sell = i - 1;

        // Increment count of buy/sell pairs
        count++;
    }

    // print solution
    if (count == 0)
        printf("There is no day when buying the stock will make profit\r\n");
    else {
        for (int i = 0; i < count; i++)
            printf("Buy on day: %d\t Sell on day: %d\r\n", sol[i].buy, sol[i].sell);
    }

    return;
}

// Driver program to test above functions
int main()
{
    // stock prices on consecutive days
    int price[] = { 100, 180, 260, 310, 40, 535, 695 };
    int n = sizeof(price) / sizeof(price[0]);

    // function call
    printf("100, 180, 260, 310, 40, 535, 695\r\n");
    stockBuySell(price, n);

    return 0;
}