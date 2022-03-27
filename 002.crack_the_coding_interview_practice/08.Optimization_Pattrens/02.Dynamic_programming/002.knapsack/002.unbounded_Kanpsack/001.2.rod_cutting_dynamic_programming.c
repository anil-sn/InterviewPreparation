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
    Given a rod of length n inches and an array of prices that includes prices of all pieces of size smaller than n. Determine the maximum value obtainable by cutting up the rod and selling the pieces. For example, if the length of the rod is 8 and the values of different pieces are given as the following, then the maximum obtainable value is 22 (by cutting in two pieces of lengths 2 and 6)

    length   | 1   2   3   4   5   6   7   8
    --------------------------------------------
    price    | 1   5   8   9  10  17  17  20
    And if the prices are as following, then the maximum obtainable value is 24 (by cutting in eight pieces of length 1)

    length   | 1   2   3   4   5   6   7   8
    --------------------------------------------
    price    | 3   5   8   9  10  17  17  20
*/

int max(int a, int b) { return (a > b) ? a : b; }

/*
    Input:

        length[] = [1, 2, 3, 4,  5,  6,  7,  8]
        price[] =  [1, 5, 8, 9, 10, 17, 17, 20]

    Rod length: 4


    Best: Cut the rod into two pieces of length 2 each to gain revenue of 5 + 5 = 10

        Cut           Profit
        4                9
        1, 3            (1 + 8) = 9
        2, 2            (5 + 5) = 10
        3, 1            (8 + 1) = 9
        1, 1, 2         (1 + 1 + 5) = 7
        1, 2, 1         (1 + 5 + 1) = 7
        2, 1, 1         (5 + 1 + 1) = 7
        1, 1, 1, 1      (1 + 1 + 1 + 1) = 4
*/
int rodCutting(int length[], int rodLength, int price[], int Cost)
{
    // Step 1: Initialization
    int t[rodLength + 1][Cost + 1];

    for (int i = 0; i < rodLength + 1; i++) {
        for (int j = 0; j < Cost + 1; j++ ){
            if (i == 0 || j == 0)
                t[i][j] = 0;
        }
    }

    // Step 2: Iterative code
    for (int i = 1; i < rodLength + 1; i++) {
        for (int j = 1; j < Cost + 1; j++) {
            if (length[i - 1] <= j)
                t[i][j] = max(price[i - 1] + t[i][j - length[i - 1]], t[i - 1][j]);
            else
                t[i][j] = t[i - 1][j];
        }
    }

    printf("After Calculation: \r\n");
    for (int i = 0; i < rodLength + 1; i++) {
        for (int j = 0; j < Cost + 1; j++) {
            printf("%d,%d=%2d  ",i, j, t[i][j]);
        }
        printf ("\r\n");
    }
    printf ("\r\n");

    // Step 3: Return
    return t[rodLength][Cost];
}

int main()
{
    int length[] = {1, 2, 3, 4,  5,  6,  7,  8};
    int price[] =  {1, 5, 8, 9, 10, 17, 17, 20};
    int size = sizeof(length) / sizeof(length[0]);
    int N = 8;

    int profit = rodCutting(length, size, price, N);
    printf("The profit will be %d\r\n", profit);

    return 0;
}
