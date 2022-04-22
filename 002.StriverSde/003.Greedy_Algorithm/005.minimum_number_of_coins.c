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
    Find minimum number of coins
    Problem Statement: Given a value V, if we want to make a change for V Rs,
        and we have an infinite supply of each of the denominations in Indian currency,
        i.e., we have an infinite supply of { 1, 2, 5, 10, 20, 50, 100, 500, 1000} valued coins/notes,
        what is the minimum number of coins and/or notes needed to make the change.

    Examples:
        Example 1:
            Input: V = 70
            Output: 2
            Explaination: We need a 50 Rs note and a 20 Rs note.

        Example 2:
            Input: V = 121
            Output: 3
            Explaination: We need a 100 Rs note, a 20 Rs note and a 1 Rs coin.
*/

int main()
{
    int V = 49;
    int ans[1000] = {-1};
    int pos = 0;
    int coins[] = {1, 2, 5, 10, 20, 50, 100, 500, 1000};
    int n = 9;

    for (int i = n - 1; i >= 0; i--) {
        while (V >= coins[i]) {
            V -= coins[i];
            ans[pos++] = coins[i];
        }
    }

    printf("The minimum number of coins is : %d \r\n", pos);
    printf("The coins are : ");
    for (int i = 0; i < pos; i++) {
        printf("%d ", ans[i]);
    }
    printf("\r\n");

    return 0;
}