
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
The Fibonacci numbers are the numbers in the following integer sequence.
0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, ……..

In mathematical terms, the sequence Fn of Fibonacci numbers is defined by
the recurrence relation

    Fn = Fn-1 + Fn-2
    with seed values

    F0 = 0 and F1 = 1.

Given a number n, print n-th Fibonacci Number.

Examples:
    Input  : n = 2
    Output : 1

    Input  : n = 9
    Output : 34

    Time Complexity: T(n) = T(n) which is linear.
*/

int fib(int n) {
    /* Declare an array to store Fibonacci numbers. */
    int f[n+2];   // 1 extra to handle case, n = 0
    int i;

    /* 0th and 1st number of the series are 0 and 1*/
    f[0] = 0;
    f[1] = 1;

    for (i = 2; i <= n; i++) {
        /* Add the previous 2 numbers in the series
           and store it */
        f[i] = f[i-1] + f[i-2];
    }

    return f[n];
}

int main () {
    printf ("Fibonacci value of %d is %d\r\n", 2, fib(2));
    printf ("Fibonacci value of %d is %d\r\n", 9, fib(9));
}