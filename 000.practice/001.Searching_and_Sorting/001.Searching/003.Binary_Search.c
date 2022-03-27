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
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }
// Swap two integers
void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

//  Given an array arr[] of n elements, write a function to search a given element x in arr[].
/*
Step-by-step Binary Search Algorithm:
    We basically ignore half of the elements just after one comparison.

    1. Compare x with the middle element.
    2. If x matches with the middle element, we return the mid index.
    3. Else If x is greater than the mid element, then x can only lie in the right half
       subarray after the mid element. So we recur for the right half.
    4. Else (x is smaller) recur for the left half.
*/
int search (int arr[], int n, int x) {

    int l = 0;
    int r = n-1;
    int m = 0;

    while (l <= r) {

        m = l + (r - l) / 2;

        // Check if x is present at mid
        if (arr[m] == x)
            return m;

        // If x greater, ignore left half
        if (arr[m] < x)
            l = m + 1;

        // If x is smaller, ignore right half
        else
            r = m - 1;
    }

    return -1;
}

int main(void)
{
    int arr[] = { 2, 3, 4, 10, 40 };
    int x = 10;
    int n = sizeof(arr) / sizeof(arr[0]);

    // Function call
    int result = search(arr, n, x);

    printf ("%s:%d\r\n", (result == -1) ? "Element is not present in array" : "Element is present at index ", result);
    return 0;
}