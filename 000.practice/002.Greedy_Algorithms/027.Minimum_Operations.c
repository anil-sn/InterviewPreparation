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
    Minimize operations required to obtain N

    Given an integer N, the task is to obtain N, starting from 1 using minimum number of
    operations. The operations that can be performed in one step are as follows:

    Multiply the number by 2.
    Multiply the number by 3.
    Add 1 to the number.

    Explanation:

    Input: N = 5
    Output: 3
    Explanation: Starting value: x = 1
        Multiply x by 2 : x = 2
        Multiply x by 2 : x = 4
        Add 1 to x : x = 5
        Therefore, the minimum number of operations required = 3

    Input: N = 15
    Output: 4
    Explanation:
        3 operations required to obtain x = 5.
        Multiply x by 3 : x = 15.
        Therefore, the minimum number of operations required = 4

    The idea is to use the concept of Dynamic Programming. Follow the steps below:

    If minimum operations to obtain any number smaller than N is known,
    then minimum operations to obtain N can be calculated.
    Create the following lookup table:
    dp[i] = Minimum number of operations to obtain i from 1

    So for any number x, minimum operations required to obtain x can be calculated as:
    dp[x] = min(dp[x-1], dp[x/2], dp[x/3])
*/

// Function to find the minimum number
// of operations
int minOperations(int n)
{
    // Storing the minimum operations
    // to obtain all numbers up to N
    int dp[n + 1];

    // Initilal state
    dp[1] = 0;

    // Iterate for the remaining numbers
    for (int i = 2; i <= n; i++) {

        dp[i] = INT_MAX;

        // If the number can be obtained by multiplication by 2
        if (i % 2 == 0) {
            int x = dp[i / 2];
            if (x + 1 < dp[i]) {
                dp[i] = x + 1;
            }
        }
        // If the number can be obtained by multiplication by 3
        if (i % 3 == 0) {
            int x = dp[i / 3];
            if (x + 1 < dp[i]) {
                dp[i] = x + 1;
            }
        }

        // Obtaining the number by adding 1
        int x = dp[i - 1];
        if (x + 1 < dp[i]) {
            dp[i] = x + 1;
        }
    }

    // Return the minm operations
    // to obtain n
    return dp[n];
}

// Driver Code
int main()
{
    int n = 15;
    printf(" Minimum number of operation = %d\r\n", minOperations(n));
    return 0;
}