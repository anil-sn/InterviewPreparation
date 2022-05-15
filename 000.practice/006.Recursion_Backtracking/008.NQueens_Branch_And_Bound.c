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
    1. You are given a number n, the size of a chess board.
    2. You are required to place n number of queens in the n * n cells of board such that no queen can
        kill another.
    Note - Queens kill at distance in all 8 directions
    3. Complete the body of printNQueens function - without changing signature - to calculate and
        print all safe configurations of n-queens

    Use sample input and output to get more idea.

    Note -> The online judge can't force you to write the function recursively but that is what the spirit
                of question is.

    Write recursive and not iterative logic. The purpose of the question is to aid learning recursion, branch and bound technique and not test you.
    Input Format
    A number n
*/

char array[1024];
#define N 4

void queensCombinations (bool board[4][4], int totalQueens,
                         int row, bool cols[], bool ndiag[], bool rdiag[])
{
    if (row == totalQueens) {
        printf("%s\r\n", array);
        memset(array, '\0', sizeof(array));

        printf("\r\n--------------------------------\r\n");
        for (int i = 0; i < totalQueens; i++) {
            for (int j = 0; j < totalQueens; j++) {
                if (board[i][j] == false) {
                    printf("-\t");
                } else {
                    printf("q%d\t", board[i][j]);
                }
            }
            printf("\r\n");
        }
        printf("--------------------------------\r\n\r\n");
        return;
    }

    for (int col = 0 ; col < totalQueens; col++) {
        if (cols[col] == false &&
            ndiag[row + col] == false &&
            rdiag[row - col + (N - 1)] == false) {

            board[row][col] = true;
            cols[col] = true;
            ndiag[row + col] = true;
            rdiag[row - col + (N - 1)] = true;

            int pos = strlen(array);
            sprintf(array+pos, "%d-%d, ", row, col);
            queensCombinations (board, totalQueens, row + 1, cols, ndiag, rdiag);
            memset(array, '\0', sizeof(array));

            board[row][col] = false;
            cols[col] = false;
            ndiag[row + col] = false;
            rdiag[row - col + (N - 1)] = false;
        }
    }
}

int main ()
{
    int n = N;
    bool board[N][N]  = {{false}};
    bool cols[N] = {false};
    bool ndiag[2*N - 1] = {false};
    bool rdiag[2*N - 1]= {false};

    memset(array, '\0', sizeof(array));
    queensCombinations(board, n, 0, cols, ndiag, rdiag);

    return 0;
}
