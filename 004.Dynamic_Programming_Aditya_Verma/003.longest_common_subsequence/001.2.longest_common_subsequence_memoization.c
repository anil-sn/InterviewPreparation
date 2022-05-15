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

int result[100][100];

int
longest_common_subsequence(char X[], int n, char Y[], int m) {

    if ((0 == n) || (0 == m)) {
        return 0;
    }

    if (0 != result[n][m]) {
        return result[n][m];
    }

    if (X[n-1] == Y[m-1]) {
        result[n][m] = 1 + largest_common_subsequence(X, n-1, Y, m-1);
    } else {
        result[n][m] += max(largest_common_subsequence(X, n, Y, m-1),
                     largest_common_subsequence(X, n-1, Y, m));
    }

    return result[n][m];
}

int main () {
    char X[] = "abcdgh";
    char Y[] = "abedfh";
    int m = strlen(X);
    int n = strlen(Y);

    printf ("Input Strings %s & %s\r\n", X, Y);
    printf ("Longest Common Subsequence %d\r\n", longest_common_subsequence(X, m, Y, n));
}