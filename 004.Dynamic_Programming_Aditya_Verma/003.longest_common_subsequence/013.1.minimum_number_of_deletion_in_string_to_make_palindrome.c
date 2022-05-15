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

int
print_longest_palindromic_subsequence(char X[], int m, char Y[], int n) {

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
                dp[i][j] += 1 + dp[i-1][j-1];
            } else {
                dp[i][j] += max(dp[i-1][j], dp[i][j-1]);
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

    // Following code is used to print LCS
    int index = dp[m][n];

    // Create a character array to store the lcs string
    char lcs[index+1];
    lcs[index] = '\0'; // Set the terminating character

    // Start from the right-most-bottom-most corner and
    // one by one store characters in lcs[]
    int i = m, j = n;
    while (i > 0 && j > 0)
    {
        // If current character in X[] and Y are same, then
        // current character is part of LCS
        if (X[i-1] == Y[j-1])
        {
            lcs[index-1] = X[i-1]; // Put current character in result
            i--; j--; index--;     // reduce values of i, j and index
        }

        // If not same, then find the larger of two and
        // go in the direction of larger value
        else if (dp[i-1][j] > dp[i][j-1]) {
            i--;
        } else {
            j--;
        }
    }

    printf("Longest palindromic subsequence : %s\r\n", lcs);
    return(dp[m][n]);
}

int main () {
    char X[] = "ahbcdbah";
    int m = strlen(X);
    char Y[9] = {'\0'};

    int pos = 0;
    for(int i = m-1; i >= 0; i--)
    {
        Y[pos] = X[i];
        pos++;
    }
    int n = strlen(Y);

    int lps = print_longest_palindromic_subsequence(X, m, Y, n);
    printf ("Longest palindromic subsequence %d\r\n", lps);
    printf ("Minimum number of deletion to get Longest palindromic subsequence %d\r\n", m-lps);
}