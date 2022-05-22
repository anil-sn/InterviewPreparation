#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int
countSubIslands (int row, int col, int B[row][col], int A[row][col])
{
    int res = 0;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            if (A[i][j] == 1) {
                res += dfs (row, col, B, A, i, j);
            }

    return res;
}


int
dfs (int row, int col, int B[row][col], int A[row][col], int i, int j)
{
    int res = 1;

    if (i < 0 || i == row || j < 0 || j == col || A[i][j] == 0) {
        return 1;
    }

    A[i][j] = 0;
    res &= dfs (row, col, B, A, i - 1, j);
    res &= dfs (row, col, B, A, i + 1, j);
    res &= dfs (row, col, B, A, i, j - 1);
    res &= dfs (row, col, B, A, i, j + 1);
    return res & B[i][j];
}