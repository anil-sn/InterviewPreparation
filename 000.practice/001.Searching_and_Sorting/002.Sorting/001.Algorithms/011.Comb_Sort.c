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
    Comb Sort is mainly an improvement over Bubble Sort.
    Bubble sort always compares adjacent values. So all inversions are removed one by one.
    Comb Sort improves on Bubble Sort by using gap of size more than 1.
    The gap starts with a large value and shrinks by a factor of 1.3 in every iteration
    until it reaches the value 1. Thus Comb Sort removes more than one inversion counts
    with one swap and performs better than Bubble Sort.
*/

// To find gap between elements
int getNextGap(int gap)
{
    // Shrink gap by Shrink factor
    gap = (gap*10)/13;

    if (gap < 1)
        return 1;
    return gap;
}

// Function to sort a[0..n-1] using Comb Sort
void combSort(int a[], int n)
{
    // Initialize gap
    int gap = n;

    // Initialize swapped as true to make sure that
    // loop runs
    bool swapped = true;

    // Keep running while gap is more than 1 and last
    // iteration caused a swap
    while (gap != 1 || swapped == true)
    {
        // Find next gap
        gap = getNextGap(gap);

        // Initialize swapped as false so that we can
        // check if swap happened or not
        swapped = false;

        // Compare all elements with current gap
        for (int i=0; i<n-gap; i++)
        {
            if (a[i] > a[i+gap])
            {
                swap(&(a[i]), &(a[i+gap]));
                swapped = true;
            }
        }
    }
}

// A utility function to print a given array of given size
void printArray(int* arr, int size)
{
    int i;
    for (i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");
}

/* Driver program to test above functions */
int main()
{
    int arr[] = {12, 11, 13, 5, 6, 7, 3, 2, 1};
    int size = sizeof(arr)/sizeof(arr[0]);

    printf("\nUnSorted array is \n");
    printArray(arr, size);

    combSort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}