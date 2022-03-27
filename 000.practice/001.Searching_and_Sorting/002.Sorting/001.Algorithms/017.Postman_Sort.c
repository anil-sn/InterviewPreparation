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
    A highly engineered variant of top-down radix sort where attributes of the key are described
    so the Sorting algorithm can allocate buckets and distribute efficiently.

    Postman sort works by sorting the integers of an array from their most significant digits
    to their least significant digits. In the Postman sort the integer having most significant digits
    and the number of elements in that integer is determined, the length of the longest integer is stored.
    All the elements in the array are divided by a particular base.
    The elements of the array are sorted on the basis of the most significant digit to the
    least significant digit i.e. from leftmost to rightmost digit.

    Algorithm
    The algorithm consists of these steps: It compares the integers in the array
    on the basis of the significant digits.

    Firstly, the maximum number of significant digits in the array of integers is determined,
    a particular base is set according to the significant digits.

    Every element in the array is divided by that particular base and in this way
    the leftmost digits of all the elements in the array i.e. the most significant digit
    is compared throughout the array elements and the numbers are swapped if they are not in correct order.

    Similarly next most significant digit is compared as the base is reset,
    the elements of the array which are having the same value of that particular
    digit are arranged by comparing their values.

    The randomly arranged numbers are sorted using Postman Sort.

    Firstly the significant digits in all the elements in the array is determined,
    the base is set according to the maximum significant digits.

    In this case the maximum significant digits is 3 so the base is set as 100.

    All the array elements are divided by the base, in this way the most significant digit
    is detemined and arranged if they are not found in correct order.
    The base is divided by 10, the base is reset then the elements are arranged on the basis of
    the next most significant digit. The array elements are divided by the base ,
    if the elements are not found in correct order they are arranged.
    During the last iteration, the base becomes 1, the array elements are
    divided by the base and are arranged if not found in correct order.
*/

void arrangeItems(int arr[], int arr1[], int k, int n)
{
    int temp = 0;
    int i = 0;
    int j = 0;

    for (i = k; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (arr1[i] > arr1[j]) {
                temp = arr1[i];
                arr1[i] = arr1[j];
                arr1[j] = temp;

                temp = (arr[i] % 10);
                arr[i] = (arr[j] % 10);
                arr[j] = temp;
            }
        }
    }
}

int main()
{
    int i = 0;
    int j = 0;
    int c = 0;
    int t = 0;
    int k = 0;
    int n = 1;

    int t1 = 0;
    int t2 = 0;
    int n1 = 0;

    int max = 0;
    int len = 0;
    int maxd = 0;
    int temp = 0;

    int arr[9] = {12, 11, 13, 5, 6, 7, 3, 2, 1};
    int arr1[9];

    len = sizeof(arr) / sizeof(arr[0]);

    printf("Unsorted Array:\n");
    for (i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");


    for (i = 0; i < len; i++)
        arr1[i] = arr[i];

    for (i = 0; i < len; i++) {
        t = arr[i];
        while (t > 0) {
            c++;
            t = t / 10;
        }
        if (maxd < c)
            maxd = c;
        c = 0;
    }
    while (--maxd)
        n = n * 10;

    for (i = 0; i < len; i++) {
        max = arr[i] / n;
        t = i;

        for (j = i + 1; j < len; j++) {
            if (max > (arr[j] / n)) {
                max = arr[j] / n;
                t = j;
            }
        }

        temp = arr1[t];
        arr1[t] = arr1[i];
        arr1[i] = temp;

        temp = arr[t];
        arr[t] = arr[i];
        arr[i] = temp;
    }
    while (n >= 1) {
        for (i = 0; i < len;) {
            t1 = arr[i] / n;
            for (j = i + 1; t1 == (arr[j] / n); j++)
                ;
            arrangeItems(arr, arr1, i, j);
            i = j;
        }
        n = n / 10;
    }

    printf("Sorted Array:\n");
    for (i = 0; i < len; i++)
        printf("%d ", arr1[i]);
    printf("\n");

    return 0;
}