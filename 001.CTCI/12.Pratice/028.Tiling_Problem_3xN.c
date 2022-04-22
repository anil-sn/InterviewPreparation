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
    Given a 3 x n board, find the number of ways to fill it with 2 x 1 dominoes.

    Example 1
        Following are all the 3 possible ways to fill up a 3 x 2 board.

        ┌─────┬─────┐    ┌───────────┐
        │     │     │    │           │      ┌──────────┐
        │     │     │    │           │      │          │
        │     │     │    ├─────┬─────┤      │          │
        │     │     │    │     │     │      ├──────────┤
        │     │     │    │     │     │      │          │
        │     │     │    │     │     │      │          │
        ├─────┴─────┤    │     │     │      ├──────────┤
        │           │    │     │     │      │          │
        │           │    │     │     │      │          │
        └───────────┘    └─────┴─────┘      └──────────┘

        ┌────────┬───────┬───────────────┬───────────────┬─────────────┐
        │        │       │               │               │             │
        │        │       │               │               │             │
        │        │       │               │               │             │
        │        │       ├────────┬──────┴───────┬───────┼─────────────┤
        │        │       │        │              │       │             │
        │        │       │        │              │       │             │
        │        │       │        │              │       │             │
        │        │       │        │              │       │             │
        │        │       │        ├──────────────┤       ├─────────────┤
        │        │       │        │              │       │             │
        ├────────┴───────┤        │              │       │             │
        │                │        │              │       │             │
        │                │        │              │       │             │
        │                │        │              │       │             │
        └────────────────┴────────┴──────────────┴───────┴─────────────┘

    Examples :
        Input : 2
        Output : 3

        Input : 8
        Output : 153

        Input : 12
        Output : 2131

    An =  No. of ways to completely fill a 3 x n board. (We need to find this)
    Bn =  No. of ways to fill a 3 x n board with top corner in last column not filled.
    Cn =  No. of ways to fill a 3 x n board with bottom corner in last column not filled.
*/

int countWays(int n)
{
    int A[n + 1], B[n + 1];

    A[0] = 1, A[1] = 0, B[0] = 0, B[1] = 1;

    for (int i = 2; i <= n; i++) {
        A[i] = A[i - 2] + 2 * B[i - 1];
        B[i] = A[i - 1] + B[i - 2];
    }

    return A[n];
}

// Driver Function
int main()
{
    printf("Number of ways 3x%d is %d\r\n", 4, countWays(4));
    printf("Number of ways 3x%d is %d\r\n", 8, countWays(8));

    return 0;
}