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

void flipItems(int arr[], int length, int num) {
    int temp;
    int i = 0;

    while (i < --num) {
        temp = arr[i];
        arr[i] = arr[num];
        arr[num] = temp;
        i++;
    }
}

int panCakeSort(int arr[], unsigned int len) {
    int i = 0;
    int j = 0;
    int max = 0;
    int moves = 0;

    if (len < 2) {
        return 0;
    }

    i = len;

    while (i > 1) {
        max = 0;
        j = 0;

        while (j < i) {
            if (arr[j] > arr[max]) {
                max = j;
            }

            j++;
        }

        if (max == i - 1) {
            i--;
            continue;
        }

        if (max) {
            moves++;
            flipItems(arr, len, max + 1);
        }

        flipItems(arr, len, i);
        i--;
    }

    return moves;
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

    panCakeSort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}