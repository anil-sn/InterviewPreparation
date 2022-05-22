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

/**************************************/
void
rotate (int **matrix, int matrixSize, int *matrixColSize)
{
    int c = 0;
    int n = matrixSize - 1;

    for ( c = 0 ; c <= n ; c++, n = --) {
        for ( int i = c, j = n ; i < n; i++, j--) {
            int tmp = matrix[c][j];
            matrix[c][j] = matrix[i][c];
            matrix[i][c] = matrix[n][i];
            matrix[n][i] = matrix[j][n];
            matrix[j][n] = tmp;
        }
    }
}

/**************************************/
// https://leetcode.com/problems/rotate-image/discuss/2049385/C%2B%2B-100-Simple-solution-using-just-2-flips-step-by-step-visualization.
void
rotate (int **matrix, int matrixSize, int *matrixColSize)
{
    int height = matrixSize;
    int width = (* matrixColSize);
    int temp;

    // flip diagonally (upper-left -> lower-right)
    for (int i = 0; i < height; ++i) {
        for (int j = i + 1; j < width; ++j) {
            // swap element
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }

    // flip horizontally
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width / 2; ++j) {
            // swap element
            temp = matrix[i][j];
            matrix[i][j] = matrix[i][width - 1 - j];
            matrix[i][width - 1 - j] = temp;
        }
    }
}