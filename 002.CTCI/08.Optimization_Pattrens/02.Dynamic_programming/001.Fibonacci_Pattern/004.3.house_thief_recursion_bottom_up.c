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
Find maximum possible stolen value from houses

There are n houses build in a line, each of which contains some value in it.
A thief is going to steal the maximal value of these houses,
but he can’t steal in two adjacent houses because the owner of the stolen houses
will tell his two neighbors left and right side. What is the maximum stolen value?

Examples:


    Input: hval[] = {6, 7, 1, 3, 8, 2, 4}
        Output: 19

    Explanation: The thief will steal 6, 1, 8 and 4 from the house.

    Input: hval[] = {5, 3, 4, 11, 2}
        Output: 16

    Explanation: Thief will steal 5 and 11
*/
int max(int a, int b) {
    return (a > b ? a : b);
}

// calculate the maximum stolen value
int maxLoot(int *hval, int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return hval[0];
    if (n == 2)
        return max(hval[0], hval[1]);

    // dp[i] represent the maximum value stolen
    // so far after reaching house i.
    int dp[n];

    // Initialize the dp[0] and dp[1]
    dp[0] = hval[0];
    dp[1] = max(hval[0], hval[1]);

    // Fill remaining positions
    for (int i = 2; i<n; i++)
        dp[i] = max(hval[i] + dp[i-2], dp[i-1]);

    return dp[n-1];
}

// Driver to test above code
int main()
{
    int hval[] = {6, 7, 1, 3, 8, 2, 4};
    int n = sizeof(hval)/sizeof(hval[0]);
    printf("Maximum loot possible : %d\r\n", maxLoot(hval, n));
    return 0;
}