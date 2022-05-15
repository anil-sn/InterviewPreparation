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
    Given two strings X and Y, print the shortest string that has both X and Y
    as subsequences. If multiple shortest supersequence exists,
    print any one of them.
    Examples:


    Input: X = "AGGTAB",  Y = "GXTXAYB"
    Output: "AGXGTXAYB" OR "AGGXTXAYB"
        OR Any string that represents shortest
        supersequence of X and Y

    Input: X = "HELLO",  Y = "GEEK"
    Output: "GEHEKLLO" OR "GHEEKLLO"
        OR Any string that represents shortest
        supersequence of X and Y

    We start from the bottom-right most cell of the matrix and
    push characters in output string based on below rules-

        1. If the characters corresponding to current cell (i, j)
            in X and Y are same, then the character is part of shortest
            supersequence. We append it in output string and move
            diagonally to next cell (i.e. (i - 1, j - 1)).

        2. If the characters corresponding to current cell (i, j)
            in X and Y are different, we have two choices -

            If matrix[i - 1][j] > matrix[i][j - 1],
            we add character corresponding to current
            cell (i, j) in string Y in output string
            and move to the left cell i.e. (i, j - 1)
            else
            we add character corresponding to current
            cell (i, j) in string X in output string
            and move to the top cell i.e. (i - 1, j)

        3. If string Y reaches its end i.e. j = 0, we add remaining
            characters of string X in the output string
            else if string X reaches its end i.e. i = 0, we add
            remaining characters of string Y in the output string.
*/

int
shortest_common_supersequence(char X[], int m, char Y[], int n) {

    if ((0 == n) || (0 == m)) {
        return 0;
    }

    int dp[m+1][n+1];
    for (int i = 0; i <= m; i++){
        for (int j = 0; j <= n; j++) {
            dp[i][j] = 0;
            printf("%2d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= n; j++) {
            if (X[i-1] == Y[j-1]) {
                dp[i][j] = 1 + dp[i-1][j-1];
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
            printf ("X[%d]= %c, Y[%d]=%c DP= %d\r\n", i-1, X[i-1], j-1, Y[j-1], dp[i][j]);
        }

        printf ("\r\nInput Strings %s & %s\r\n", X, Y);
        for (int i = 0; i <= m; i++){
            for (int j = 0; j <= n; j++) {
                printf("%2d", dp[i][j]);
            }
            printf("\r\n");
        }
        printf("\r\n");
    }

    // string to store the shortest supersequence
    char str[m+n+1];
    str[m+n] = '\0';
    int pos = 0;

    // Start from the bottom right corner and one by one
    // push characters in output string
    int i = m, j = n;
    while (i > 0 && j > 0)
    {
        // If current character in X and Y are same, then
        // current character is part of shortest supersequence
        if (X[i - 1] == Y[j - 1])
        {
            // Put current character in result
            str[pos] = (X[i - 1]);
            pos ++;

            // reduce values of i, j and index
            i--;
            j--;
        }
        else
        {
            // If current character in X and Y are different
            if (dp[i][j-1] > dp[i - 1][j]) {
                // Put current character of X in result
                str[pos] = (Y[j - 1]);
                pos ++;

                // reduce values of j and index
                j--;
            } else if (dp[i - 1][j] >= dp[i][j-1]) {

                // Put current character of X in result
                str[pos] = (X[i - 1]);
                pos ++;

                // reduce values of j and index
                i--;
            }
        }
    }

    // If Y reaches its end, put remaining characters
    // of X in the result string
    while (i > 0)
    {
        str[pos] = (X[i - 1]);
        pos ++;
        i--;
    }

    // If X reaches its end, put remaining characters
    // of Y in the result string
    while (j > 0)
    {
        str[pos] = (Y[j - 1]);
        pos ++;
        j--;
    }

    str[pos] = '\0';

    // reverse the string and return it
    printf("Shortest Common supersequence : %s\r\n", str);
    return((m+n)-dp[m][n]);
}

int main () {
    char X[] = "AGGTAB";
    char Y[] = "GXTXAYB";
    int m = strlen(X);
    int n = strlen(Y);
    int scs = shortest_common_supersequence(X, m, Y, n);
    printf ("Shortest Common supersequence lenght is %d \r\n",scs);
}