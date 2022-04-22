#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Merge two Sorted Arrays Without Extra Space

    Problem statement: Given two sorted arrays arr1[] and arr2[] of sizes n and m in
        non-decreasing order. Merge them in sorted order. Modify arr1 so that it contains the
        first N elements and modify arr2 so that it contains the last M elements.

    Examples:

        Example 1:
            Input:
                n = 4, arr1[] = [1 4 8 10]
                m = 5, arr2[] = [2 3 9]

            Output:
                arr1[] = [1 2 3 4]
                arr2[] = [8 9 10]

            Explanation:
                After merging the two non-decreasing arrays, we get, 1,2,3,4,8,9,10.

        Example2:
            Input:
                n = 4, arr1[] = [1 3 5 7]
                m = 5, arr2[] = [0 2 6 8 9]

            Output:
                arr1[] = [0 1 2 3]
                arr2[] = [5 6 7 8 9]

            Explanation:
                After merging the two non-decreasing arrays, we get, 0 1 2 3 5 6 7 8 9.
*/

void swap(int *i, int *j) {
    int temp = 0;
    temp = *i;
    *i = *j;
    *j = temp;
}

void merge(int arr1[], int arr2[], int n, int m) {
    // code here
    int i, k;
    for (i = 0; i < n; i++) {

        // take first element from arr1
        // compare it with first element of second array
        // if condition match, then swap
        if (arr1[i] > arr2[0]) {
            int temp = arr1[i];
            arr1[i] = arr2[0];
            arr2[0] = temp;
        }

        // then sort the second array
        // put the element in its correct position
        // so that next cycle can swap elements correctly
        int first = arr2[0];

        // insertion sort is used here
        for (k = 1; k < m && arr2[k] < first; k++) {
            arr2[k - 1] = arr2[k];
        }

        arr2[k - 1] = first;
    }
}

void mergeGap(int ar1[], int ar2[], int n, int m) {

    // code here
    int gap = ceil((double)(n + m) / 2);
    while (gap > 0) {
        int i = 0;
        int j = gap;

        while (j < (n + m)) {
            if (j < n && ar1[i] > ar1[j]) {

                swap(&(ar1[i]), &(ar1[j]));
            } else if (j >= n && i < n && ar1[i] > ar2[j - n]) {

                swap(&(ar1[i]), &(ar2[j - n]));
            } else if (j >= n && i >= n && ar2[i - n] > ar2[j - n]) {

                swap(&(ar2[i - n]), &(ar2[j - n]));
            }

            j++;
            i++;
        }

        if (gap == 1) {
            gap = 0;
        } else {
            gap = ceil((double) gap / 2);
        }
    }
}

int main() {

    int arr1[] = {1,4,7,8,10};
    int arr2[] = {2,3,9};
    int count1 = sizeof(arr1)/sizeof(arr1[0]);
    int count2 = sizeof(arr2)/sizeof(arr2[0]);

    printf("Before merge: \r\n");
    for (int i = 0; i < count1; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\r\n");

    for (int i = 0; i < count2; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\r\n");

    mergeGap(arr1, arr2, count1, count2);

    printf("After merge: \r\n");
    for (int i = 0; i < count1; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\r\n");

    for (int i = 0; i < count2; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\r\n");
}