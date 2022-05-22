/*
54. Spiral Matrix

Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.



For example,
Given the following matrix:

[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]


You should return [1,2,3,6,9,8,7,4,5].
*/



/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *
spiralOrder (int **matrix, int matrixSize, int *matrixColSize, int *returnSize)
{
    int *result = (int *)malloc (matrixSize * (*matrixColSize) * sizeof (int));
    int left = 0;
    int right = *matrixColSize - 1;
    int up = 0;
    int down = matrixSize - 1;
    int idx = 0;

    while (left <= right && up <= down) {
        for (int i = left; i <= right; i++) //left to right
        { result[idx++] = matrix[up][i]; }

        for (int i = up + 1; i <= down; i++) //up to down
        { result[idx++] = matrix[i][right]; }

        for (int i = right - 1; i > left && up != down; i--) //right to left
        { result[idx++] = matrix[down][i]; }

        for (int i = down; i > up && left != right; i--) //down to up
        { result[idx++] = matrix[i][left]; }

        up++;
        down--;
        left++;
        right--;
    }

    *returnSize = idx;
    return result;
}

/*
Difficulty:Medium
Total Accepted:106.2K
Total Submissions:409K


Companies Microsoft Google Uber
Related Topics Array
Similar Questions


                    Spiral Matrix II
*/
