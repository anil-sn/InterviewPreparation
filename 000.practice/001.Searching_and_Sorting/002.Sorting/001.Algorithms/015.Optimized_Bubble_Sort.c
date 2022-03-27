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
    Bubble Sort is a simple, stable, and in-place sorting algorithm.
    Due to its simplicity, it is widely used as a sorting algorithm by computer programmers.

    The basic working principle of a simple bubble sort is that it repeatedly swaps
    the adjacent elements if they are in the wrong order.
    Hence, after every full iteration, the largest element reaches its position as
    in the sorted array.

    However, this simple bubble sort has time complexity O(n^2) in all cases because
    the inner loop runs even if the array is sorted. Therefore, we use an optimized
    version of bubble sort. This version uses a bool variable to check whether any
    swap took place in the previous iteration. If yes, only then the next iteration takes place. If no, then the loop breaks.

    Pseudo-code:

    	for i: 0 to n-1 not inclusive do:
    	     swapped = false
    	     for j: 0 to n-i-1 not inclusive do:
    	          If a[j] > a[j+1] then
    	                   swap a[j] and a[j+1]
    	                   swapped = true
    	          end if
    	     end for
    	     if swapped == false then
    	            break
    	     end if
    	end for
*/

void bubble_sort(int arr[], int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
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

    bubble_sort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}