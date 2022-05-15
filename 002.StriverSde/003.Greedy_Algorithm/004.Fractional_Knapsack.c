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
    Fractional Knapsack Problem : Greedy Approach
    Problem Statement: The weight of N items and their corresponding values are given.
    We have to put these items in a knapsack of weight W such that the total value obtained is maximized.

    Note: We can either take the item as a whole or break it into smaller units.

    Example:
    Input: N = 3, W = 50, values[] = {100,60,120}, weight[] = {20,10,30}.
    Output: 240.00

    Explanation: The first and second items  are taken as a whole
    while only 20 units of the third item is taken. Total value = 100 + 60 + 80 = 240.00
*/
typedef struct item_ {
    int value;
    int weight;
} ITEM;

int cmp(const void *a, const void *b)
{
    ITEM *x = (ITEM *)a;
    ITEM *y = (ITEM *)b;
    double r1 = (double) x->value / (double) x->weight;
    double r2 = (double) y->value / (double) y->weight;
    return r2 - r1;
}

float fractionalKnapsack(ITEM items[], int n, int capacity)
{
    int curWeight = 0;
    double finalvalue = 0.0;

    qsort(items, n, sizeof(ITEM), cmp);

    printf("Given Items for maxCapacity: %d\r\n", capacity);
    for (int i = 0; i < n; i++){
        printf("Value : %d, Weight : %d \r\n", items[i].value, items[i].weight);
    }

    for (int i = 0; i < n; i++) {
        printf("Consider Value : %d, Weight : %d \r\n", items[i].value, items[i].weight);
        if (curWeight + items[i].weight <= capacity) {
            curWeight += items[i].weight;
            finalvalue += items[i].value;
        } else {
            int remain = capacity - curWeight;
            finalvalue += (items[i].value / (double) items[i].weight) * (double)remain;
            printf("Factional Value : %f, Weight : %d \r\n", (items[i].value / (double) items[i].weight) * (double)remain,
                                                            remain);
            break;
        }
    }

    return finalvalue;
}

int main ()
{
    ITEM items[] = {{100, 20}, {60, 10}, {120, 30}};
    int n = sizeof(items)/sizeof(items[0]);
    int capacity = 50;

    float maxProfit = fractionalKnapsack(items, n, capacity);
    printf("Max Profit by adding items in knapsack is %f \r\n", maxProfit);

    return;
}