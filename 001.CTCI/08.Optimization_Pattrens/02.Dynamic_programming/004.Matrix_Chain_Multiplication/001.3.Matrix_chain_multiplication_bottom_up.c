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
int Minimum_MCM(int p[], int n)
{
    /* For simplicity of the program,
       one extra row and one
       extra column are allocated in m[][].
       0th row and 0th
       column of m[][] are not used */
    int m[n][n];
    memset(m, 0, sizeof(m));

    int i, j, k, L, count;
    int left;
    int right;
    int calculation;

    /* m[i, j] = Minimum number of scalar multiplications needed to compute the matrix
       A[i]A[i+1]...A[j] = A[i..j] where dimension of A[i]
       is p[i-1] x p[i] */

    // cost is zero when multiplying one matrix.
    for (i = 1; i < n; i++)
        m[i][i] = 0;

    // L is chain length.
    for (L = 1; L < n - 1 ; L++) {
        printf ("L = %d\r\n", L);
        for (i = 1; i < n - L; i++)
        {
            j = i + L;
            m[i][j] = INT_MAX;
            printf ("\ti = %d, j = %d\r\n", i, j);
            for (k = i; k <= j - 1; k++)
            {
                left = m[i][k];
                right = m[k + 1][j];
                calculation = p[i - 1] * p[k] * p[j];

                // count = cost/scalar multiplications
                count =  left + right + calculation;
                printf ("\t\tk = %d\r\n", k);
                printf ("\t\tm[i][k] = %d, m[k + 1][j] = %d, p[i - 1] * p[k] * p[j] = %d \r\n",
                        left,
                        right,
                        calculation);
                if (count < m[i][j])
                    m[i][j] = count;
            }
            printf("\r\n\t\t");
            for (int a = 0; a < n; a++){
                for (int b = 0; b < n; b++) {
                    printf("%4d", m[a][b]);
                }
                printf("\r\n\t\t");
            }
            printf("\r\n");
        }
    }

    return m[1][n - 1];
}

// Driver code
int main()
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("\r\nInput Matrix\r\n");
    printf("\r\n1 x 2, 2 x 3, 3 x 4, 4 x 5 \r\n\r\n");
    printf("\r\n Minimum number of multiplications is %d \r\n\r\n", Minimum_MCM(arr, n));

    return 0;
}