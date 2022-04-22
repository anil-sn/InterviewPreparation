#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Stock Buy And Sell
    Problem Statement: You are given an array of prices where prices[i] is the price of a
    given stock on an ith day.
    You want to maximize your profit by choosing a single day to buy one stock and
    choosing a different day in the future to sell that stock. Return the maximum profit
    you can achieve from this transaction. If you cannot achieve any profit, return 0.

    Examples:
        Example 1:
            Input: prices = [7,1,5,3,6,4]
            Output: 5
            Explanation: Buy on day 2 (price = 1) and sell on day 5 (price = 6),
            profit = 6-1 = 5.
            Note: That buying on day 2 and selling on day 1 is not allowed
            because you must buy before you sell.

        Example 2:
            Input: prices = [7,6,4,3,1]
            Output: 0
            Explanation: In this case, no transactions are done and the max profit = 0.
*/
int max(int a, int b){
    return ((a > b) ? a : b);
}

int min(int a, int b){
    return ((a < b) ? a : b);
}

int maxProfit(int arr[], int n) {

    int maxPro = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[i]) {
                maxPro = max(arr[j] - arr[i], maxPro);
                printf("Max Candiate So Far: [%d]%d <-> [%d]%d : %d\r\n", i, arr[i], j, arr[j], maxPro);
            }
        }
    }

    return maxPro;
}

int maxProfit2(int arr[], int n) {
    int maxPro = 0;
    int minPrice = INT_MAX;

    for (int i = 0; i < n; i++) {
        minPrice = min(minPrice, arr[i]);
        maxPro = max(maxPro, arr[i] - minPrice);
    }
    return maxPro;
}

int main() {

    int arr[] = {7,1,5,3,6,4};
    int n = sizeof(arr)/sizeof(arr[0]);

    int maxPro = maxProfit(arr, n);
    printf("Max profit is: %d \r\n",maxPro);

    maxPro = maxProfit2(arr, n);
    printf("Max profit2 is: %d \r\n",maxPro);

    return;
}