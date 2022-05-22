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
fill (int **ret, int rowMax, int colMax, int row, int col, int oldC, int newC)
{
    if (row < 0 || row >= rowMax) {
        return;
    }

    if (col < 0 || col >= colMax) {
        return;
    }

    if (ret[row][col] != oldC) {
        return;
    }

    ret[row][col] = newC;
    fill (ret, rowMax, colMax, row + 1, col, oldC, newC);
    fill (ret, rowMax, colMax, row - 1, col, oldC, newC);
    fill (ret, rowMax, colMax, row, col + 1, oldC, newC);
    fill (ret, rowMax, colMax, row, col - 1, oldC, newC);
    return;
}

int **
floodFill (int **image, int imageSize, int *imageColSize, int sr, int sc, int newColor, int *returnSize, int **returnColumnSizes)
{
    if (imageSize < 1 || *imageColSize < 1) {
        return image;
    }

    int **ret = malloc (sizeof (int *) * imageSize);
    *returnColumnSizes = malloc (sizeof (int) * imageSize);

    for (int i = 0; i < imageSize; i++) {
        ret[i] = malloc (sizeof (int) * (*imageColSize));

        for (int j = 0; j < *imageColSize; j++) {
            ret[i][j] = image[i][j];
        }

        (*returnColumnSizes)[i] = (*imageColSize);
    }

    *returnSize = imageSize;

    if (image[sr][sc] != newColor) {
        fill (ret, imageSize, *imageColSize, sr, sc, image[sr][sc], newColor);
    }

    return ret;
}