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
    Minimize the sum of product of two arrays with permutations allowed
    Given two arrays, A and B, of equal size n,
    the task is to find the minimum value of
    A[0] * B[0] + A[1] * B[1] +…+ A[n-1] * B[n-1].
    Shuffling of elements of arrays A and B is allowed.

    Examples :

    Input : A[] = {3, 1, 1} and B[] = {6, 5, 4}.
    Output : 23
    Minimum value of S = 1*6 + 1*5 + 3*4 = 23.

    Input : A[] = { 6, 1, 9, 5, 4 } and B[] = { 3, 4, 8, 2, 4 }
    Output : 80.
    Minimum value of S = 1*8 + 4*4 + 5*4 + 6*3 + 9*2 = 80.

    The idea is to multiply minimum element of one array to
    maximum element of another array.

    Algorithm to solve this problem:

    Sort both the arrays A and B.
    Traverse the array and for each element,
    multiply A[i] and B[n – i – 1] and add to the total.

    Note: We are adding multiplication of elements which can lead to overflow conditions.
*/
int cmp (const void *x, const void *y){
    int *a = (int *)x;
    int *b = (int *)y;

    return *a > *b;
}

// Returns minimum sum of product of two arrays with permutations allowed
long long int minValue(int A[], int B[], int n)
{
    // Sort A and B so that minimum and maximum
    // value can easily be fetched.
    qsort(A, n, sizeof(A[0]), cmp);
    qsort(B, n, sizeof(B[0]), cmp);

    // Multiplying minimum value of A and maximum
    // value of B
    long long int result = 0;
    for (int i = 0; i < n; i++)
        result += (A[i] * B[n - i - 1]);

    return result;
}

// Driven Code
int main()
{
    int A[] = { 3, 1, 1 };
    int B[] = { 6, 5, 4 };
    int n = sizeof(A) / sizeof(A[0]);
    printf("Minimum Product sum is %lld\r\n", minValue(A, B, n));
    return 0;
}