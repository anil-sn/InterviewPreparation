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
Bucket sort is mainly useful when input is uniformly distributed over a range.
For example, consider the following problem.
Sort a large set of floating point numbers which are in range from 0.0 to 1.0 and
are uniformly distributed across the range. How do we sort the numbers efficiently?
A simple way is to apply a comparison based sorting algorithm.
The lower bound for Comparison based sorting algorithm
(Merge Sort, Heap Sort, Quick-Sort .. etc) is Ω(n Log n), i.e.,
they cannot do better than nLogn.
Can we sort the array in linear time? Counting sort can not be
 applied here as we use keys as index in counting sort.
 Here keys are floating point numbers.

The idea is to use bucket sort. Following is bucket algorithm.

bucketSort(arr[], n)
1) Create n empty buckets (Or lists).
2) Do following for every array element arr[i].
    a) Insert arr[i] into bucket[n*array[i]]
3) Sort individual buckets using insertion sort.
4) Concatenate all sorted buckets.
*/

int getMax(int a[], int n) { // function to get maximum element from the given array
    int max = a[0];

    for (int i = 1; i < n; i++)
        if (a[i] > max) {
            max = a[i];
        }

    return max;
}

// function to implement bucket sort
void bucket(int a[], int n) {
    int max = getMax(a, n); //max is the maximum element of array
    int bucket[max], i;

    for (int i = 0; i <= max; i++) {
        bucket[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        bucket[a[i]]++;
    }

    for (int i = 0, j = 0; i <= max; i++) {
        while (bucket[i] > 0) {
            a[j++] = i;
            bucket[i]--;
        }
    }
}

// function to print array elements
void printArr(int a[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d  ", a[i]);
    }
}

int main() {
    int a[] = {54, 12, 84, 57, 69, 41, 9, 5};
    // n is the size of array
    int n = sizeof(a) / sizeof(a[0]);
    printf("Before sorting array elements are - \n");
    printArr(a, n);
    bucket(a, n);
    printf("\nAfter sorting array elements are - \n");
    printArr(a, n);
}