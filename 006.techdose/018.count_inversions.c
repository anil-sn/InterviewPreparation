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
    if a[i] > a[j] where i < j is inversion

    9 6 8 4

    9 6 8 4

    9 -> 6
    9 -> 8 
    9 -> 4
    6 -> 4
    8 -> 4

    total : 5

    Logic Merge Sort:

    SPLIT:
    1       20      6       7       5       8       11      3

    1       20      6      7|       5       8       11      3

    1       20|     6      7|       5      8|      11|      3

    1|      20|     6|     7|      5|      8|      11|      3

    MERGE:
    1       20|     6      7|      5    8|         3         11  - Count 1

    1       6      7        20|     3       5      8       11    - Count 1 + 2 + 2 = 5

    1       3       5       6      7       8        11      20    - count 5 + 2 + 2 = 9

    Algorithm: 
    The idea is similar to merge sort, divide the array into two equal or almost equal halves in each step until the base case is reached.
    Create a function merge that counts the number of inversions when two halves of the array are merged, 
        create two indices i and j, i is the index for the first half, and j is an index of the second half. 
        if a[i] is greater than a[j], then there are (mid – i) inversions. 
        because left and right subarrays are sorted, 
        so all the remaining elements in left-subarray (a[i+1], a[i+2] … a[mid]) will be greater than a[j].
    Create a recursive function to divide the array into halves and find the answer by summing the number of inversions is the first half, 
    the number of inversion in the second half and the number of inversions by merging the two.
    The base case of recursion is when there is only one element in the given half.
    Print the answer
*/

 
 
/* This function merges two sorted arrays and returns inversion count in the arrays.*/
int merge(int arr[], int temp[], int left, int mid, int right)
{
    int i, j, k;
    int inv_count = 0;
    
    i = left; /* i is index for left subarray*/
    j = mid; /* j is index for right subarray*/
    k = left; /* k is index for resultant merged subarray*/

    printf("i : %d, mid : %d, right : %d \r\n", i, mid, right);

    printf("Left Array : ");
    for (int x = left; x < mid; x++) {
        printf("%d ", arr[x]);
    }
    printf("\r\n");

    printf("Right Array : ");
    for (int x = mid; x <= right; x++) {
        printf("%d ", arr[x]);
    }
    printf("\r\n");

    while ((i <= mid - 1) && (j <= right)) {

        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }

        else {
            printf("%d , %d [Count : %d] \r\n", arr[i], arr[j], (mid - i));
            temp[k++] = arr[j++];
 
            /* This is the trick */
            inv_count = inv_count + (mid - i);
        }
    }
 
    /* Copy the remaining elements of left subarray (if there are any) to temp*/
    while (i <= mid - 1)
        temp[k++] = arr[i++];
 
    /* Copy the remaining elements of right subarray (if there are any) to temp*/
    while (j <= right)
        temp[k++] = arr[j++];
 
    /* Copy back the merged elements to original array */
    for (i = left; i <= right; i++)
        arr[i] = temp[i];
 
    return inv_count;
}

/* An auxiliary recursive function that sorts the input array and returns the number of inversions in the array. */
int _mergeSort(int arr[], int temp[], int left, int right)
{
    int mid, inv_count = 0;

    if (right > left) {

        /* Divide the array into two parts and call _mergeSortAndCountInv() for each of the parts */
        mid = (right + left) / 2;
 
        /* Inversion count will be the sum of inversions in left-part, right-part and number of inversions in merging */
        inv_count += _mergeSort(arr, temp, left, mid);
        inv_count += _mergeSort(arr, temp, mid + 1, right);
 
        /*Merge the two parts*/
        inv_count += merge(arr, temp, left, mid + 1, right);
    }
    return inv_count;
}

/* This function sorts the input array and returns the number of inversions in the array */
int mergeSort(int arr[], int array_size)
{
    int* temp = (int*)calloc(array_size, sizeof(int));
    return _mergeSort(arr, temp, 0, array_size - 1);
}

/* Driver code*/
int main(void)
{
    int arr[] = { 1, 20, 6, 4, 5 };
    int len = sizeof(arr)/sizeof(arr[0]);

    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");

    printf(" Number of inversions are %d \n", mergeSort(arr, len));

    return 0;
}