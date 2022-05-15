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
    Given a sequence of matrices, find the most efficient way to multiply these matrices together.
    The problem is not actually to perform the multiplications, but merely to decide in which order
    to perform the multiplications.
    We have many options to multiply a chain of matrices because matrix multiplication is associative.
    In other words, no matter how we parenthesize the product, the result will be the same.
    For example, if we had four matrices A, B, C, and D, we would have:

    (ABC)D = (AB)(CD) = A(BCD) = ....
    However, the order in which we parenthesize the product affects the number of simple
    arithmetic operations needed to compute the product, or the efficiency.
    For example, suppose A is a 10 × 30 matrix, B is a 30 × 5 matrix, and C is a 5 × 60 matrix.

    Then,

        (AB)C = (10×30×5) + (10×5×60) = 1500 + 3000 = 4500 operations
        A(BC) = (30×5×60) + (10×30×60) = 9000 + 18000 = 27000 operations.

    Given an array p[] which represents the chain of matrices such that the ith matrix Ai
    is of dimension p[i-1] x p[i]. We need to write a function MatrixChainOrder() that
    should return the minimum number of multiplications needed to multiply the chain.

    Input: p[] = {40, 20, 30, 10, 30}
    Output: 26000
        There are 4 matrices of dimensions 40x20, 20x30, 30x10 and 10x30.
        Let the input 4 matrices be A, B, C and D.  The minimum number of
        multiplications are obtained by putting parenthesis in following way
        (A(BC))D --> 20*30*10 + 40*20*10 + 40*10*30

    Input: p[] = {10, 20, 30, 40, 30}
    Output: 30000
        There are 4 matrices of dimensions 10x20, 20x30, 30x40 and 40x30.
        Let the input 4 matrices be A, B, C and D.  The minimum number of
        multiplications are obtained by putting parenthesis in following way
        ((AB)C)D --> 10*20*30 + 10*30*40 + 10*40*30

    Input: p[] = {10, 20, 30}
    Output: 6000
        There are only two matrices of dimensions 10x20 and 20x30. So there
        is only one way to multiply the matrices, cost of which is 10*20*30
*/

// Matrix Ai has dimension p[i-1] x p[i] for i = 1..n
int Minimum_MCM(int p[], int i, int j)
{
    /* Zero Elememts, no matrix can be formed */
    if (i > j) {
        return 0;
    }

    /* One Elememts, no matrix can be formed */
    if (i == j) {
        return 0;
    }

    int k;
    int result = INT_MAX;
    int count;
    int left;
    int right;
    int calculation;

    // place parenthesis at different places between first and last matrix,
    // recursively calculate count of multiplications for each parenthesis placement
    // and return the minimum count
    for (k = i; k < j; k++)
    {
        left = Minimum_MCM(p, i, k);
        right = Minimum_MCM(p, k + 1, j);
        calculation = p[i - 1] * p[k] * p[j];

        count =  left + right + calculation;

        if (count < result) {
            printf ("%d x %d , %d x %d : %d\r\n", p[i - 1], p[k], p[k], p[j], count);
            result = count;
        }
    }

    // Return minimum count
    return result;
}

// Driver code
int main()
{
    int arr[] = { 1, 2, 3, 4, 3 };
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Minimum number of multiplications is %d \r\n", Minimum_MCM(arr, 1, n - 1));

    return 0;
}