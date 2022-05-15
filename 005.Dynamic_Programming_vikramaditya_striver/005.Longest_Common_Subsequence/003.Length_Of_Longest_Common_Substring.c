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
// https://takeuforward.org/data-structure/longest-common-substring-dp-27/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int recurAns = 0;
int lcsUtilMemoization(char s1[], char s2[], int ind1, int ind2, int row, int col, int dp[row][col])
{

    int val = 0;

    if(ind1 < 0 || ind2 < 0)
        return 0;

    if(-1 != dp[ind1][ind2])
        return dp[ind1][ind2];

    if(s1[ind1] == s2[ind2]) {

        val = 1 + lcsUtilMemoization(s1,s2,ind1-1,ind2-1, row, col, dp);
        dp[ind1][ind2] = val;
        recurAns = MAX(recurAns, dp[ind1][ind2]);
    } else {
        dp[ind1][ind2] = 0;

        val = lcsUtilMemoization(s1,s2,ind1-1,ind2, row, col, dp);
        recurAns = MAX(recurAns, dp[ind1][ind2]);

        val = lcsUtilMemoization(s1,s2,ind1,ind2-1, row, col, dp);
        recurAns = MAX(recurAns, dp[ind1][ind2]);
    }

    return recurAns;
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
    int ans = 0;

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

                int val = 1 + dp[ind1-1][ind2-1];
                dp[ind1][ind2] = val;
                ans = MAX(ans, dp[ind1][ind2]);
            } else {
                dp[ind1][ind2] = 0;
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

    return ans;
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

        }

        for (int i = 0; i <= m; i++){
            prev[i] = curr[i];
            printf("%3d", prev[i]);
        }
        printf("\r\n");
    }

    return ans;
}

int main()
{
    char s1[] = "abcde";
    char s2[] = "bdgek";
    int len = 0;

    printf("\r\nFind the length of Longest Common SubString : \n\n %10s \r\n %10s \n\n", s1, s2);

    len = lcsMemoization (s1,s2);
    printf("[Memoization] The Length of Longest Common SubString is %d \r\n", len);

    len = lcsTabulation (s1,s2);
    printf("[Tabulation] The Length of Longest Common SubString is %d \r\n", len);

    len = lcsOptimized (s1,s2);
    printf("[Optimized] The Length of Longest Common SubString is %d \r\n", len);
}