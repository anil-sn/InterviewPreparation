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

void CockTailSort(int arr[], int size)
{
    int n = size;
    int i = 0;
    int c = 0;

    do {
        i = 0;
        while (i < n - 1) {
            if (arr[i] > arr[i + 1]) {
                arr[i] = arr[i] + arr[i + 1];
                arr[i + 1] = arr[i] - arr[i + 1];
                arr[i] = arr[i] - arr[i + 1];
            }
            i++;
        }

        n = n - 1;
        i = size - 1;
        c = 0;

        while (i >= c) {
            if (arr[i] < arr[i - 1]) {
                arr[i] = arr[i] + arr[i - 1];
                arr[i - 1] = arr[i] - arr[i - 1];
                arr[i] = arr[i] - arr[i - 1];
            }
            i--;
        }
        c = c + 1;
    } while (n != 0 && c != 0);
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

    CockTailSort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}