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
backtrack (char **board, int row_size, int col_size, char **curr_result, int row, int col, char *word)
{
    int new_row;
    int new_col;
    bool result;

    if (*word == 0) {
        return true;
    }

    if (row >= row_size || col  >= col_size || row < 0 || col < 0) {
        return false;
    }

    if (curr_result[row][col] == 1) {
        return false;
    } else if (board[row][col] != *word) {
        return false;
    } else {
        curr_result[row][col] = 1;
    }

    // 1, 0
    // 0, 1
    // -1, 0
    // 0, -1
    result = backtrack (board,
                        row_size,
                        col_size,
                        curr_result,
                        row + 1,
                        col,
                        word + 1);

    if (result) {
        curr_result[row][col] = 0;
        return result;
    }

    result = backtrack (board,
                        row_size,
                        col_size,
                        curr_result,
                        row,
                        col + 1,
                        word + 1);

    if (result) {
        curr_result[row][col] = 0;
        return result;
    }

    result = backtrack (board,
                        row_size,
                        col_size,
                        curr_result,
                        row - 1,
                        col,
                        word + 1);

    if (result) {
        curr_result[row][col] = 0;
        return result;
    }

    result = backtrack (board,
                        row_size,
                        col_size,
                        curr_result,
                        row,
                        col - 1,
                        word + 1);

    if (result) {
        curr_result[row][col] = 0;
        return result;
    }

    curr_result[row][col] = 0;
    return false;
}

bool
exist (char **board, int boardSize, int *boardColSize, char *word)
{
    char **curr_result;
    int result;
    curr_result = calloc (boardSize, sizeof (char *));

    for (int i = 0; i < boardSize; i++) {
        curr_result[i] = calloc (boardColSize[0], sizeof (char));
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardColSize[0]; j++) {
            if (board[i][j] == word[0]) {
                result = backtrack (board, boardSize, boardColSize[0], curr_result, i, j, word);

                if (result) {
                    return result;
                }
            }
        }
    }

    return result;
}