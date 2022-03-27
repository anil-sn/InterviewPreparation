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
    Given two integers N and K, the task is to find the number of ways to place K bishops
    on an N × N chessboard so that no two bishops attack each other.

    Input: N = 2, K = 2
    Output: 4
    ┌─────────┬─────────┐  ┌─────────┬─────────┐  ┌─────────┬─────────┐  ┌─────────┬─────────┐
    │         │         │  │         │         │  │         │         │  │         │         │
    │   XX    │         │  │   XX    │   XX    │  │         │   XX    │  │         │         │
    │         │         │  │         │         │  │         │         │  │         │         │
    │         │         │  │         │         │  │         │         │  │         │         │
    ├─────────┼─────────┤  ├─────────┼─────────┤  ├─────────┼─────────┤  ├─────────┼─────────┤
    │         │         │  │         │         │  │         │         │  │         │         │
    │   XX    │         │  │         │         │  │         │         │  │   XX    │    XX   │
    │         │         │  │         │         │  │         │   XX    │  │         │         │
    │         │         │  │         │         │  │         │         │  │         │         │
    └─────────┴─────────┘  └─────────┴─────────┘  └─────────┴─────────┘  └─────────┴─────────┘

    Input: N = 4, K = 3
    Output: 232

    Approach: This problem can be solved using dynamic programming.

    o   Let dp[i][j] denote the number of ways to place j bishops on diagonals with indices up to i
        which have the same color as diagonal i.
        Then i = 1...2N-1 and j = 0...K.
    o   We can calculate dp[i][j] using only values of dp[i-2]
        (we subtract 2 because we only consider diagonals of the same color as i).
        There are two ways to get dp[i][j].
            Either we place all j bishops on previous diagonals:
                then there are dp[i-2][j] ways to achieve this.
            Or we place one bishop on diagonal i and j-1 bishops on previous diagonals.
                The number of ways to do this equals the number of squares in diagonal i – (j – 1),
                because each of j-1 bishops placed on previous diagonals will block one square on
                the current diagonal.
    o The base case is simple: dp[i][0] = 1, dp[1][1] = 1.
    o Once we have calculated all values of dp[i][j],
        the answer can be obtained as follows:
        consider all possible numbers of bishops placed on black diagonals i=0…K,
        with corresponding numbers of bishops on white diagonals K-i.
        The bishops placed on black and white diagonals never attack each other,
        so the placements can be done independently.
        The index of the last black diagonal is 2N-1, the last white one is 2N-2.
        For each i we add dp[2N-1][i] * dp[2N-2][K-i] to the answer.
*/

// returns the number of squares in diagonal i
int squares(int i)
{

    if ((i & 1) == 1)
        return i / 4 * 2 + 1;
    else
        return (i - 1) / 4 * 2 + 2;
}

// returns the number of ways to fill a
// n * n chessboard with k bishops so
// that no two bishops attack each other.
long bishop_placements(int n, int k)
{
    /* return 0 if the number of valid places to be
     * filled is less than the number of bishops
     */
    if (k > 2 * n - 1)
        return 0;

    // dp table to store the values
    long dp[n * 2][k + 1];

    // Setting the base conditions
    for(int i = 0; i < n * 2; i++)
    {
        for(int j = 0; j < k + 1; j++)
        {
            dp[i][j] = 0;
        }

    }
    for (int i = 0; i < n * 2; i++)
        dp[i][0] = 1;
    dp[1][1] = 1;

    // calculate the required number of ways
    for (int i = 2; i < n * 2; i++)
    {
        for (int j = 1; j <= k; j++)
        {
            dp[i][j] = dp[i - 2][j]
                    + dp[i - 2][j - 1] * (squares(i) - j + 1);

        }
    }

    // stores the answer
    long ans = 0;
    for (int i = 0; i <= k; i++)
    {
        ans += dp[n * 2 - 1][i] * dp[n * 2 - 2][k - i];
    }

    return ans;
}

// Driver code
int main()
{
    int n = 2;
    int k = 2;
    long ans = bishop_placements(n, k);
    printf("\r\n%d Bishops can be placed in %dx%d board in %ld ways \r\n",k, n, n, bishop_placements(n, k));
    printf ("\r\n"
    "┌─────────┬─────────┐  ┌─────────┬─────────┐  ┌─────────┬─────────┐  ┌─────────┬─────────┐\r\n"
    "│         │         │  │         │         │  │         │         │  │         │         │\r\n"
    "│   XX    │         │  │   XX    │   XX    │  │         │   XX    │  │         │         │\r\n"
    "│         │         │  │         │         │  │         │         │  │         │         │\r\n"
    "│         │         │  │         │         │  │         │         │  │         │         │\r\n"
    "├─────────┼─────────┤  ├─────────┼─────────┤  ├─────────┼─────────┤  ├─────────┼─────────┤\r\n"
    "│         │         │  │         │         │  │         │         │  │         │         │\r\n"
    "│   XX    │         │  │         │         │  │         │         │  │   XX    │    XX   │\r\n"
    "│         │         │  │         │         │  │         │   XX    │  │         │         │\r\n"
    "│         │         │  │         │         │  │         │         │  │         │         │\r\n"
    "└─────────┴─────────┘  └─────────┴─────────┘  └─────────┴─────────┘  └─────────┴─────────┘\r\n"
    "\r\n");
}