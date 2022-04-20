#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    Given weights and values of n items, we need to put these items in a knapsack of
    capacity W to get the maximum total value in the knapsack.

    In the 0-1 Knapsack problem, we are not allowed to break items.
    We either take the whole item or don’t take it.

    Input:
        Items as (value, weight) pairs
        arr[] = {{60, 10}, {100, 20}, {120, 30}}
        Knapsack Capacity, W = 50;

    Output:
        Maximum possible value = 240
        by taking items of weight 10 and 20 kg and 2/3 fraction of 30 kg.
        Hence total price will be 60+100+(2/3)(120) = 240
*/

#define VALUE 0
#define WEIGHT 1

// Comparison function to sort Item according to val/weight ratio
int cmp(const void *x, const void *y)
{
    int *a = (int *)x;
    int *b = (int *)y;

    double r1 = (double)a[VALUE] / (double)a[WEIGHT];
    double r2 = (double)b[VALUE] / (double)b[WEIGHT];
    return r1 > r2;
}

// Main greedy function to solve problem
double fractionalKnapsack(int W, int arr[][2], int n)
{
    // sorting Item on basis of ratio
    qsort(arr, n, sizeof(arr[0]), cmp);

    //    Uncomment to see new order of Items with their ratio
    for (int i = 0; i < n; i++) {
        printf ("%d %d : %f\r\n", arr[i][VALUE], arr[i][WEIGHT], ((double)arr[i][VALUE] / arr[i][WEIGHT]));
    }

    int curWeight = 0; // Current weight in knapsack
    double finalvalue = 0.0; // Result (value in Knapsack)

    // Looping through all Items
    for (int i = 0; i < n; i++) {
        // If adding Item won't overflow, add it completely
        if (curWeight + arr[i][WEIGHT] <= W) {
            curWeight += arr[i][WEIGHT];
            finalvalue += arr[i][VALUE];
        }

        // If we can't add current Item, add fractional part
        // of it
        else {
            int remain = W - curWeight;
            finalvalue += arr[i][VALUE] * ((double)remain / (double)arr[i][WEIGHT]);
            break;
        }
    }

    // Returning final value
    return finalvalue;
}

// Driver code
int main()
{
    int W = 50; //    Weight of knapsack
    int arr[][2] = { { 60, 10 }, { 100, 20 }, { 120, 30 } };

    int n = 3;

    // Function call
    printf("Maximum value we can obtain = %f", fractionalKnapsack(W, arr, n));
    return 0;
}