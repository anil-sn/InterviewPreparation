#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int *
sortArrayByParity (int *A, int ASize, int *returnSize)
{
    int *array = (int *) malloc (sizeof (int) * ASize);
    int front = 0, end = ASize - 1, i = 0;

    while ((end - front) >= 0) {
        if (A[i] & 0x1) {
            array[end] = A[i];
            end--;
        } else {
            array[front] = A[i];
            front++;
        }

        i++;
    }

    *returnSize = ASize;
    return array;
}

int *
sortArrayByParity (int *A, int ASize, int *returnSize)
{
    *returnSize = ASize;

    for (int i = 0, j = 0; j < ASize; j++) {
        if (A[j] % 2 == 0) {
            int tmp = A[i];
            A[i++] = A[j];
            A[j] = tmp;;
        }
    }

    return A;
}