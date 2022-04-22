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
void rotateFunc(int matrix[][3], int rotated[][3], int rowSize, int colSize) {

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            rotated[j][rowSize - i - 1] = matrix[i][j];
        }
    }

    return;
}

void rotateFunc2(int matrix[][3], int rowSize, int colSize) {

    printf("\r\n\r\n");
    for (int i = 0; i < rowSize; i++) {
        for (int j = i; j < colSize; j++) {
            int temp = 0;
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
            printf("[%d, %d] -> [%d, %d] \t", i,j,j,i);
        }
        printf("\r\n");
    }
    printf("\r\n\r\n");

#if 1
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < rowSize / 2; j++) {
            int temp = 0;
            temp = matrix[i][j];
            matrix[i][j] = matrix[i][rowSize - 1 - j];
            matrix[i][rowSize - 1 - j] = temp;
            printf("[%d, %d] -> [%d, %d] \t", i,j,i,(rowSize - 1 - j));
        }
        printf("\r\n");
    }
    printf("\r\n");
#endif
}

int main() {

    int arr[][3] =  {{1, 2, 3},
                    {4, 5, 6},
                    {7, 8, 9}};

    int rotate[][3] =  {{0, 0, 0},
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

    rotateFunc(arr, rotate, rowSize, colSize);

    printf("Rotated Image \r\n");
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            //printf("[%d, %d] -> [%d, %d] \t", i,j,j,(rowSize-1-i));
            printf("%3d", rotate[i][j]);
        }
        printf("\r\n");
    }

    rotateFunc2(arr, rowSize, colSize);
    printf("In Place Trasformation \r\n");
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            //printf("[%d, %d] -> [%d, %d] \t", i,j,j,(rowSize-1-i));
            printf("%3d", arr[i][j]);
        }
        printf("\r\n");
    }
}