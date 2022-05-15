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

void
longest_common_substring(char X[], int m, char Y[], int n) {

   int len = 0;
   int row, col;

    if ((0 == n) || (0 == m)) {
        return;
    }

    int dp[m+1][n+1];
    for (int i = 0; i <= m; i++){
        for (int j = 0; j <= n; j++) {
            dp[i][j] = 0;
        }
    }

    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= n; j++) {
            if (X[i-1] == Y[j-1]) {
                dp[i][j] += 1 + dp[i-1][j-1];
                if (dp[i][j] > len) {
                    len = dp[i][j];
                    row = i;
                    col = j;
                }
            } else {
                dp[i][j] += 0;
            }
        }

    }

    printf ("\r\nInput Strings %s & %s\r\n", X, Y);
    for (int i = 0; i <= m; i++){
        for (int j = 0; j <= n; j++) {
            printf("%2d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    char lcs[10] = {'\0'};
    int index = 0;
    while (dp[row][col] != 0) {
        printf("  dp[%d,%d] = %d [%c]", row, col, dp[row][col], X[row - 1]);
        lcs[index++] = X[row - 1];
        row--;
        col--;
    }
    printf("\r\n\r\nLongest Common Substring is %s \r\n", lcs);

    return;
}

int main () {
    char X[] = "azecdfgh";
    char Y[] = "abedfgh";
    int m = strlen(X);
    int n = strlen(Y);

    (void)longest_common_substring(X, m, Y, n);
}