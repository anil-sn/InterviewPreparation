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
    Search in a sorted 2D matrix
    Problem Statement:
        Given an m*n 2D matrix and an integer,
        write a program to find if the given integer exists in the matrix.

    Given matrix has the following properties:
        Integers in each row are sorted from left to right.
        The first integer of each row is greater than the
        last integer of the previous row

    Example 1:

        Input:
            matrix = [[ 1,  3,  5,  7],
                    [ 10, 11, 16, 20],
                    [ 23, 30, 34, 60]],

        target = 3
        Output: true

    Explanation:
            As the given integer(target) exists in the given 2D matrix,
            the function has returned true.

    Example 2:

        Input:
            matrix = [[ 1,  3,  5,  7],
                      [10, 11, 16, 20],
                      [23, 30, 34, 60]],

        target = 13
        Output: false

    Explanation:
        As the given integer(target) does not exist in the given 2D matrix,
        the function has returned false.
*/
bool search2DMatrix(int matrix[][4], int rowCount, int colCount, int key)
{
    int totalElements = (rowCount * colCount -1);
    if (0 == totalElements) {
        return false;
    }

    int high = totalElements;
    int low = 0;
    int mid = 0;

    while (low <= high) {
        mid = (low + high) / 2;
        if(matrix[mid / rowCount][mid % rowCount] == key) {
            return true;
        }
        if(matrix[mid / rowCount][mid % rowCount] < key) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return false;
}

int main ()
{
    int matrix[][4] = {{ 1,  3,  5,  7},
                       {10, 11, 16, 20},
                       {23, 30, 34, 60}};

    int rowCount = 3;
    int colCount = 4;
    int key = 4;
    bool result = false;

    result = search2DMatrix(matrix, rowCount, colCount, key);
    printf("Is key %d found in the matrix : %s \r\n", key, result ? "True" : "False");
}