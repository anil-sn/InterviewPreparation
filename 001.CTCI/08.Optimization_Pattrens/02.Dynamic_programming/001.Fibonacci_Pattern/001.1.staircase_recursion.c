#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

//Count ways to reach the n’th stair
    /*
        There are n stairs, a person standing at the bottom wants to reach the top.
        The person can climb either 1 stair or 2 stairs at a time.
        Count the number of ways, the person can reach the top.

    Examples:

        Input: n = 1
        Output: 1
        There is only one way to climb 1 stair

        Input: n = 2
        Output: 2
        There are two ways: (1, 1) and (2)

        Input: n = 4
        Output: 5
        (1, 1, 1, 1), (1, 1, 2), (2, 1, 1), (1, 2, 1), (2, 2)
*/

// A simple recursive program to
// find n'th fibonacci number
int fib(int n)
{
    if (n <= 1)
        return n;

    return fib(n - 1) + fib(n - 2);
}

// Returns number of ways to reach s'th stair
int countWays(int s)
{
    return fib(s + 1);
}

// Driver program to test above functions
int main()
{
    int s = 4;
    printf("Number of ways = %d\r\n", countWays(s));
    return 0;
}