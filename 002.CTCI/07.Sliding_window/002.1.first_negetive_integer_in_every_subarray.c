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
    First negative integer in every window of size k

    Given an array A[] of size N and a positive integer K,
    find the first negative integer for each and every window(contiguous subarray) of size K.

    Example:

    Input :
        N = 5
        A[] = {-8, 2, 3, -6, 10}
        K = 2
    Output :
        -8 0 -6 -6
    Explanation :
        First negative integer for each window of size k
        {-8, 2} = -8
        {2, 3} = 0 (does not contain a negative integer)
        {3, -6} = -6
        {-6, 10} = -6
*/

void printNegetiveNumberEachWindow(int arr[], int n, int k)
{
    int i = 0, j = 0;
    while (j < n)
    {
        printf("i = %d, j = %d, k = %d, n = %d\r\n", i, j, k, n);

        // Calculation
        if(arr[j] < 0) {
            printf("\r\n%d\r\n\r\n", arr[j]);
        }

        if (j - i + 1 < k) {
            printf("j++ condition [j - i + 1 < k] : %d\r\n", j - i + 1);
            j++;
        } else if (j - i + 1 == k) {
            printf("j++ & i++ condition [j - i + 1 == k] : %d\r\n", j - i + 1);
            j++;
            i++;
        }
    }
    return;
}

// Driver code
int main()
{
    int arr[] = {12, -1, -7, 8, -15, 30, 16, 28};
    int k = 3;
    int n = sizeof(arr) / sizeof(arr[0]);
    printNegetiveNumberEachWindow(arr, n, k);
    return 0;
}