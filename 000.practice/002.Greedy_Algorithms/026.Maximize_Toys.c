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
    Given an array arr[ ] of length N consisting cost of N toys and an integer K
    depicting the amount with you. Your task is to find maximum number of toys
    you can buy with K amount.

    Example 1:
        Input:
            N = 7
            K = 50
            arr[] = {1, 12, 5, 111, 200, 1000, 10}
        Output: 4
        Explaination: The costs of the toys you can buy are 1, 12, 5 and 10.

    Example 2:
        Input:
            N = 3
            K = 100
            arr[] = {20, 30, 50}
        Output: 3
        Explaination: You can buy all toys.
*/

int cmp (const void *a, const void *b){
    int *x = (int *)a;
    int *y = (int *)b;

    return (*x > *y);
}

// This functions returns the required
// number of toys
int maximum_toys(int cost[], int N, int K)
{
    int count = 0, sum = 0;

    // sort the cost array
    qsort(cost, N, sizeof(cost[0]), cmp);

    for (int i = 0; i < N; i++) {

        // Check if we can buy ith toy or not
        if (sum +cost[i] <= K)
        {
            sum = sum + cost[i];

            // Increment count
            count++;
        }
    }

    return count;
}

// Driver Code
int main()
{
    int K = 50;
    int cost[] = { 1, 12, 5, 111, 200, 1000, 10, 9, 12, 15 };
    int N = sizeof(cost) / sizeof(cost[0]);

    printf("Maximum number of toys bought : %d\r\n", maximum_toys(cost, N, K));
    return 0;
}