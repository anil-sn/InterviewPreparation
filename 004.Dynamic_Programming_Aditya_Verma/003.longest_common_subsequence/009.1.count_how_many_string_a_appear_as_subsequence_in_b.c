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
    Find number of times a string occurs as a subsequence in given string
    Given two strings, find the number of times the second string occurs in the first string, whether continuous or discontinuous.

    Examples:

        Input:
            string a = "GeeksforGeeks"
            string b = "Gks"
        Explanation:
        The four strings are
        G<eek<s<forGeeks
        G<eeksforGeek<s<
        G<eek<sforGeeks<
        GeeksforG<eek<s<

    If we carefully analyze the given problem, we can see that it can be easily divided into sub-problems.
    The idea is to process all characters of both strings one by one starting from either from left or right side.
    Let us traverse from right corner, there are two possibilities for every pair of character being traversed.

    m: Length of str1 (first string)
    n: Length of str2 (second string)

    If last characters of two strings are same,
    1. We consider last characters and get count for remaining strings.
        So we recur for lengths m-1 and n-1.
    2. We can ignore last character of first string and recurse for lengths m-1 and n.
    else If last characters are not same,
       We ignore last character of first string and recurse for lengths m-1 and n.
*/

int
count_common_subsequence(char X[], int m, char Y[], int n) {

    int dp[m+1][n+1];


    for (int i = 0; i <= m; i++){
        for (int j = 0; j <= n; j++) {
            dp[i][j] = 0;
        }
    }

    // If first string is empty
    for (int i = 0; i <= n; ++i)
        dp[0][i] = 0;

    // If second string is empty
    for (int i = 0; i <= m; ++i)
        dp[i][0] = 1;

    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= n; j++) {
            if (X[i-1] == Y[j-1]) {
                // If last characters are same, we have two
                // options -
                    // 1. consider last characters of both strings in solution
                    // 2. ignore last character of first string
                dp[i][j] = dp[i-1][j-1] + dp[i-1][j];;
            } else {
                // If last character are different, ignore
                // last character of first string
                dp[i][j] = dp[i - 1][j];
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

int count(char a[], char b[])
{
    int m = strlen(a);
    int n = strlen(b);

    // Create a table to store results of sub-problems
    int lookup[m + 1][n + 1];

    // If first string is empty
    for (int i = 0; i <= n; ++i)
        lookup[0][i] = 0;

    // If second string is empty
    for (int i = 0; i <= m; ++i)
        lookup[i][0] = 1;

    // Fill lookup[][] in bottom up manner
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            // If last characters are same, we have two
            // options -
            // 1. consider last characters of both strings
            //    in solution
            // 2. ignore last character of first string
            if (a[i - 1] == b[j - 1])
                lookup[i][j] = lookup[i - 1][j - 1] +
                               lookup[i - 1][j];

            else
                // If last character are different, ignore
                // last character of first string
                lookup[i][j] = lookup[i - 1][j];
        }
    }

    return lookup[m][n];
}

int main () {
    char X[] = "GeeksforGeeks";
    char Y[] = "Gks";
    int m = strlen(X);
    int n = strlen(Y);

    printf ("Count Common subsequence1 %d\r\n", count_common_subsequence(X, m, Y, n));
    printf ("Count Common subsequence2 %d\r\n", count(X,Y));
}