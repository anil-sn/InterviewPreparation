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
longest_common_subsequence(char X[], int m, char Y[], int n) {

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
    return(dp[m][n]);
}

int main () {
    char X[] = "abc";
    char Y[] = "abedcgh";
    int m = strlen(X);
    int n = strlen(Y);
    int lcs = longest_common_subsequence(X, m, Y, n);

    printf ("Longest Common substring %d\r\n", lcs);
    if (m == lcs) {
        printf("String %s is substring of %s\r\n", X, Y);
    } else {
        printf("String %s is not a substring of %s\r\n", X, Y);
    }
}