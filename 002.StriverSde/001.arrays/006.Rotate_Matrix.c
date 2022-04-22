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
    Rotate Image by 90 degree
    Problem Statement: Given a matrix, your task is to rotate the matrix by 90 degrees.

    Examples:

        Example 1:
        Input:  [[1,2,3],
                 [4,5,6],
                 [7,8,9]]
        Output: [[7,4,1],
                 [8,5,2],
                 [9,6,3]]
        Explanation: Rotate the matrix simply by 90 degree clockwise and return the matrix.

        Example 2:
        Input:  [[ 5,  1,  9, 11],
                 [ 2,  4,  8, 10],
                 [13,  3,  6,  7],
                 [15, 14, 12, 16]]
        Output: [[15, 13,  2,  5],
                 [14,  3,  4,  1],
                 [12,  6,  8,  9],
                 [16,  7, 10, 11]]
        Explanation: Rotate the matrix simply by 90 degree clockwise and return the matrix
*/

#define ACCESS(_arr, _i, _j, _len) (*(((_arr)+((_i)*(_len))) + (_j)))


#if 1
void rotateFunc(int *matrix, int *rotated, int rowSize, int colSize) {

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            ACCESS(rotated, j, (rowSize - i - 1), rowSize) = ACCESS(matrix, i, j, rowSize);
            //rotated[j][rowSize - i - 1] = matrix[i][j];
        }
    }

    return;
}
#endif

int main() {

    int arr[3][3] =  {{1, 2, 3},
                    {4, 5, 6},
                    {7, 8, 9}};

    int rotate[3][3] =  {{0, 0, 0},
                        {0, 0, 0},
                        {0, 0, 0}};
    int rowSize = (sizeof(arr)/sizeof(arr[0]));
    int colSize = (sizeof(arr[0])/sizeof(arr[0][0]));

    printf("Original Image \r\n");
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            //printf("[%d, %d] -> [%d, %d] \t", i,j,j,(rowSize-1-i));
            printf("%3d", arr[i][j]);
        }
        printf("\r\n");
    }

    rotateFunc((int *)arr, (int *)rotate, rowSize, colSize);

    printf("Rotated Image \r\n");
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            //printf("[%d, %d] -> [%d, %d] \t", i,j,j,(rowSize-1-i));
            printf("%3d", rotate[i][j]);
        }
        printf("\r\n");
    }
}