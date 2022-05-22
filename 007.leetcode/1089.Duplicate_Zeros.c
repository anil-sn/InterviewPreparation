#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void
duplicateZeros (int *arr, int arrSize)
{
    for (int i = 0; i < arrSize - 1; i++) {
        if (arr[i] == 0) {
            move_backward (arr, i, arrSize);
            arr[i + 1] = 0;
            i++;
        }
    }
}

// Helper function to move elements backward
void
move_backward (int *arr, int i, int arrSize)
{
    for (int j = arrSize - 1; j > i; j--) {
        arr[j] = arr[j - 1];
    }
}

/*
    First pass count 0s in the array, this is our shift variable.
    Second reverse pass:

    If arr[i] == 0 reduce shift count by 1.
    If arr[i + shift] within array boundary, then assign arr[i + shift] = arr[i] (shift element to it's new position). If arr[i] == 0 then also assign next element after arr[i + shift] to 0, meaning arr[i + shift + 1] = 0, but don't forget to check that i + shift + 1 is withing array boundary.
*/

int
count (int *arr, int arrSize, int item)
{
    int c = 0;

    for (int i = 0; i < arrSize; i++) {
        if (arr[i] == item) {
            c++;
        }
    }

    return c;
}

void
duplicateZeros (int *arr, int arrSize)
{
    int shift = count (arr, arrSize, 0);

    for (int i = arrSize - 1; i >= 0; i--) {
        if (!arr[i]) {
            shift--;
        }

        if (i + shift < arrSize) {
            arr[i + shift] = arr[i];

            if (!arr[i] && i + shift + 1 < arrSize) {
                arr[i + shift + 1] = 0;
            }
        }
    }
}