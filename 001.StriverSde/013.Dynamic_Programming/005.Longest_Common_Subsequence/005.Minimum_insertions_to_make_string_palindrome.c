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
// https://takeuforward.org/data-structure/minimum-insertions-to-make-string-palindrome-dp-29/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int recurAns = 0;
int lcsUtilMemoization(char s1[], char s2[], int ind1, int ind2, int row, int col, int dp[row][col])
{
    if(ind1 < 0 || ind2 < 0)
        return 0;

    if(-1 != dp[ind1][ind2])
        return dp[ind1][ind2];

    if(s1[ind1] == s2[ind2]) {

        dp[ind1][ind2] = 1 + lcsUtilMemoization(s1,s2,ind1-1,ind2-1, row, col, dp);
    } else {
        dp[ind1][ind2] = 0;

        dp[ind1][ind2] = 0 + MAX(lcsUtilMemoization(s1,s2,ind1-1,ind2, row, col, dp),
                                 lcsUtilMemoization(s1,s2,ind1,ind2-1, row, col, dp));
    }

    return dp[ind1][ind2];
}

int lcsMemoization(char s1[], char s2[]) {

    int n = strlen(s1);
    int m = strlen(s2);
    int result = 0;

    int dp[n][m];
    memset(dp, -1, sizeof(dp));
    result = lcsUtilMemoization(s1, s2, n-1, m-1, n, m, dp);

    printf("DP Output : \r\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%3d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return result;
}

int lcsTabulation(char s1[], char s2[])
{
    int n = strlen(s1);
    int m = strlen(s2);

    int dp[n+1][m+1];
    memset(dp, 0, sizeof(dp));

    for(int i = 0; i <= n; i++) {
        dp[i][0] = 0;
    }

    for(int i = 0; i <= m; i++) {
        dp[0][i] = 0;
    }

    for(int ind1 = 1; ind1 <= n; ind1++){
        for(int ind2 = 1; ind2 <= m; ind2++){

            if(s1[ind1-1] == s2[ind2-1]) {

                dp[ind1][ind2] = 1 + dp[ind1 - 1][ind2 - 1];
            } else {
                dp[ind1][ind2] = 0 + MAX(dp[ind1 - 1][ind2],dp[ind1][ind2 - 1]);
            }
        }
    }

    printf("DP Output : \r\n");
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            printf("%3d", dp[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return dp[n][m];
}

int lcsOptimized(char s1[], char s2[])
{
    int n = strlen(s1);
    int m = strlen(s2);
    int ans = 0;

    int prev[m+1];
    memset(prev, 0, sizeof(prev));

    int curr[m+1];
    memset(curr, 0, sizeof(curr));

    printf("DP Output : \r\n");
    for (int i = 0; i <= m; i++){
        prev[i] = curr[i];
        printf("%3d", prev[i]);
    }
    printf("\r\n");

    for(int ind1 = 1; ind1 <= n; ind1++){
        for(int ind2 = 1; ind2 <= m; ind2++){
            if(s1[ind1-1] == s2[ind2-1]) {
                int val = 1 + prev[ind2-1];
                curr[ind2] = val;
                ans = MAX(ans, curr[ind2]);
            } else {
                curr[ind2] = 0;
            }
            if(s1[ind1-1] == s2[ind2-1]) {

                curr[ind2] = 1 + prev[ind2 - 1];
            } else {
                curr[ind2] = 0 + MAX(prev[ind2],curr[ind2 - 1]);
            }

        }

        for (int i = 0; i <= m; i++){
            prev[i] = curr[i];
            printf("%3d", prev[i]);
        }
        printf("\r\n");
    }

    return prev[m];
}


typedef enum {
    MEMOIZATION = 1,
    TABULATION = 2,
    OPTIMIZED = 3
}FUNC;

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }

      return str;
}

int longestPalindromeSubsequence(char s[], FUNC func)
{
    int result = 0;
    int len = strlen(s);
    char t[len + 1];

    sprintf(t, "%s", s);
    (void) strrev(t);

    printf("String : %s, String : %s \r\n", s, t);

    if (MEMOIZATION == func) {
        return(lcsMemoization (s,t));
    }

    if (TABULATION == func) {
        return(lcsTabulation (s,t));
    }

    if (OPTIMIZED == func) {
        return(lcsOptimized (s,t));
    }

    return result;
}

int main()
{
    char s[] = "bbabcbcab";
    int strLen = strlen(s);
    int len = 0;

    len = longestPalindromeSubsequence(s, MEMOIZATION);
    printf("[memoization] The Minimum insertions required to make string palindrome: %d \r\n", (strLen - len));

    len = longestPalindromeSubsequence(s, TABULATION);
    printf("[tabulation] The Minimum insertions required to make string palindrome: %d \r\n", (strLen - len));

    len = longestPalindromeSubsequence(s, OPTIMIZED);
    printf("[optimized] The Minimum insertions required to make string palindrome: %d \r\n", (strLen - len));
}