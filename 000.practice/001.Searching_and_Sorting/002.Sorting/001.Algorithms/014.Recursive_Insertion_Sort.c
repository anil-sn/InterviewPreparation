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
    The only difference between Insertion sort and Recursive Insertion Sort is
    that in the Recursive method, we start from placing the last element in its
    correct position in the sorted array instead of starting from the first.
*/

void rec_insertion(int arr[], int n)
{
    // When the elements are all over
    if (n <= 1)
        return;

    // sorting n-1 elements
    rec_insertion(arr, n - 1);

    int last = arr[n - 1];
    int j = n - 2;

    while (j >= 0 && last < arr[j]) {
        arr[j + 1] = arr[j];
        j--;
    }
    arr[j + 1] = last;

    printf("\nAfter performing Insertion sort:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
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

    rec_insertion(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}