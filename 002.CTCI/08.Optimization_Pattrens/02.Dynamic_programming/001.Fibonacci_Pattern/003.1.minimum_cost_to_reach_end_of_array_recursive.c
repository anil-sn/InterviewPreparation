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
    Given an array arr[] of N integers and an integer K, one can move from an index i
    to any other j if j ≤ i + k. The cost of moving from one index i to the other index j
    is abs(arr[i] – arr[j]). Initially, we start from the index 0 and we need to reach the
    last index i.e. N – 1. The task is to reach the last index in the minimum cost possible.
        Examples:


        Input: arr[] = {10, 30, 40, 50, 20}, k = 3
        Output: 30
        0 -> 1 -> 4
        the total cost will be: |10-30| + |30-20| = 30

        Input: arr[] = {40, 10, 20, 70, 80, 10}, k = 4
        Output: 30

    This approach is based on Bellman ford’s DP solution to the single-source shortest path.
    In Bellman’s ford SSSP, the main idea is to find the next vertex through minimizing on edges,
    we can do the same we minimize on abs values of two elements of an array to find the next index.
    For solving any DP problems we first guess all the possible solutions to the subproblems and
    memoize them then choose the best solutions to the subproblem. we write Recurrence for the
    problem

    Recurrence: DP(j) = min{DP(i) + abs(A[i] – A[j])}
    where i is in [0, N-1] and j is in [i + 1, j + k + 1], and k is number of jumps allowed.
    this can also be compared with relaxtion in SSSP. We are going to relax every next
    approachable index.


    // base case
    memo[0] = 0;
    for (i = 0 to N-1)
    for (j = i+1 to i+k+1)
    memo[j] = min(memo[j], memo[i] + abs(A[i] – A[j]));
*/

int min(int a, int b){
    return (a < b ? a : b);
}

// Function to return the minimum cost
// to reach the last index
int FindMinimumCost(int ind, int a[], int n, int k, int dp[])
{

    // If we reach the last index
    if (ind == (n - 1))
        return 0;

    // Already visited state
    else if (dp[ind] != -1)
        return dp[ind];
    else {

        // Initially maximum
        int ans = INT_MAX;

        // Visit all possible reachable index
        for (int i = 1; i <= k; i++) {

            // If inside range
            if (ind + i < n)
                ans = min(ans, abs(a[ind + i] - a[ind]) + FindMinimumCost(ind + i, a, n, k, dp));

            // We cannot move any further
            else
                break;
        }

        // Memoize
        return dp[ind] = ans;
    }
}

// Driver Code
int main()
{
    int a[] = { 10, 30, 40, 50, 20 };
    int k = 3;
    int n = sizeof(a) / sizeof(a[0]);
    int dp[n];

    memset(dp, -1, sizeof dp);
    printf("Minimum Csot to reach end of array is %d\r\n",FindMinimumCost(0, a, n, k, dp));

    return 0;
}