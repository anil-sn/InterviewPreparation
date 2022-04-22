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

// C program to move all values equal to K to the end of the Array

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void
display(int array[], int N) {
    int i;
    for (i=0; i < N; i++) {
        printf("%d ", array[i]);
    }
    printf("\r\n");
}

// Function to move the element to the end
void
moveElementToEnd( int array[], int size, int toMove)
{

    display(array, size);

    // Mark left pointer
    int i = 0;

    // Mark the right pointer
    int j = size - 1;

    // Iterate until left pointer
    // crosses the right pointer
    while (i < j) {

        while (i < j && array[j] == toMove) {
            // decrement right pointer
            j--;
        }

        if (array[i] == toMove) {

            // swap the two elements
            // in the array
            swap(&array[i], &array[j]);
        }

        // increment left pointer
        i++;
    }

    display(array, size);

    // return the result
    return;
}

// Driver code
int main(int argc, char* argv[])
{
    int array[] = { 100, 99, 3, 5, 99, 20, 40, 99, 23, 56 };
    int len = sizeof(array)/sizeof(array[0]);
    int K = 99;

    moveElementToEnd(array, len, K);

    return 0;
}