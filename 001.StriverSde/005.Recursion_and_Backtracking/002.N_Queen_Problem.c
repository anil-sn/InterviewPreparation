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
    N Queen Problem | Return all Distinct Solutions to the N-Queens Puzzle
    Problem Statement: The n-queens is the problem of placing n queens on n × n chessboard such that no two queens can attack each other. Given an integer n, return all distinct solutions to the n -queens puzzle. Each solution contains a distinct boards configuration of the queen’s placement, where ‘Q’ and ‘.’ indicate queen and empty space respectively.

    Examples:

    Input: n = 4

    Output: [[".Q..","...Q","Q...","..Q."],["..Q.","Q...","...Q",".Q.."]]
    Explanation: There exist two distinct solutions to the 4-queens puzzle
*/
bool isSafe1(int row, int col, char board[][4], int n)
{
    // check upper element
    int duprow = row;
    int dupcol = col;

    while (row >= 0 && col >= 0) {
        if (board[row][col] == 'Q') {
            return false;
        }
        row--;
        col--;
    }

    col = dupcol;
    row = duprow;
    while (col >= 0) {
        if (board[row][col] == 'Q') {
            return false;
        }
        col--;
    }

    row = duprow;
    col = dupcol;
    while (row < n && col >= 0) {
        if (board[row][col] == 'Q') {
            return false;
        }
        row++;
        col--;
    }

    return true;
}

void solve(int col, char board[][4], int n)
{
    if (col == n) {
        printf("Soultion : \r\n\r\n");
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                printf("%4c", board[i][j]);
            }
            printf("\r\n");
        }
        printf("\r\n");
        return;
    }

    for (int row = 0; row < n; row++) {
        if (isSafe1(row, col, board, n)) {
            board[row][col] = 'Q';
            solve(col + 1, board, n);
            board[row][col] = '-';
        }
    }
}

void solveNQueens(int n)
{
    char board[n][n];
    memset(board, '-', sizeof(board));
    solve(0, board, n);
}

int main() {
    int n = 4; // we are taking 4*4 grid and 4 queens
    solveNQueens(n);
    return 0;
}