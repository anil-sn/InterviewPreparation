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

int isSorted(int arr[], int len)
{
    len = len - 1;
    while (len > 0) {
        if (arr[len] < arr[len - 1])
            return 0;
        len--;
    }
    return 1;
}

void shuffle(int arr[], int len)
{
    int i = 0;
    int t = 0;

    int temp = 0;

    while (i < len) {
        t = arr[i];

        temp = rand() % len;
        arr[i] = arr[temp];
        arr[temp] = t;

        i++;
    }
}

void bogoSort(int arr[], int len)
{
    while (!isSorted(arr, len))
        shuffle(arr, len);
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

    bogoSort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}