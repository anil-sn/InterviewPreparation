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
    2. You are required to calculate and print the permutations in which n queens can be placed on the
        n * n chess-board.

    Note -> Use the code snippet and follow the algorithm discussed in question video. The judge can't
                force you but the intention is to teach a concept. Play in spirit of the question.
    Input Format
    A number n
*/
void queensCombination (int queens, int totalQueens, int chess[4][4], int i, int j) {
    if (queens == totalQueens) {
        printf("\r\n------------\r\n");
        for (int i = 0; i < totalQueens; i++) {
            for (int j = 0; j < totalQueens; j++) {
                if (chess[i][j] == 0) {
                    printf("-\t");
                } else {
                    printf("q\t");
                }
            }
            printf("\r\n");
        }
        printf("------------\r\n\r\n");
        return;
    }

    for (int col = j + 1; col < totalQueens; col++) {
        chess[i][col] = true;
        queensCombination ((queens + 1), totalQueens, chess, i , col);
        chess[i][col] = false;
    }

    for (int row = i + 1; row < totalQueens; row++) {
        for (int col = 0; col < totalQueens; col++) {
            chess[row][col] = true;
            queensCombination ((queens + 1), totalQueens, chess, row , col);
            chess[row][col] = false;
        }
    }
}

int main ()
{
    int n = 2;
    int chess[4][4]  = {{0}};

    queensCombination(0, n, chess, 0, -1);

    return 0;
}