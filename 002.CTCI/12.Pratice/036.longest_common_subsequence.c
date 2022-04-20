#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int Max(int a, int b) {
    return (a > b ? a : b);
}

int lcs(char *s1, char*s2, int m, int n){
    if ((0 == m) || (0 == n)) {
        return 0;
    }

    if (s1[m-1] == s2[n-1]){
        return 1 + lcs(s1, s2, m-1, n-1);
    } else {
        return Max(lcs(s1, s2, m-1,n), lcs(s1, s2, m, n-1));
    }

    return 0;
}

// Driver code
int main()
{
    char s1[] = "AAACCGTGAGTTAT";
    char s2[] = "CACCCCTAAGGTA";

    printf("Longest common subsequence : %d\r\n", lcs(s1, s2, strlen(s1), strlen(s2)));
    return 0;
}