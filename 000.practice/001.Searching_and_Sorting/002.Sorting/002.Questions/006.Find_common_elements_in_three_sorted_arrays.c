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
    Given three arrays sorted in non-decreasing order, print all common elements in these arrays.

    Examples:

        Input:
            ar1[] = {1, 5, 10, 20, 40, 80}
            ar2[] = {6, 7, 20, 80, 100}
            ar3[] = {3, 4, 15, 20, 30, 70, 80, 120}
        Output: 20, 80

        Input:
            ar1[] = {1, 5, 5}
            ar2[] = {3, 4, 5, 5, 10}
            ar3[] = {5, 5, 10, 20}
        Output: 5, 5
*/

/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%3d ", arr[i]);
    printf("\n");
}

// This function prints common elements in ar1
void findCommon(int ar1[], int ar2[], int ar3[], int n1, int n2, int n3)
{
    // Initialize starting indexes for ar1[], ar2[] and ar3[]
    int i = 0, j = 0, k = 0;

    // Iterate through three arrays while all arrays have elements
    while (i < n1 && j < n2 && k < n3)
    {
        // If x = y and y = z, print any of them and move ahead
        // in all arrays
        if (ar1[i] == ar2[j] && ar2[j] == ar3[k]){
            printf("%d ", ar1[i]);
            i++; j++; k++;
        } else if (ar1[i] < ar2[j]) { // x < y
             i++;
        } else if (ar2[j] < ar3[k]) { // y < z
             j++;
        } else { // We reach here when x > y and z < y, i.e., z is smallest
             k++;
        }
    }
}

// Driver code
int main()
{
    int ar1[] = {1, 5, 10, 20, 40, 80};
    int ar2[] = {6, 7, 20, 80, 100};
    int ar3[] = {3, 4, 15, 20, 30, 70, 80, 120};
    int n1 = sizeof(ar1)/sizeof(ar1[0]);
    int n2 = sizeof(ar2)/sizeof(ar2[0]);
    int n3 = sizeof(ar3)/sizeof(ar3[0]);

    printArray(ar1, n1);
    printArray(ar2, n2);
    printArray(ar3, n3);
    printf("Common Elements are ");
    findCommon(ar1, ar2, ar3, n1, n2, n3);
    printf("\r\n");
    return 0;
}