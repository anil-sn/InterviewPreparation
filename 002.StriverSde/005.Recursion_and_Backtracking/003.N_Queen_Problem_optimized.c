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
void solve(int col, char board[][4], int leftrow[], int upperDiagonal[], int lowerDiagonal[], int n)
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
        if ((leftrow[row] == 0) &&
            (lowerDiagonal[row + col] == 0) &&
            (upperDiagonal[n - 1 + col - row] == 0)) {

            board[row][col] = 'Q';
            leftrow[row] = 1;
            lowerDiagonal[row + col] = 1;
            upperDiagonal[n - 1 + col - row] = 1;

            solve(col + 1, board, leftrow, upperDiagonal, lowerDiagonal, n);

            board[row][col] = '-';
            leftrow[row] = 0;
            lowerDiagonal[row + col] = 0;
            upperDiagonal[n - 1 + col - row] = 0;
        }
    }
}

void solveNQueens(int n)
{
    char board[n][n];
    int leftrow[n];
    int upperDiagonal[2 * n - 1];
    int lowerDiagonal[2 * n - 1];

    memset(board, '-', sizeof(board));
    memset(leftrow, 0, sizeof(leftrow));
    memset(upperDiagonal, 0, sizeof(upperDiagonal));
    memset(lowerDiagonal, 0, sizeof(lowerDiagonal));

    solve(0, board, leftrow, upperDiagonal, lowerDiagonal, n);
}

int main() {
    int n = 4; // we are taking 4*4 grid and 4 queens
    solveNQueens(n);
    return 0;
}