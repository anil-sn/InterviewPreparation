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
    Given a matrix of integers A of size N x M in which each row is sorted.
    Find an return the overall median of the matrix A.
    Note: No extra memory is allowed.
    Note: Rows are numbered from top to bottom and columns are numbered from left to right.

    Input Format
        The first and only argument given is the integer matrix A.
    Output Format
        Return the overall median of the matrix A.
    Constraints
        1 <= N, M <= 10^5
        1 <= N*M  <= 10^6
        1 <= A[i] <= 10^9
    N*M is odd
    For Example
        Input 1:
            A = [   [1, 3, 5],
                    [2, 6, 9],
                    [3, 6, 9]   ]
        Output 1:
            5
        Explanation 1:
            A = [1, 2, 3, 3, 5, 6, 6, 9, 9] Median is 5. So, we return 5.
*/

// It returns the index of the first element in the range [first, last) which compares grater than val.
int upper_bound(int arr[], int first, int last, int val)
{
    // Initialise the "low" and "high" appropriately.
    int low = first;
    int high = last;

    while (low < high) {

        int mid = (low + high) / 2;
        if (arr[mid] > val) {
            /* If the current element is less than val, reduce the search space to left half. */
            high = mid;
        } else {
            /* If the current element is greater than or equal to val, reduce the search space to right half. */
            low = mid + 1;
        }
    }

    return low;
}

/* The median is the middle number in a sorted, ascending or descending, list of numbers */
int findMedian(int matrix[3][3], int row, int col)
{
    // Since, 1 <= matrix[i][j] <= 10^5, set the "low" and "high" value accordingly.
    int low = 1, high = 1000;

    // Loop until our search spaces convergers to single element.
    while (low < high)
    {

        int mid = (low + high) / 2;

        // To count number of elements less than or equal to "mid".
        int count = 0;

        // Iterate through each row and increment the "count".
        for (int i = 0; i < row; i++)
        {
            /*
               NOTE:
               upper_bound (ForwardIterator first, ForwardIterator last, const T& val);
               Returns an iterator
               pointing to the first element in the range [first,last)
               which compares greater than val.
            */
            count = count + upper_bound(matrix[i], 0, col, mid);
        }

        if (count >= (row * col + 1) / 2) {
            // The median will lie in the range of [“mid”, “high”].
            high = mid;
        } else
        {
            // The median will lie in the range of ["mid" + 1, "high"].
            low = mid + 1;
        }
    }
    return low;
}

int main ()
{
    int array[3][3] = { {1, 3,  5},
                        {2, 6,  9},
                        {3, 6,  9}
                    };
    int row = 3;
    int col = 3;

    int median = findMedian(array, row, col);
    printf("The median value for the given array is : %d \r\n", median);

    return;
}