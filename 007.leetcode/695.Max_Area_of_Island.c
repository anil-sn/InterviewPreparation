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
dfs (int row, int col, int grid[row][col], int i, int j, int area)
{
    if (i < 0 || i >= row || j < 0 || j >= col || grid[i][j] == 0) {
        return area;
    }

    grid[i][j] = 0;
    area++;
    area = dfs (row, col, grid, i + 1, j, area);
    area = dfs (row, col, grid, i, j + 1, area);
    area = dfs (row, col, grid, i - 1, j, area);
    area = dfs (row, col, grid, i, j - 1, area);
    return area;
}

int
maxAreaOfIsland (int row, int col, int grid[row][col])
{
    if (grid == NULL || 0 == row) {
        return 0;
    }

    int max = 0;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (grid[i][j] == 1) {
                int area = dfs (row, col, grid, i, j, 0);
                max = MAX (area, max);
            }
        }
    }

    return max;
}
