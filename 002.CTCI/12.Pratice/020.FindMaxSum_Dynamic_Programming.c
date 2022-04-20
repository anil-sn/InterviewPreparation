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
    Dont practice until you get it right
    practice until you can't get it worng
*/

/*
    We can also solve this problem using the Dynamic Programming approach.

    Dynamic Programming Approach:
    Let’s decide the states of ‘dp’. Let dp[i] be the largest possible sum for
    the sub-array starting from index ‘i’ and ending at index ‘N-1’.

    Now, we have to find a recurrence relation between this state and a lower-order state.
    In this case for an index ‘i’, we will have two choices.

    1) Choose the current index:
        In this case, the relation will be dp[i] = arr[i] + dp[i+2]
    2) Skip the current index:
        Relation will be dp[i] = dp[i+1]

    We will choose the path that maximizes our result.
    Thus, the final relation will be:

    dp[i] = max(dp[i+2]+arr[i], dp[i+1])
*/

#define maxLen 10

// variable to store states of dp
int dp[maxLen];

// variable to check if a given state has been solved
bool v[maxLen];

int max(int a, int b) {
    return (a > b ? a : b);
}
// Function to find the maximum sum subsequence
// such that no two elements are adjacent
int maxSum(int arr[], int i, int n)
{
    // Base case
    if (i >= n)
        return 0;

    printf ("arr[%d] = %d isResolved[%s], dp[%d] = %d \r\n\r\n",i, arr[i],
                                                                (true == v[i]) ? "TRUE" : "FALSE",
                                                                i, dp[i]);

    // To check if a state has been solved
    if (v[i]) {
       return dp[i];
    }

    v[i] = true;

    // Required recurrence relation
    dp[i] = max(maxSum(arr, i + 1, n), arr[i] + maxSum(arr, i + 2, n));

    // Returning the value
    return dp[i];
}

// Driver code
int main()
{
    int arr[] = {5, 1, 10, 100, 10, 5};
    int n = sizeof(arr) / sizeof(int);

    printf ("[5, 1, 10, 100, 10, 5]\r\n\r\n");
    printf("So the Max Sum is : %d \r\n", maxSum(arr, 0, n));

    return 0;
}