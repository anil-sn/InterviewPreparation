
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

one more interesting recurrence formula that can be used
to find n’th Fibonacci Number in O(Log n) time.

If n is even then k = n/2:
F(n) = [2*F(k-1) + F(k)]*F(k)

If n is odd then k = (n + 1)/2
F(n) = F(k)*F(k) + F(k-1)*F(k-1)

Taking determinant on both sides, we get

(-1)n = Fn+1Fn-1 - Fn2

Moreover, since AnAm = An+m for any square matrix A,
the following identities can be derived (they are obtained
form two different coefficients of the matrix product)

FmFn + Fm-1Fn-1 = Fm+n-1         ---------------------------(1)

By putting n = n+1 in equation(1),
FmFn+1 + Fm-1Fn = Fm+n             --------------------------(2)

Putting m = n in equation(1).
F2n-1 = Fn2 + Fn-12
Putting m = n in equation(2)

F2n = (Fn-1 + Fn+1)Fn = (2Fn-1 + Fn)Fn (Source: Wiki)
( By putting Fn+1 = Fn + Fn-1 )
To get the formula to be proved, we simply need to do the following
If n is even, we can put k = n/2
If n is odd, we can put k = (n+1)/2

*/
const int MAX = 1000;

// Returns n'th fibonacci number using table f[]
int fib(int f[MAX], int n)
{
    // Base cases
    if (n == 0)
        return 0;
    if (n == 1 || n == 2)
        return (f[n] = 1);

    // If fib(n) is already computed
    if (f[n])
        return f[n];

    int k = (n & 1)? (n+1)/2 : n/2;

    // Applying above formula [Note value n&1 is 1
    // if n is odd, else 0.
    f[n] = (n & 1)? (fib(f, k)*fib(f, k) + fib(f, k-1)*fib(f, k-1))
           : (2*fib(f, k-1) + fib(f, k))*fib(f, k);

    return f[n];
}

/* Driver program to test above function */
int main() {
    // Create an array for memoization
    int f[MAX];
    memset(f, 0, sizeof(f));

    int n = 2;
    printf("%d\n", fib(f, n));

    n = 9;
    printf("%d\n", fib(f, n));

    return 0;
}