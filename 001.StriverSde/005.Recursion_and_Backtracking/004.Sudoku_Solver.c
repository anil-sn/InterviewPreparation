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
    Problem Statement:

    Given a 9×9 incomplete sudoku, solve it such that it becomes valid sudoku.
    Valid sudoku has the following properties.

            1. All the rows should be filled with numbers(1 – 9) exactly once.

            2. All the columns should be filled with numbers(1 – 9) exactly once.

            3. Each 3×3 submatrix should be filled with numbers(1 – 9) exactly once.

    Note: Character ‘.’ indicates empty cell.

Example:

    Input:                                          Output:
    ┌────┬────┬────┬────┬────┬────┬────┬────┬────┐  ┌────┬────┬────┬────┬────┬────┬────┬────┬────┐
    │ 9  │ 5  │  7 │  - │ 1  │ 3  │ -  │ 8  │ 4  │  │ 9  │ 5  │  7 │  6 │ 1  │ 3  │ 2  │ 8  │ 4  │
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┼────┼────┼────┤
    │ 4  │ 8  │  3 │  - │ 5  │ 7  │ 1  │ 9  │ 6  │  │ 4  │ 8  │  3 │  2 │ 5  │ 7  │ 1  │ 9  │ 6  │
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┼────┼────┼────┤
    │ -  │ 1  │  2 │  - │ 4  │ 9  │ 5  │ 3  │ 7  │  │ 6  │ 1  │  2 │  8 │ 4  │ 9  │ 5  │ 3  │ 7  │
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┼────┼────┼────┤
    │ 1  │ 7  │  - │  3 │ -  │ 4  │ 9  │ -  │ 2  │  │ 1  │ 7  │  8 │  3 │ 6  │ 4  │ 9  │ 5  │ 2  │
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┼────┼────┼────┤
    │ 5  │ -  │  4 │  9 │ 7  │ -  │ 3  │ 6  │ -  │  │ 5  │ 2  │  4 │  9 │ 7  │ 1  │ 3  │ 6  │ 8  │
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┼────┼────┼────┤
    │ 3  │ -  │  9 │  5 │ -  │ 8  │ 7  │ -  │ 1  │  │ 3  │ 6  │  9 │  5 │ 2  │ 8  │ 7  │ 4  │ 1  │
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┼────┼────┼────┤
    │ 8  │ 4  │  5 │  7 │ 9  │ -  │ 6  │ 1  │ 3  │  │ 8  │ 4  │  5 │  7 │ 9  │ 2  │ 6  │ 1  │ 3  │
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┼────┼────┼────┤
    │ -  │ 9  │  1 │  - │ 3  │ 6  │ -  │ 7  │ 5  │  │ 2  │ 9  │  1 │  4 │ 3  │ 6  │ 8  │ 7  │ 5  │
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┼────┼────┼────┤
    │ 7  │ -  │  6 │  1 │ 8  │ 5  │ 4  │ -  │ 9  │  │ 7  │ 3  │  6 │  1 │ 8  │ 5  │ 4  │ 2  │ 9  │
    └────┴────┴────┴────┴────┴────┴────┴────┴────┘  └────┴────┴────┴────┴────┴────┴────┴────┴────┘
    Explanation:
        The empty cells are filled with the possible numbers.
        There can exist many such arrangements of numbers.
        The above solution is one of them.
        Let’s see how we can fill the cells below.
 */
bool isValid(char board[9][9], int row, int col, char c) {

    for (int i = 0; i < 9; i++) {
        if (board[i][col] == c) {
            return false;
        }

        if (board[row][i] == c) {
            return false;
        }

        if (board[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == c) {
            return false;
        }
    }
    return true;
}

bool solve (char board[9][9], int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == '.') {
                for (char c = '1'; c <= '9'; c++) {
                    if (isValid(board, i, j, c)) {
                        board[i][j] = c;
                        if (solve(board, size)) {
                            return true;
                        } else {
                            board[i][j] = '.';
                        }
                    }
                }
                return false;
            }
        }
    }

    return true;
}

int main() {
    char board[9][9] = {
                        {'9', '5', '7', '.', '1', '3', '.', '8', '4'},
                        {'4', '8', '3', '.', '5', '7', '1', '.', '6'},
                        {'.', '1', '2', '.', '4', '9', '5', '3', '7'},
                        {'1', '7', '.', '3', '.', '4', '9', '.', '2'},
                        {'5', '.', '4', '9', '7', '.', '3', '6', '.'},
                        {'3', '.', '9', '5', '.', '8', '7', '.', '1'},
                        {'8', '4', '5', '7', '9', '.', '6', '1', '3'},
                        {'.', '9', '1', '.', '3', '6', '.', '7', '5'},
                        {'7', '.', '6', '1', '8', '5', '4', '.', '9'}
                    };

    printf("\r\nInput : \r\n");
    for(int i= 0; i< 9; i++) {
        for(int j= 0; j< 9; j++) {
            printf("%3c", board[i][j]);
        }
        printf("\r\n");
    }

    solve(board, 9);

    printf("\r\nSolution : \r\n");
    for(int i= 0; i< 9; i++) {
        for(int j= 0; j< 9; j++) {
            printf("%3c", board[i][j]);
        }
        printf("\r\n");
    }

    printf("\r\n");
    return 0;
}