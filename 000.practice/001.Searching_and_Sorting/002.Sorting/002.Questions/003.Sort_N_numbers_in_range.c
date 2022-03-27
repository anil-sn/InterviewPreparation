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

// A function to do counting sort of arr[] according to
// the digit represented by exp.
void countSort(int arr[], int n, int exp)
{
    int output[n]; // output array
    int i, count[10];
    int mod = 0;

    memset(output, 0, sizeof(output));
    memset(count, 0, sizeof(count));

    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    printf("Updated count array :\r\n");
    printArray(count, 10);

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    printf("Updated position count array :\r\n");
    printArray(count, 10);

    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        printf("arr[i] = %d, (arr[i] / exp) % 10 = %d, count[(arr[i] / exp) % 10] = %d\r\n", arr[i], (arr[i] / exp) % 10, count[(arr[i] / exp) % 10]);
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    printf("Output array :\r\n");
    printArray(output, n);

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

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

void sort(int arr[], int n)
{
    // Find the maximum number to know number of digits
    int m = getMax(arr, n);

    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

int main() {
    int arr[] ={12, 16, 22, 30, 35, 39, 42, 45, 48, 50, 53, 55, 56};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    int n = 4;
    int k = 35;

    printf("\nBefore Sorting \n");
    printArray(arr, arr_size);

    sort(arr, arr_size);

    printf("\nAfter Sorting \n");
    printArray(arr, arr_size);
    return 0;
}