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
Pigeonhole Sort Algorithm:

    Pigeonhole sorting is a sorting algorithm that is suitable for sorting lists of
    elements where the number of elements and the number of possible key values are
    approximately the same.
    It requires O(n + Range) time where n is number of elements in input array and
    ‘Range’ is number of possible values in array.

    Working of Algorithm :
    Find minimum and maximum values in array. Let the minimum and maximum values
    be ‘min’ and ‘max’ respectively. Also find range as ‘max-min+1’.

    Set up an array of initially empty “pigeonholes” the same size as of the range.
    Visit each element of the array and then put each element in its pigeonhole.
    An element arr[i] is put in hole at index arr[i] – min.
    Start the loop all over the pigeonhole array in order and put the elements
    from non- empty holes back into the original array.
*/

/* Sorts the array using pigeonhole algorithm */
void pigeonHoleSort(int *arr, int len) {
    int i = 0;
    int j = 0;
    int min = 0;
    int max = 0;
    int range = 0;

    int *temp;

    min = arr[0];
    max = arr[0];

    i = 1;

    while (i < len) {
        if (arr[i] < min) {
            min = arr[i];
        }

        if (arr[i] > max) {
            max = arr[i];
        }

        i++;
    }

    range = max - min + 1;
    temp = (int *)malloc(sizeof(int) * range);

    i = 0;

    while (i < range) {
        temp[i] = 0;
        i++;
    }

    i = 0;

    while (i < len) {
        temp[arr[i] - min]++;
        i++;
    }

    j = 0;
    i = 0;

    while (i < range) {
        while (temp[i] > 0) {
            arr[j] = i + min;
            temp[i]--;
            j++;
        }

        i++;
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

    pigeonHoleSort(arr, size);

    printf("\nSorted array is \n");
    printArray(arr, size);
    return 0;
}