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
    Cycle sort is an in-place sorting Algorithm, unstable sorting algorithm,
    a comparison sort that is theoretically optimal in terms of the total number of
    writes to the original array.

    It is optimal in terms of number of memory writes.
    It minimizes the number of memory writes to sort
    (Each value is either written zero times, if it’s already in its correct position,
    or written one time to its correct position.)

    It is based on the idea that array to be sorted can be divided into cycles.
    Cycles can be visualized as a graph. We have n nodes and an edge directed from node
    i to node j if the element at i-th index must be present at j-th index in the sorted array.
    Cycle in arr[] = {2, 4, 5, 1, 3}

    Explanation :

        arr[] = {10, 5, 2, 3}
        index =  0   1   2   3
        cycle_start = 0
        item = 10 = arr[0]

        Find position where we put the item
        pos = cycle_start
        i=pos+1
        while(i<n)
        if (arr[i] < item)
            pos++;

        We put 10 at arr[3] and change item to
        old value of arr[3].
        arr[] = {10, 5, 2, 10}
        item = 3

        Again rotate rest cycle that start with index '0'
        Find position where we put the item = 3
        we swap item with element at arr[1] now
        arr[] = {10, 3, 2, 10}
        item = 5

        Again rotate rest cycle that start with index '0' and item = 5
        we swap item with element at arr[2].
        arr[] = {10, 3, 5, 10 }
        item = 2

        Again rotate rest cycle that start with index '0' and item = 2
        arr[] = {2, 3,  5, 10}

        Above is one iteration for cycle_stat = 0.
        Repeat above steps for cycle_start = 1, 2, ..n-2
*/

// Function sort the array using Cycle sort
void cycleSort(int arr[], int n)
{
    // count number of memory writes
    int writes = 0;

    // traverse array elements and put it to on
    // the right place
    for (int cycle_start = 0; cycle_start <= n - 2; cycle_start++) {
        // initialize item as starting point
        int item = arr[cycle_start];

        // Find position where we put the item. We basically
        // count all smaller elements on right side of item.
        int pos = cycle_start;
        for (int i = cycle_start + 1; i < n; i++)
            if (arr[i] < item)
                pos++;

        // If item is already in correct position
        if (pos == cycle_start)
            continue;

        // ignore all duplicate  elements
        while (item == arr[pos])
            pos += 1;

        // put the item to it's right position
        if (pos != cycle_start) {
            swap(&(item), &(arr[pos]));
            writes++;
        }

        // Rotate rest of the cycle
        while (pos != cycle_start) {
            pos = cycle_start;

            // Find position where we put the element
            for (int i = cycle_start + 1; i < n; i++)
                if (arr[i] < item)
                    pos += 1;

            // ignore all duplicate  elements
            while (item == arr[pos])
                pos += 1;

            // put the item to it's right position
            if (item != arr[pos]) {
                swap(&(item), &(arr[pos]));
                writes++;
            }
        }
    }

    // Number of memory writes or swaps
    // cout << writes << endl ;
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
    int arr[] = {12, 11, 13, 5, 6, 7, 3, 2, 1};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("\nUnSorted array is \n");
    printArray(arr, size);

    cycleSort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}