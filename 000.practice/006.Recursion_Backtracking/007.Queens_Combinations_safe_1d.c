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
    1. You are given a number n, representing the size of a n * n chess board.
    2. You are required to calculate and print the permutations in which n queens (distinct) can be
        placed on the n * n chess-board.
    3. No queen shall be able to kill another.

    Note -> Use the code snippet and follow the algorithm discussed in question video. The judge can't
                force you but the intention is to teach a concept. Play in spirit of the question.
*/
bool
isQueenSafe(int chess[4][4], int boardSize, int row, int col) {
    for (int i = row, j = col; i >= 0; i--) {
        if (chess[i][j] > 0) {
            return false;
        }
    }

    for (int i = row, j = col; i < boardSize; i++) {
        if (chess[i][j] > 0) {
            return false;
        }
    }

    for (int i = row, j = col; j >= 0; j--) {
        if (chess[i][j] > 0) {
            return false;
        }
    }

    for (int i = row, j = col; j < boardSize; j++) {
        if (chess[i][j] > 0) {
            return false;
        }
    }

    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (chess[i][j] > 0) {
            return false;
        }
    }

    for (int i = row, j = col; i < boardSize && j < boardSize; i++, j++) {
        if (chess[i][j] > 0) {
            return false;
        }
    }


    for (int i = row, j = col; i < boardSize && j >= 0; i++, j--) {
        if (chess[i][j] > 0) {
            return false;
        }
    }

    for (int i = row, j = col; i >= 0 && j < boardSize; i--, j++) {
        if (chess[i][j] > 0) {
            return false;
        }
    }

    return true;
}

void queensCombinations (int queens, int totalQueens, int chess[4][4], int lastCell)
{
    if (queens == totalQueens) {
        printf("\r\n--------------------------------\r\n");
        for (int i = 0; i < totalQueens; i++) {
            for (int j = 0; j < totalQueens; j++) {
                if (chess[i][j] == 0) {
                    printf("-\t");
                } else {
                    printf("q%d\t", chess[i][j]);
                }
            }
            printf("\r\n");
        }
        printf("--------------------------------\r\n\r\n");
        return;
    }

    for (int i = lastCell + 1 ; i < totalQueens * totalQueens; i++) {
        int row  = i / totalQueens;
        int col  = i % totalQueens;

        if (chess[row][col] == 0 && isQueenSafe(chess, totalQueens, row, col)) {
            chess[row][col] = true;
            queensCombinations(queens + 1, totalQueens, chess, row * totalQueens + col);
            chess[row][col] = false;
        }
    }
}

int main ()
{
    int n = 4;
    int chess[4][4]  = {{0}};

    queensCombinations(0, n, chess, -1);

    return 0;
}