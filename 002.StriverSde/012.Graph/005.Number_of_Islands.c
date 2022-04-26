#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void dfs(char grid[4][5], int i, int j, int row, int col)
{
    if ((i < 0)         ||
        (j < 0)         ||
        (i >= row)      ||
        (j >= col)      ||
        (grid[i][j] != '1')) {

        return;
    }

    grid[i][j] = '#';
    dfs(grid, i+1, j, row, col);
    dfs(grid, i-1, j, row, col);
    dfs(grid, i, j+1, row, col);
    dfs(grid, i, j-1, row, col);

    return;
}

void printGrid(char grid[4][5], int row, int col)
{
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%2c", grid[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");
}

int numberOfIslands(char grid[4][5], int row, int col)
{
    int count = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (grid[i][j] == '1') {
                dfs(grid, i, j, row, col);
                count += 1;
                printf("VISIT [%d]: \r\n", count);
                printGrid(grid, 4, 5);
            }
        }
    }

    return count;
}

int main (void)
{
    char grid1[4][5] = {{'1','1','1','1','0'},
                        {'1','1','0','1','0'},
                        {'1','1','0','0','0'},
                        {'0','0','0','0','0'}};

    char grid2[4][5] = {{'1','1','0','0','0'},
                        {'1','1','0','0','0'},
                        {'0','0','1','0','0'},
                        {'0','0','0','1','1'}};

    int count = 0;

    printf("Input Grid 1: \r\n");
    printGrid(grid1, 4, 5);
    count = numberOfIslands(grid1, 4, 5);
    printf("Number of islands : %d \r\n", count);

    printf("Input Grid 1: \r\n");
    printGrid(grid2, 4, 5);
    count = numberOfIslands(grid2, 4, 5);
    printf("Number of islands : %d \r\n", count);

    return 0;
}