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
Count of rooks that can attack each other out of K rooks placed on a N*N chessboard

    Given pair of coordinates of K rooks on an N X N chessboard,
    the task is to count the number of rooks that can attack each other.
    Note: 1 <= K <= N*N

    Input: K = 2, arr[][] = { {2, 2}, {2, 3} }, N = 8
    Output: 2
    Explanation: Both the rooks can attack each other, because they are in the same row.
                 Therefore, count of rooks that can attack each other is 2


    Input: K = 1, arr[][] = { {4, 5} }, N = 4
    Output: 0

    Approach: The task can easily be solved using the fact that,
              2 rooks can attack each other if they are either in the same row
              or in the same column, else they can’t attack each other.
*/
// Function to count the number of attacking rooks
int willAttack(int arr[][2], int k, int N)
{
    int ans = 0;

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (i != j) {

                // Check if rooks are in same row or same column
                if ((arr[i][0] == arr[j][0]) || (arr[i][1] == arr[j][1])) {
                    ans++;
                }
            }
        }
    }

    return ans;
}

// Driver Code
int main()
{
    int arr[][2] = { { 2, 2 }, { 2, 3 } };
    int K = 2, N = 8;
    printf("There are %d rooks in %dx%d board out of which %d attach each other\r\n",K, N, N, willAttack(arr, K, N));
    return 0;
}