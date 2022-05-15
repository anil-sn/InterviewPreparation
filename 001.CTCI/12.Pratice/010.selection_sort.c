
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
display_array(int array[10]) {
    int i;
    for (i=0; i <10; i++) {
        printf("%d ", array[i]);
    }
    printf("\r\n");
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}
void selection_sort(int array[10]) {
    int i,j, min_idx;

    printf("Original Array :");
    display_array(array);

    for (i =0; i < (10-1); i++) {

        // Find the minimum element in unsorted array
        min_idx = i;

        for (j = i+1; j < 10; j++) {
            if (array[j] < array[min_idx]) {
                min_idx = j;
            }
        }

        // Swap the found minimum element with the first element
        swap(&array[min_idx], &array[i]);
    }

    printf("Sorted Array :");
    display_array(array);
}

int main (){
    int array[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    selection_sort(array);
}