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
// https://youtu.be/excAOvwF_Wk

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 1e8

// One Buy and One Sell
int main (void)
{
    int stockPrice [] = {7, 1, 5, 3, 6, 4};
    int n = SIZEOF(stockPrice);

    int mini = stockPrice[0];
    int profit = 0;

    for (int i = 1; i < n; i++){
        int cost = stockPrice[i] - mini;
        profit = MAX(cost, profit);
        mini = MIN(mini, stockPrice[i]);
    }

    printf("The maximim profit is : %d \r\n", profit);

    return 0;
}