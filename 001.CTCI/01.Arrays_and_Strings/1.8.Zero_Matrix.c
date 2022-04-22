#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
Zero Matrix: Write an algorithm such that if an element in an MxN matrix is 0, its entire row and
column are set to 0.

SOLUTION
At first glance, this problem seems easy: just iterate through the matrix and every time we see a cell with
value zero, set its row and column to 0. There's one problem with that solution though: when we come
across other cells in that row or column, we'll see the zeros and change their row and column to zero. Pretty
soon, our entire matrix will be set to zeros.

To make this somewhat more space efficient we could use a bit vector instead of a boolean array. It would
still be O(N) space.
We can reduce the space to 0(1) by using the first row as a replacement for the row array and the first
column as a replacement for the column array. This works as follows:
1. Check if the first row and first column have any zeros, and set variables rowHasZero and
columnHasZero. (We'll nullify the first row and first column later, if necessary.)
2. Iterate through the rest of the matrix, setting matrix[ i] [ 0) and matrix [ 0) [ j] to zero whenever
there's a zero in matrix[i] [j ].
3. Iterate through rest of matrix, nullifying row i if there's a zero in matrix [ i] [ 0].
4. Iterate through rest of matrix, nullifying column j if there's a zero in matrix [ 0] [ j].
5. Nullify the first row and first column, if necessary (based on values from Step 1 ).

This code has a lot of "do this for the rows, then the equivalent action for the column:' In an interview, you
could abbreviate this code by adding comments and TODOs that explain that the next chunk of code looks
the same as the earlier code, but using rows. This would allow you to focus on the most important parts of
the algorithm.

*/

void displayMatrix(u_int32_t p[4][4], u_int32_t n)
{
    u_int32_t row, col;

    for (row = 0; row < n; row++)
    {
        for (col = 0; col < n; col++) {
            printf("%d\t", p[row][col]);
        }

        printf("\n");
    }

    printf("\n\n");
}

void nullifyRow(u_int32_t matrix[4][4], int length, int row) {
    for (int j = 0; j < length; j++) {
        matrix[row][j] = 0;
    }
}

void nullifyColumn(u_int32_t matrix[4][4], int length, int col) {
    for (int i= 0; i < length; i++) {
        matrix[i][col] = 0;
    }
}

void setzeros(u_int32_t matrix[4][4], int length) {

    bool rowHasZero = false;
    bool colHasZero = false;

    // Check if first row has a zero
    for (int j = 0; j < length; j++) {
        if (matrix[0][j] == 0) {
            rowHasZero = true;
            break;
        }
    }

    // Check if first column has a zero
    for (int i = 0; i < length; i++) {
        if (matrix[i][0] == 0) {
            colHasZero = true;
            break;
        }
    }

    // Check for zeros in the rest of the array
    for (int i= 1; i < length; i++) {
        for (int j = 1; j < length;j++) {
            if (matrix[i][j] == 0) {
                matrix[i][0] = 0;
                matrix[0][j] = 0;
            }
        }
    }

    // Nullify rows based on values in first column
    for (int i= 1; i < length; i++) {
        if (matrix[i][0] == 0) {
            nullifyRow(matrix, length, i);
        }
    }

    // Nullify columns based on values in first row
    for (int j = 1; j < length; j++) {
        if (matrix[0][j] == 0) {
            nullifyColumn(matrix, length, j);
        }
    }

    // Nullify first row
    if (rowHasZero) {
        nullifyRow(matrix, length, 0);
    }

    // Nullify first column
    if (colHasZero) {
        nullifyColumn(matrix, length, 0);
    }
}

int main()
{
    u_int32_t image[4][4] = {{1,0,3,4},
                            {5,6,7,8},
                            {9,10,0,12},
                            {13,14,15,16}};
    u_int32_t *pDst;
    u_int32_t n;

    // setting initial values and memory allocation
    n = 4;

    // process each buffer
    printf("Input Matrix : \r\n");
    displayMatrix(image, n);

    setzeros(image, n);

    printf("Zero Matrix : \r\n");
    displayMatrix(image, n);

    return 0;
}

