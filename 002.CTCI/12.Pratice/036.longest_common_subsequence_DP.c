#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int **LCSCount;

int max(int a, int b) {
    return (a > b ? a : b);
}

int lcs(char *X, char*Y, int m, int n) {

    int i, j;

    LCSCount = (int**)malloc((m+1) * sizeof(int*));
    for (i = 0; i <= m; i++)
        LCSCount[i] = (int*)malloc((n+1) * sizeof(int));

    /* Following steps build LCSCount[m+1][n+1] in bottom up fashion. Note
        that LCSCount[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
    for (i=0; i<=m; i++) {
        for (j=0; j<=n; j++) {
            if (i == 0 || j == 0)
                LCSCount[i][j] = 0;

            else if (X[i-1] == Y[j-1])
                LCSCount[i][j] = LCSCount[i-1][j-1] + 1;

            else
                LCSCount[i][j] = max(LCSCount[i-1][j], LCSCount[i][j-1]);
        }
    }

    /* LCSCount[m][n] contains length of LCS for X[0..n-1] and Y[0..m-1] */
    return LCSCount[m][n];
}

void printLCSCountMatrix(int m, int n){
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j<=n; j++) {
            printf ("%2d ",LCSCount[i][j]);
        }
        printf ("\r\n");
    }
    printf ("\r\n");
}

void printlcs(char *X, char*Y, int m, int n) {

    int index = lcs(X, Y, m, n);
    int lcsLen = index;
    printLCSCountMatrix(m,n);

    // Following code is used to print LCS

    // Create a character array to store the lcs string
    char final[index+1];
    final[index] = '\0'; // Set the terminating character

    // Start from the right-most-bottom-most corner and
    // one by one store characters in lcs[]
    int i = m, j = n;
    while (i > 0 && j > 0)
    {
        // If current character in X[] and Y are same, then
        // current character is part of LCS
        if (X[i-1] == Y[j-1])
        {
        final[index-1] = X[i-1]; // Put current character in result
        i--; j--; index--;     // reduce values of i, j and index
        }

        // If not same, then find the larger of two and
        // go in the direction of larger value
        else if (LCSCount[i-1][j] > LCSCount[i][j-1])
        i--;
        else
        j--;
    }

    printf ("%s[%ld],%s[%ld]\r\n", X, strlen(X),Y, strlen(Y));
    printf("Longest common subsequence : %s[%d]\r\n", final, lcsLen);
    return;
}

// Driver code
int main() {
    char s1[] = "AAACCGTGAGTTATCCTA";
    char s2[] = "CACCCCTAAGGTAAACTA";

    printlcs(s1, s2, strlen(s1), strlen(s2));
    return 0;
}