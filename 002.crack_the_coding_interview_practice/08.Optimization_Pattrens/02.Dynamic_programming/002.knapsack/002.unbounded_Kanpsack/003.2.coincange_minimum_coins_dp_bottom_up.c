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
    Find minimum number of coins that make a given value

        Given a value V, if we want to make a change for V cents,
        and we have an infinite supply of each of C = { C1, C2, .., Cm} valued coins,
        what is the minimum number of coins to make the change?
        If it’s not possible to make a change, print -1.

    Examples:

        Input: coins[] = {25, 10, 5}, V = 30
        Output: Minimum 2 coins required
        We can use one coin of 25 cents and one of 5 cents

        Input: coins[] = {9, 6, 5, 1}, V = 11
        Output: Minimum 2 coins required
        We can use one coin of 6 cents and 1 coin of 5 cents
*/

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

int
printMatrix(int k[][12], int numberOfConins, int target) {
    for (int i = 0; i <= numberOfConins; i++){
        for (int j = 0; j <= target; j++) {
            printf("%8x ", k[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");
    //getchar();
}
/*
    Best thing of this question is you have to initialise DP table with INT_MAX-1 instead of INT_MAX.
    Reason: if we take INT_MAX as intialisation, then in
    dp[i][j] = min((dp[i][j-a[i-1]]+1), dp[i-1][j]),
    (dp[i][j-a[i-1]]+1) ==> this part become INT_MIN
    if case is not valid and min(INT_MIN, INT_MAX) = INT_MIN which affects our answer.
    INT_MAX = 2147483647[7fffffff], INT_MIN = -2147483648[80000000]
    INT_MAX+1 = -2147483648[80000000]
*/

int minCoins(int coins[], int numberOfConins, int target)
{
    int i, w;
    int dp[numberOfConins + 1][target + 1];
    int choiceIncludeItem;
    int choiceExcludeItem;

    for (int i = 0; i <= numberOfConins; i++){
        for (int j = 0; j <= target; j++) {
            if (i == 0) {
                dp[i][j] = INT_MAX -1;
            } else if (j == 0) {
                dp[i][j] = 0;
            } else if (i == 1) {
                if ( 0 == (j % coins[0])) {
                    dp[i][j] = j / coins[0];
                } else {
                    dp[i][j] = INT_MAX -1;
                }
            }
        }
    }

    printMatrix(dp, numberOfConins, target);

    // Build table K[][] in bottom up manner
    for (int i = 1; i <= numberOfConins; i++) {
        for (int j = 1; j <= target; j++) {
            if (coins[i - 1] <= target) {
                choiceIncludeItem = (dp[i][j-coins[i-1]]+1);
                choiceExcludeItem = dp[i-1][j];
                dp[i][j] = min(choiceIncludeItem, choiceExcludeItem);
            }
            else {
                choiceExcludeItem = dp[i - 1][w];
                dp[i][j] = choiceExcludeItem;
            }
            printMatrix(dp, numberOfConins, target);
        }
    }

    return dp[numberOfConins][target];
}

// Driver program to test above function
int main()
{
    int coins[] =  {2, 3, 3, 2, 1};
    int m = sizeof(coins)/sizeof(coins[0]);
    int V = 11;
    printf("Minimum coins required is %d \r\n", minCoins(coins, m, V));
    printf ("INT_MAX = %d[%x], INT_MIN = %d[%x] INT_MAX+1 = %d[%x]\r\n",INT_MAX,INT_MAX, INT_MIN, INT_MIN, INT_MAX+1, INT_MAX+1);
    return 0;
}

