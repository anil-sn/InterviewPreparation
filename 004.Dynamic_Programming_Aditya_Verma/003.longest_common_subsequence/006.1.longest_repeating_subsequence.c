#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    Longest Repeating Subsequence
        Given a string, find the length of the longest repeating subsequence such
        that the two subsequences don’t have same string character at the same position,
        i.e., any i’th character in the two subsequences shouldn’t have the same index
        in the original string.
    Examples:

        Input: str = "abc"
        Output: 0
        There is no repeating subsequence

        Input: str = "aab"
        Output: 1
        The two subsequence are 'a'(first) and 'a'(second).
        Note that 'b' cannot be considered as part of subsequence
        as it would be at same index in both.

        Input: str = "aabb"
        Output: 2

        Input: str = "axxxy"
        Output: 2
*/

int
longest_repeating_subsequence(char X[], int m) {

    if ((0 == m)) {
        return 0;
    }

    int dp[m+1][m+1];
    for (int i = 0; i <= m; i++){
        for (int j = 0; j <= m; j++) {
            dp[i][j] = 0;
            printf("%2d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= m; j++) {
            if ((X[i-1] == X[j-1]) && (i != j)) {
                dp[i][j] += 1 + dp[i-1][j-1];
            } else {
                dp[i][j] += max(dp[i-1][j], dp[i][j-1]);
            }
            printf ("X[%d]= %c, X[%d]=%c DP= %d\r\n", i-1, X[i-1], j-1, X[j-1], dp[i][j]);
        }

        printf ("\r\nInput String: %s \r\n", X);
        for (int x = 0; x <= m; x++){
            for (int y = 0; y <= m; y++) {
                printf("%2d", dp[x][y]);
            }
            printf("\r\n");
        }
        printf("\r\n");
    }
    return(dp[m][m]);
}

int main () {
    char X[] = "abbcddghh";
    int lrs = longest_repeating_subsequence(X, strlen(X));

    printf ("Longest Repeating subsequence : %d\r\n", lrs);
    return 0;
}