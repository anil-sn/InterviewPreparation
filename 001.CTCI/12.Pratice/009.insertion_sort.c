
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

void insertion_sort(int array[10]) {
    int i,j, temp, key;

    printf("Original Array :");
    display_array(array);

    for (i =0; i <10; i++) {
        key = array[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }

    printf("Sorted Array :");
    display_array(array);
}

int main (){
    int array[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    insertion_sort(array);
}