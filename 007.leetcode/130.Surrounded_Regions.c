#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


int stck[655350], top;
void
dfs1 (int x, int y, char **board, const int row, const int col, const char tag)
{
    top = 0;
    stck[top++] = x;
    stck[top++] = y;

    while (top) {
        y = stck[--top];
        x = stck[--top];

        if (x < 0 || y < 0 || x >= row || y >= col || board[x][y] != 'O') {
            continue;
        }

        board[x][y] = tag;
        stck[top++] = x;
        stck[top++] = y - 1;
        stck[top++] = x;
        stck[top++] = y + 1;
        stck[top++] = x - 1;
        stck[top++] = y;
        stck[top++] = x + 1;
        stck[top++] = y;
    }
}
void
solve (char **board, int boardSize, int *boardColSize)
{
    for (int i = 0; i < boardSize; i++) {
        dfs1 (i, 0, board, boardSize, (* boardColSize), '#'), dfs1 (i, (* boardColSize) - 1, board, boardSize, (* boardColSize), '#');
    }

    for (int j = 0; j < (* boardColSize); j++) {
        dfs1 (0, j, board, boardSize, (* boardColSize), '#'), dfs1 (boardSize - 1, j, board, boardSize, (* boardColSize), '#');
    }

    for (int i = 1; i < boardSize - 1; i++)
        for (int j = 1; j < (* boardColSize) - 1; j++) {
            dfs1 (i, j, board, boardSize, (* boardColSize), 'X');
        }

    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < (* boardColSize); j++)
            if (board[i][j] == '#') {
                board[i][j] = 'O';
            }
}

void
dfs (char **board, int m, int n, int r, int c, char from, char to)
{
    if (r < 0 || c < 0 || r >= m || c >= n) {
        return;
    }

    if (board[r][c] == from) {
        board[r][c] = to;
    } else {
        return;
    }

    dfs (board, m, n, r - 1, c, from, to);
    dfs (board, m, n, r, c + 1, from, to);
    dfs (board, m, n, r + 1, c, from, to);
    dfs (board, m, n, r, c - 1, from, to);
}

/*
 * Replace all cells that are dfs reachable from the edges to 'Z' from 'O'
 * Then all the cells that are  left with 'O' are covered by X's, so replace
 * them with X's
 */

void
solve (char **board, int boardSize, int *boardColSize)
{
    int i, j, m, n;

    if (!board || !boardColSize || !boardSize) {
        return;
    }

    m = boardSize;
    n = *boardColSize;

    for (i = 0; i < n; i++) {
        dfs (board, m, n, 0, i, 'O', 'Z');
    }

    for (i = 0; i < m; i++) {
        dfs (board, m, n, i, 0, 'O', 'Z');
    }

    for (i = 0; i < m; i++) {
        dfs (board, m, n, i, n - 1, 'O', 'Z');
    }

    for (i = 0; i < n; i++) {
        dfs (board, m, n, m - 1, i, 'O', 'Z');
    }

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++) {
            if (board[i][j] == 'Z') {
                board[i][j] = 'O';
            } else if (board[i][j] == 'O') {
                board[i][j] = 'X';
            }
        }
}