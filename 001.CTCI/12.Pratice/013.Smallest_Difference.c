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
    Dont practice until you get it right
    practice until you can't get it worng
*/

void
display(int array[], int N) {
    int i;
    for (i=0; i < N; i++) {
        printf("%d ", array[i]);
    }
    printf("\r\n");
}

void sort(int array[], int N) {
    int i,j, temp;

    printf("Original Array :");
    display(array, N);

    for (i =0; i <N; i++) {
        for (j=i; j<N; j++) {
            if (array[i] > array[j]) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    printf("Sorted Array :");
    display(array, N);
}

// function to calculate Small
// result between two arrays
int findSmallestDifference(int A[], int B[], int m, int n) {
    // Sort both arrays using
    // sort function
    sort(A, m);
    sort(A, n);

    int a = 0, b = 0;

    // Initialize result as max value
    int result = INT_MAX;

    // Scan Both Arrays upto
    // sizeof of the Arrays
    while (a < m && b < n) {
        if (abs(A[a] - B[b]) < result) {
            result = abs(A[a] - B[b]);
        }

        // Move Smaller Value
        if (A[a] < B[b])
            a++;
        else
            b++;
    }

    // return final smallest result
    return result;
}

int main () {
    // Input given array A
    int A[] = {1, 2, 11, 5};

    // Input given array B
    int B[] = {4, 12, 19, 23, 127, 235};

    printf("Smallest Different b/w A & B is %d\r\n",
           findSmallestDifference(A, B, sizeof(A)/sizeof(A[0]), sizeof(B)/sizeof(A[0])));

    return;
}