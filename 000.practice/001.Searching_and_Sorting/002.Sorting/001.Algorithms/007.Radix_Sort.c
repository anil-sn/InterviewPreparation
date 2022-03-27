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
    The lower bound for Comparison based sorting algorithm (Merge Sort, Heap Sort, Quick-Sort .. etc)
    is Ω(nLogn), i.e., they cannot do better than nLogn.

    Radix Sort is the answer. The idea of Radix Sort is to do digit by digit sort starting from
    least significant digit to most significant digit. Radix sort uses counting sort as a subroutine to sort.

    The Radix Sort Algorithm

    Do following for each digit i where i varies from least significant digit to the most significant digit.
    Sort input array using counting sort (or any stable sort) according to the i’th digit.

    Counting sort is a linear time sorting algorithm that sort in O(n+k) time when elements
    are in the range from 1 to k.

    What if the elements are in the range from 1 to n2?
    We can’t use counting sort because counting sort will take O(n2) which is
    worse than comparison-based sorting algorithms. Can we sort such an array in linear time?

    Example:

        Original, unsorted list:
        170, 45, 75, 90, 802, 24, 2, 66

        Sorting by least significant digit (1s place) gives:
        [*Notice that we keep 802 before 2,
        because 802 occurred  before 2 in the original list, and
        similarly for pairs  170 & 90 and 45 & 75.]

        170, 90, 802, 2, 24, 45, 75, 66

        Sorting by next digit (10s place) gives:
        [*Notice that 802 again comes before 2 as 802 comes before 2 in the previous list.]

        802, 2, 24, 45, 66, 170, 75, 90

        Sorting by the most significant digit (100s place) gives:
        2, 24, 45, 66, 75, 90, 170, 802
*/

// A utility function to get maximum value in arr[]
int getMax(int arr[], int n)
{
    int mx = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > mx) {
            mx = arr[i];
        }
    }
    return mx;
}

// A function to do counting sort of arr[] according to
// the digit represented by exp.
void countSort(int arr[], int n, int exp)
{
    int output[n]; // output array
    int i, count[10] = { 0 };

    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

// The main function to that sorts arr[] of size n using
// Radix Sort
void radixsort(int arr[], int n)
{
    // Find the maximum number to know number of digits
    int m = getMax(arr, n);

    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

// A utility function to print a given array of given size
void printArray(int* arr, int size)
{
    int i;
    for (i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");
}

/* Driver program to test above functions */
int main()
{
    int arr[] = {12, 11, 13, 5, 6, 7, 3, 2, 1};
    int size = sizeof(arr)/sizeof(arr[0]);

    printf("\nUnSorted array is \n");
    printArray(arr, size);

    radixsort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}