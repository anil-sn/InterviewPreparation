#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

bool
searchMatrix (int matrix[3][4], int row, int col, int target)
{
    // binary search
    int left = 0, right = row * col - 1;
    int pivotIdx, pivotElement;

    while (left <= right) {
        pivotIdx = left + (right - left) / 2;
        pivotElement = matrix[pivotIdx / row][pivotIdx % row];

        if (target == pivotElement) {
            return true;
        } else {
            if (target < pivotElement) {
                right = pivotIdx - 1;
            } else {
                left = pivotIdx + 1;
            }
        }
    }

    return false;
}


int
main (void)
{
    int matrix[3][4] = {{1,   3,  5,  7},
        {0,  11, 16, 20},
        {23, 30, 34, 60}
    };
    int target;
    bool result = false;
    target = 13;
    result = searchMatrix (matrix, 3, 4, target);
    printf ("Searching %d result %s \r\n", target, result ? "True" : "False");
    target = 16;
    result = searchMatrix (matrix, 3, 4, target);
    printf ("Searching %d result %s \r\n", target, result ? "True" : "False");
    return 0;
}