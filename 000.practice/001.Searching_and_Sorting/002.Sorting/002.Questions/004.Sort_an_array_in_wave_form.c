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

/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%3d ", arr[i]);
    printf("\n");
}

/*
    Given an unsorted array of integers, sort the array into a wave like array.
    An array ‘arr[0..n-1]’ is sorted in wave form if
    arr[0] >= arr[1] <= arr[2] >= arr[3] <= arr[4] >= …..

        Examples:

            Input:  arr[] = {10, 5, 6, 3, 2, 20, 100, 80}
            Output: arr[] = {10, 5, 6, 2, 20, 3, 100, 80} OR
                            {20, 5, 10, 2, 80, 6, 100, 3} OR
                            any other array that is in wave form

            Input:  arr[] = {20, 10, 8, 6, 4, 2}
            Output: arr[] = {20, 8, 10, 4, 6, 2} OR
                            {10, 8, 20, 2, 6, 4} OR
                            any other array that is in wave form

            Input:  arr[] = {2, 4, 6, 8, 10, 20}
            Output: arr[] = {4, 2, 8, 6, 20, 10} OR
                            any other array that is in wave form

            Input:  arr[] = {3, 6, 5, 10, 7, 20}
            Output: arr[] = {6, 3, 10, 5, 20, 7} OR
                            any other array that is in wave form

    A Simple Solution is to use sorting. First sort the input array, then swap all adjacent elements.
    For example, let the input array be {3, 6, 5, 10, 7, 20}. After sorting,
    we get {3, 5, 6, 7, 10, 20}. After swapping adjacent elements, we get {5, 3, 7, 6, 20, 10}.
*/
// Comparison function to sort Item according to val/weight ratio
int cmp(const void *x, const void *y)
{
    int *a = (int *)x;
    int *b = (int *)y;

    return *a > *b;
}

// This function sorts arr[0..n-1] in wave form, i.e.,
// arr[0] >= arr[1] <= arr[2] >= arr[3] <= arr[4] >= arr[5]..

void sortInWave(int arr[], int n)
{
    // Sort the input array
    qsort(arr, n, sizeof(arr[0]), cmp);
    printf("\nAfter Quick Sorting \n");
    printArray(arr, n);

    // Swap adjacent elements
    for (int i=0; i<n-1; i += 2)
        swap(&arr[i], &arr[i+1]);
}


int main() {
    int arr[] ={12, 16, 22, 30, 35, 39, 42, 45, 48, 50, 53, 55, 56};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    int n = 4;
    int k = 35;

    printf("\nBefore Sorting \n");
    printArray(arr, arr_size);

    sortInWave(arr, arr_size);

    printf("\nAfter Wave Sorting \n");
    printArray(arr, arr_size);
    return 0;
}