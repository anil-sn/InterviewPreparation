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
    2. You are required to calculate and print the combinations in which n knights can be placed on the
        n * n chess-board.
    3. No knight shall be able to kill another.

    Note -> Use the code snippet and follow the algorithm discussed in question video. The judge can't
                force you but the intention is to teach a concept. Play in spirit of the question.
    Input Format
    A number n
*/
char array[1024];
#define N 2

bool IsKnightSafe(bool chess[N][N], int i, int j) {
    if(i - 1 >= 0 && j - 2 >= 0 && chess[i - 1][j - 2]){
        return false;
    }

    if(i - 2 >= 0 && j - 1 >= 0 && chess[i - 2][j - 1]){
        return false;
    }

    if(i - 2 >= 0 && j + 1 < N && chess[i - 2][j + 1]){
        return false;
    }

    if(i - 1 >= 0 && j + 2 < N && chess[i - 1][j + 2]){
        return false;
    }

    return true;
}

void nknights(int currentNight, int nights, bool chess[N][N], int lastLocation) {
    if (currentNight == nights) {
        printf("\r\n-----------\r\n");
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                chess[row][col] ? printf("k\t") : printf("-\t");
            }
            printf("\r\n");
        }
        printf("-------------\r\n");
        return;
    }

    for (int cell = lastLocation + 1; cell < N * N; cell++) {
        int row = cell / N;
        int col = cell % N;

        if (chess[row][col] == false &&
            IsKnightSafe(chess, row, col)) {

            chess[row][col] = true;

            nknights(currentNight + 1, nights, chess, row * N + col);

            chess[row][col] = false;
        }
    }
}



int main ()
{
    int n = N;
    bool board[N][N]  = {{false}};

    memset(array, '\0', sizeof(array));
    nknights(0, n, board, -1);

    return 0;
}
