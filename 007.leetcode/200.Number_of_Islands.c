#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define ROWS 4

int directions[ROWS][2] = {
    {-1, 0}, //up
    {0, 1}, //right
    {1, 0}, //down
    {0, -1} //left
};

struct Node {
    int row;
    int col;
    struct Node *next;
} *front = NULL, *rear = NULL;

struct dimension {
    int row;
    int col;
} ;

void
enqueue (int row, int col)
{
    struct Node *t;
    t = (struct Node *)malloc (sizeof (struct Node));

    if (t == NULL) {
        printf ("Queue is FUll\n");
    } else {
        t->row = row;
        t->col = col;
        t->next = NULL;

        if (front == NULL) {
            front = rear = t;
        } else {
            rear->next = t;
            rear = t;
        }
    }
}

struct dimension dim;

struct dimension *dequeue()
{
    struct Node *t;

    if ((front == NULL)) {
        printf ("Queue is Empty\n");
    } else {
        dim.row = front->row;
        dim.col = front->col;
        t = front;
        front = front->next;
        free (t);
    }

    return &dim;
}

int
isEmpty()
{
    return (front == NULL);
}


int
numIslands (char **grid, int gridSize, int *gridColSize)
{
    if ((gridSize == 0) || (!gridColSize)) {
        return 0;
    }

    int islandCount = 0;
    int i = 0, j = 0, k = 0;
    struct dimension *t = NULL;
    int row = 0, col = 0;
    int nxtrow = 0, nxtcol = 0;

    for (i = 0; i < gridSize; i++) {
        for (j = 0; j < (*gridColSize); j++) {
            if (grid[i][j] == '1') {
                grid[i][j] = 0;
                islandCount++;
                enqueue (i, j);

                while (!isEmpty()) {
                    t = dequeue();

                    if (t) {
                        row = t->row;
                        col = t->col;
                    }

                    for (k = 0; k < ROWS; k++) {
                        nxtrow = row + directions[k][0];
                        nxtcol = col + directions[k][1];

                        if (nxtrow < 0 || nxtcol < 0 || nxtrow > (gridSize - 1) || nxtcol > (*gridColSize - 1)) {
                            continue;
                        }

                        if (grid[nxtrow][nxtcol] == '1') {
                            enqueue (nxtrow, nxtcol);
                            grid[nxtrow][nxtcol] = '0';
                        }
                    }
                }
            }
        }
    }

    return islandCount;
}

/*************************************************/
int
numIslands (int row, int col, char A[row][col])
{
    int  res = 0;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++) {
            res += dfs (row, col, A, i, j);
        }

    return res;
}

int
dfs (int row, int col, char A[row][col], int i, int j)
{
    if (i < 0 || i == row || j < 0 || j == col || A[i][j] == '0') {
        return 0;
    }

    A[i][j] = '0';
    dfs (row, col, A, i - 1, j);
    dfs (row, col, A, i + 1, j);
    dfs (row, col, A, i, j - 1);
    dfs (row, col, A, i, j + 1);
    return 1;
}