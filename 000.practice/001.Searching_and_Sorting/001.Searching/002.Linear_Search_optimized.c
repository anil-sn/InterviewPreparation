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

//  Given an array arr[] of n elements, write a function to search a given element x in arr[].
int search (int arr[], int n, int x) {

    for (int i = 0, j = n-1; i < n && j >= 0; i++, j--) {
        if (arr[i] == x) {
            return i;
        } else if (arr[j] == x) {
            return j;
        }
    }

    return -1;
}

int main(void)
{
    int arr[] = { 2, 3, 4, 10, 40 };
    int x = 10;
    int n = sizeof(arr) / sizeof(arr[0]);

    // Function call
    int result = search(arr, n, x);

    printf ("%s:%d\r\n", (result == -1) ? "Element is not present in array" : "Element is present at index ", result);
    return 0;
}