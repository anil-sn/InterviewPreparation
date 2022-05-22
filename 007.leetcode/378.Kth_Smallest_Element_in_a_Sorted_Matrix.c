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
Binary Search
    The key point for any binary search is to figure out the "Search Space".
    I think there are two kind of "Search Space" -- index and range (the range from the smallest number to the biggest number).
    Most usually, when the array is sorted in one direction, we can use index as "search space",
    when the array is unsorted and we are going to find a specific number, we can use "range".

Let me give you two examples of these two "search space"

    index -- A bunch of examples -- https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/ ( the array is sorted)
    range -- https://leetcode.com/problems/find-the-duplicate-number/ (Unsorted Array)
    The reason why we did not use index as "search space" for this problem is the matrix is sorted in two directions,
    we can not find a linear way to map the number and its index.
*/

int
findMin (int row, int col, int matrix[row][col], int k)
{
    int lo = matrix[0][0];
    int hi = matrix[row - 1][col - 1] + 1;//[lo, hi)

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        int count = 0;
        int j = col - 1;

        for (int i = 0; i < row; i++) {
            while (j >= 0 && matrix[i][j] > mid) {
                j--;
            }

            count += (j + 1);
        }

        if (count < k) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return lo;
}

int
main (void)
{
    int data[3][3] = {{1,   5,  9},
        {10, 11, 13},
        {12, 13, 15}
    };
    int row = 3;
    int col = 3;
    int k = 8;
    int min = findMin (row, col, data, k);
    printf ("The %dth Minimum: %d \r\n", k, min);
}