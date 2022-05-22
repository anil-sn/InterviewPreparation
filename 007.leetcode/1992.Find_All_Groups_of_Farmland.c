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
    This problem is like a modification island problem!!!
    what makes this problem special is that we have to return coordinates of starting && ending block

    dfs function is simple to understand : it traverses a given island && mark all visited boxes as -1
        NOTE : in this prob we can't mark visited to 0 , because in ok function we are trying to find corner blocks &&
        it will conflict with it if visited are marked to 0

    The main thing in this solution is ok function && its working is explained below which is also very simple!!!
    ---------------------------------------------------------------
    working of ok function for current(x,y)
        up , left , right , down = stores bool vals , helps to find whether current(x,y) block is a corner block || not
        up    = (either up block don't exist OR if exists then it must be 0)
        left  = (either left block don't exist OR if exist then it must be 0)
        right = (either right block don't exist OR if exist then it must be 0)
        down  = (either down block don't exist OR if exist then it must be 0)

        if up && left both are true means : current(x,y) block is first block of rectangle
            push it to res
        if right && down both are true means : current(x,y) block is last block of rectangle
            pusht it to res
    ---------------------------------------------------------------
*/

int res[100][4];
int pos = 0;

void
ok (int x, int y, int row, int col, int land[row][col])
{
    bool up   = (x - 1 < 0 || land[x - 1][y] == 0);
    bool left = (y - 1 < 0 || land[x][y - 1] == 0);
    bool right = (y + 1 >= col || land[x][y + 1] == 0);
    bool down  = (x + 1 >= row || land[x + 1][y] == 0);

    if (up && left) {
        res[pos][0] = x;
    }

    res[pos][1] = y;

    if (right && down) {
        res[pos][2] = x;
        res[pos][3] = y;
        pos++;
    }
}

void
dfs (int x, int y, int row, int col, int land[row][col])
{
    if (x < 0 || y < 0 || x == row || y == col || land[x][y] == -1 || land[x][y] == 0) {
        return;
    }

    ok (x, y, row, col, land);
    // -1 : means visited
    land[x][y] = -1; // marking visited
    dfs (x - 1, y, row, col, land);  // UP
    dfs (x + 1, y, row, col, land);  // DOWN
    dfs (x, y - 1, row, col, land);  // LEFT
    dfs (x, y + 1, row, col, land);  // RIGHT
}

int **
findFarmland (int row, int col, int land[row][col] )
{
    for (int i = 0 ; i < row ; ++i)
        for (int j = 0 ; j < col ; ++j)
            if (land[i][j] == 1) {
                dfs (i, j, row, col, land);
            }

    return res;
}