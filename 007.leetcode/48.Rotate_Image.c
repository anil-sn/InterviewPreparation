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
reverse (int *matrix, int l, int h)
{
    int n = h - l;
    int temp;

    // Swap character starting from two corners
    for (int i = 0; i < n / 2; i++) {
        temp = matrix[i + l];
        matrix[i + l] = matrix[n - i - 1 + l];
        matrix[n - i - 1 + l] = temp;
    }
}

void
rotate (int **matrix, int matrixSize, int *matrixColSize)
{
    int n = matrixSize;
    int temp;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] =  temp;
        }
    }

    for (int i = 0; i < matrixSize; i++) {
        reverse (matrix[i], 0, (*matrixColSize));
    }
}