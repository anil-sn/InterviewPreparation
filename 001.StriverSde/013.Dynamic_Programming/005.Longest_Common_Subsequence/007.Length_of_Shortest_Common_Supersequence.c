#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// https://takeuforward.org/dynamic-programming/striver-dp-series-dynamic-programming-problems/
// https://takeuforward.org/data-structure/shortest-common-supersequence-dp-31/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999


void shortestSupersequence(char s1[], int n, char s2[], int m, char result[])
{
    int dp[n+1][m+1];
    memset(dp, 0, sizeof(dp));

    for (int i = 0; i <= n; i++) {
        dp[i][0] = 0;
    }

    for (int i = 0; i <= m; i++) {
        dp[0][i] = 0;
    }

    for (int ind1 = 1; ind1 <= n; ind1++) {
        for (int ind2 = 1; ind2 <= m; ind2++) {

            if (s1[ind1 - 1] == s2[ind2 - 1]) {
                dp[ind1][ind2] = 1 + dp[ind1 - 1][ind2 - 1];
            } else {
                dp[ind1][ind2] = 0 + MAX(dp[ind1 - 1][ind2], dp[ind1][ind2 - 1]);
            }
        }
    }

    int len = dp[n][m];
    int i = n;
    int j = m;

    int index = len - 1;
    result[len] = '\0';
    int pos = 0;

    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            result[pos++] = s1[i-1];
            index--;
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            result[pos++] = s1[i-1];
            i--;
        } else {
            result[pos++] = s2[j-1];
            j--;
        }
    }

    //Adding Remaing Characters - Only one of the below two while loops will run
    while(i>0){
        result[pos++] = s1[i-1];
        i--;
    }

    while(j>0){
        result[pos++] = s2[j-1];
        j--;
    }

    printf("%s \r\n", result);
    int first = 0;
    int last = strlen(result)-1;
    char temp;
    while (first <= last) {

        temp = result[first];
        result[first] = result[last];
        result[last] = temp;

        /* Increment first index and decrement last index. */
        first++;
        last--;
    }
    printf("%s \r\n", result);

    return;
}

int main (void)
{
    char s1[] = "brute";
    char s2[] = "groot";
    int n = strlen(s1);
    int m = strlen(s2);
    char result[n+m];
    memset(result, '\0', sizeof(result));

    (void) shortestSupersequence(s1, n, s2, m, result);
    printf("The Longest Common Supersequence for %s,%s is %s \r\n", s1, s2, result);

    return 0;
}