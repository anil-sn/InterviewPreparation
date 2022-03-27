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

/*

The selection sort algorithm sorts an array by repeatedly finding the minimum element
    (considering ascending order) from unsorted part and putting it at the beginning.

The algorithm maintains two subarrays in a given array.
    1) The subarray which is already sorted.
    2) Remaining subarray which is unsorted.

In every iteration of selection sort, the minimum element
(considering ascending order) from the unsorted subarray is picked and moved to the sorted subarray.
Following example explains the above steps:
    arr[] = 64 25 12 22 11

    // Find the minimum element in arr[0...4]
    // and place it at beginning
    11 25 12 22 64

    // Find the minimum element in arr[1...4]
    // and place it at beginning of arr[1...4]
    11 12 25 22 64

    // Find the minimum element in arr[2...4]
    // and place it at beginning of arr[2...4]
    11 12 22 25 64

    // Find the minimum element in arr[3...4]
    // and place it at beginning of arr[3...4]
    11 12 22 25 64


    arr[] = 64 25 12 22 11

    // Find the minimum element in arr[0...4]
    // and place it at beginning
    11 25 12 22 64

    // Find the minimum element in arr[1...4]
    // and place it at beginning of arr[1...4]
    11 12 25 22 64

    // Find the minimum element in arr[2...4]
    // and place it at beginning of arr[2...4]
    11 12 22 25 64

    // Find the minimum element in arr[3...4]
    // and place it at beginning of arr[3...4]
    11 12 22 25 64

*/

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n\n");
}

// Driver program to test above functions
int main()
{
    int arr[] = {64, 25, 12, 22, 11};
    int n = sizeof(arr)/sizeof(arr[0]);

    printf("Unsorted array: \n");
    printArray(arr, n);

    selectionSort(arr, n);

    printf("Sorted array: \n");
    printArray(arr, n);

    return 0;
}