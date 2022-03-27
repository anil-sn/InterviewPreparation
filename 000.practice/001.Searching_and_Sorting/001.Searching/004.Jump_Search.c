#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "math.h"
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
    Jump Search is a searching algorithm for sorted arrays. The basic idea is to check fewer
    elements (than linear search) by jumping ahead by fixed steps or skipping some elements
    in place of searching all elements.

    Let’s consider the following array: (0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610). The length of the array is 16. Jump search will find the value of 55 with the following steps assuming that the block size to be jumped is 4.
    STEP 1: Jump from index 0 to index 4;
    STEP 2: Jump from index 4 to index 8;
    STEP 3: Jump from index 8 to index 12;
    STEP 4: Since the element at index 12 is greater than 55 we will
            jump back a step to come to index 8.
    STEP 5: Perform a linear search from index 8 to get the element 55.
*/
int search (int arr[], int n, int x) {

    // Finding block size to be jumped
    int step = sqrt(n);

    // Finding the block where element is present (if it is present)
    int prev = 0;

    while (arr[min(step, n)-1] < x)
    {
        prev = step;
        step += sqrt(n);
        if (prev >= n)
            return -1;
    }

    // Doing a linear search for x in block beginning with prev.
    while (arr[prev] < x)
    {
        prev++;

        // If we reached next block or end of array, element is not present.
        if (prev == min(step, n))
            return -1;
    }

    // If element is found
    if (arr[prev] == x)
        return prev;

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