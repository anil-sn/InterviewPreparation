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
    Given weights and values of n items, put these items in a knapsack of capacity W to get the maximum
    total value in the knapsack. In other words, given two integer arrays val[0..n-1] and wt[0..n-1]
    which represent values and weights associated with n items respectively. Also given an integer W
    which represents knapsack capacity, find out the maximum value subset of val[] such that sum of the
    weights of this subset is smaller than or equal to W. You cannot break an item, either pick the complete
    item or don’t pick it (0-1 property).
*/

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

int
printMatrix(int k[][8], int n, int m, int a, int b, int choiceInclude, int choiceExclude) {
    printf("[%d][%d] : Value Array [ 1, 4, 5, 7 ] ", a, b);
    printf("Weight Array [ 1, 3, 4, 5 ] ");
    printf("Knapsack Capacity 7 [Include %d or Exclude %d]\r\n",choiceInclude,choiceExclude );

    for (int i = 0; i <=n; i++){
        for (int j = 0; j <= m; j++) {
            printf("%d ", k[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");
    getchar();
}

// Returns the maximum value that can be
// put in a knapsack of capacity W
// Returns the maximum value that
// can be put in a knapsack of capacity W

int knapSack(int W, int wt[], int val[], int n)
{
    int i, w;
    int K[n + 1][W + 1];
    int choiceIncludeItem;
    int choiceExcludeItem;

    for (int i = 0; i < n+1; i++){
        for (int j = 0; j < W+1; j++) {
            K[i][j] = 0;
        }
    }

    printMatrix(K, n, W, 0, 0, 0, 0);

    // Build table K[][] in bottom up manner
    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= W; w++)
        {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (wt[i - 1] <= w) {
                choiceIncludeItem = val[i - 1] + K[i - 1][w - wt[i - 1]];
                choiceExcludeItem = K[i - 1][w];
                K[i][w] = max(choiceIncludeItem, choiceExcludeItem);
                printMatrix(K, n, W, i, w, choiceIncludeItem, choiceExcludeItem);
            }
            else {
                choiceExcludeItem = K[i - 1][w];
                K[i][w] = choiceExcludeItem;
                printMatrix(K, n, W, i, w, 0, choiceExcludeItem);
            }
        }
    }
    return K[n][W];
}

// Driver program to test above function
int main()
{
    int val[] = { 1, 4, 5, 7 };
    int wt[] = { 1, 3, 4, 5 };
    int W = 7;
    int n = sizeof(val) / sizeof(val[0]);
    printf("Value Array [ 1, 4, 5, 7 ] \r\n");
    printf("Weight Array [ 1, 3, 4, 5 ] \r\n");
    printf("Knapsack Capacity 7 \r\n");

    printf("The maximum total value in the knapsack %d \r\n", knapSack(W, wt, val, n));
    return 0;
}