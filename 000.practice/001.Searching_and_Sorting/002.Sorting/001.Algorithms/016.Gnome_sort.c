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
    Gnome sort is a very simple, unstable, and in-place sorting algorithm.

    1. An unstable sorting algorithm is the one where two keys having equal values do not appear
       in the same order in the sorted output array as they are present in the input unsorted array.
    2. An in-place sorting algorithm has various definitions but a more used one is -
       An in-place sorting algorithm does not need extra space and uses the constant memory
       for manipulation of the input in-place. Although, it may require some extra constant
       space allowed for variables.

    Gnome sort is similar to the insertion sort algorithm as it works with one item
    at a time but the only difference is that in Gnome sort, we only swap the adjacent
    values much like in a Bubble sort.

    The sorting algorithm is inspired by a Garden Gnome sorting his flower pots.

        1. He looks at the pot next to him and the previous one. If they are in the right order,
           he moves one step forward, otherwise, he swaps them and moves one step backward.
        3. If there is no previous pot (at the start of the line), he moves one step forwards.
           And if there is no pot beside him (at the end of the line), the work is complete.

    Algorithm:

        At array index = 0, move one step backward or if array index = n-1, finish.
        If the element at the current position is bigger than the previous one, move one step to the right.
        Else swap the elements and move one step to the left.
        Repeat steps 2 - 3 till you reach the end of the line.

    Pseudo-code:
        Gnome_sort(arr):
        pos <- 0
        while pos < length(arr):
            if pos == 0 or arr[pos] >= arr[pos-1]:
                pos <- pos + 1
            else
                swap arr[pos] and arr[pos-1]
                pos <- pos – 1
            end if
        end while
*/

void gnome_sort(int arr[], int n)
{
    int pos = 0;

    while (pos < n) {
        if (pos == 0 || arr[pos] >= arr[pos - 1])
            pos++;
        else {
            swap(&arr[pos], &arr[pos - 1]);
            pos--;
        }
    }
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

    gnome_sort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}