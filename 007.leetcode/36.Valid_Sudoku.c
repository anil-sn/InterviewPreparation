#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

bool
isValidSudoku (char **board, int boardSize, int *boardColSize)
{
    int *arr = (int *)calloc (10, sizeof (int));
    int i, j;

    //for each row
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (board[i][j] != 46) {
                arr[ (board[i][j]) % 48]++;
            }
        }

        for (int k = 0; k < 10; k++) {
            if (arr[k] > 1) {
                return false;
            }
        }

        for (int k = 0; k < 10; k++) {
            arr[k] = 0;
        }
    }

    // for each column
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (board[j][i] != 46) {
                arr[ (board[j][i]) % 48]++;
            }
        }

        for (int k = 0; k < 10; k++) {
            if (arr[k] > 1) {
                return false;
            }
        }

        for (int k = 0; k < 10; k++) {
            arr[k] = 0;
        }
    }

    // for each box
    int t = 0;
    int p = 0;

    while (t < 9) {
        if (p == 9) {
            t = t + 3;
            p = 0;
        }

        if (t == 9) {
            break;
        }

        for (i = t; i < 3 + t; i++) {
            for (j = p; j < p + 3; j++) {
                if (board[i][j] != 46) {
                    arr[ (board[i][j]) % 48]++;
                }
            }
        }

        for (int k = 0; k < 10; k++) {
            if (arr[k] > 1) {
                return false;
            }
        }

        for (int k = 0; k < 10; k++) {
            arr[k] = 0;
        }

        p = p + 3;
    }

    return true;
}


bool
isValidSudoku (char **board, int boardSize, int *boardColSize)
{
    int rowNumPosition[9][9];
    int colNumPosition[9][9];

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            rowNumPosition[i][j] = -1;
            colNumPosition[i][j] = -1;
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') {
                continue;
            }

            int num = board[i][j] - '0' - 1;

            if (rowNumPosition[i][num] != -1 ||
                    colNumPosition[j][num] != -1) {
                return false;
            }

            rowNumPosition[i][num] = j;
            colNumPosition[j][num] = i;
            int rowStart = i / 3 * 3;
            int colStart = j / 3 * 3;

            for (int m = rowStart; m < i; m++) {
                if (rowNumPosition[m][num] >= colStart &&
                        rowNumPosition[m][num] < colStart + 3) {
                    return false;
                }
            }

            for (int n = colStart; n < j; n++) {
                if (colNumPosition[n][num] >= rowStart &&
                        rowNumPosition[n][num] < rowStart + 3) {
                    return false;
                }
            }
        }
    }

    return true;
}