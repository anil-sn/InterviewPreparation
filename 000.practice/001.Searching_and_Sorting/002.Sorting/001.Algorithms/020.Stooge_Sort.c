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

void StoogeSort(int arr[], int i, int j)
{
    int temp = 0;
    int temp1 = 0;

    if (arr[i] > arr[j]) {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    if ((i + 1) >= j)
        return;

    temp1 = (int)((j - i + 1) / 3);

    StoogeSort(arr, i, j - temp1);
    StoogeSort(arr, i + temp1, j);
    StoogeSort(arr, i, j - temp1);
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

    StoogeSort(arr, 0, size-1);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}