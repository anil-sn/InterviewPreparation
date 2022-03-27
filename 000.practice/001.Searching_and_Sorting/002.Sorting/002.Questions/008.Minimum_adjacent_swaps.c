
#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Swap two integers
void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

/*
    Given N number of elements, find the minimum number of swaps required so that the maximum
    element is at the beginning and the minimum element is at last with the condition that only
    swapping of adjacent elements is allowed.

    Examples:

    Input: a[] = {3, 1, 5, 3, 5, 5, 2}
    Output: 6
    Step 1: Swap 5 with 1 to make the array as {3, 5, 1, 3, 5, 5, 2}
    Step 2: Swap 5 with 3 to make the array as {5, 3, 1, 3, 5, 5, 2}
    Step 3: Swap 1 with 3 at its right to make the array as {5, 3, 3, 1, 5, 5, 2}
    Step 4: Swap 1 with 5 at its right to make the array as {5, 3, 3, 5, 1, 5, 2}
    Step 5: Swap 1 with 5 at its right to make the array as {5, 3, 3, 5, 5, 1, 2}
    Step 6: Swap 1 with 2 at its right to make the array as {5, 3, 3, 5, 5, 2, 1}
    After performing 6 swapping operations 5 is at the beginning and 1 at the end
    Input: a[] = {5, 6, 1, 3}
    Output: 2

    The approach will be to find the index of the largest element(let l).
    Find the index of the leftmost largest element if largest element appears in the array
    more than once. Similarly, find the index of the rightmost smallest element(let r).
    There exists two cases to solve this problem.


    Case 1: If l < r: Number of swaps = l + (n-r-1)
    Case 2: If l > r: Number of swaps = l + (n-r-2),
            as one swap has already been performed while swapping the
            larger element to the front

*/
// Function that returns the minimum swaps
void solve(int a[], int n)
{
    int maxx = -1, minn = a[0], l = 0, r = 0;
    for (int i = 0; i < n; i++) {

        // Index of leftmost largest element
        if (a[i] > maxx) {
            maxx = a[i];
            l = i;
        }

        // Index of rightmost smallest element
        if (a[i] <= minn) {
            minn = a[i];
            r = i;
        }
    }

    if (r < l)
        printf("%d\r\n", l + (n - r - 2));
    else
        printf("%d\r\n", l + (n - r - 1));
}

// A utility function to print a given array of given size
void printArray(int *arr, int size) {
    int i;

    for (i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }

    printf("\r\n");
}

/* Driver program to test above functions */
int main() {
    int arr[] = {5, 6, 1, 3};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("\nSorted array is \n");
    printArray(arr, size);

    solve(arr, size);

    return 0;
}