
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

void bubble_sort(int array[10]) {
    int i,j, temp;

    printf("Original Array :");
    display_array(array);

    for (i =0; i <10; i++) {
        for (j=i; j<10; j++) {
            if (array[i] > array[j]) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    printf("Sorted Array :");
    display_array(array);
}

int main (){
    int array[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    bubble_sort(array);
}