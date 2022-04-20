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
// https://takeuforward.org/data-structure/distinct-subsequences-dp-32/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int subsequenceCountingUtilMemoization(char s1[], char s2[], int ind1, int ind2, int row, int col, int dp[row][col])
{
    if(ind2 < 0) {
        return 1;
    }

    if(ind1 < 0) {
        return 0;
    }

    if(-1 != dp[ind1][ind2]) {
        return dp[ind1][ind2];
    }

    if(s1[ind1] == s2[ind2]){
        int leaveOne = subsequenceCountingUtilMemoization(s1, s2, ind1-1, ind2-1, row, col, dp);
        int stay = subsequenceCountingUtilMemoization(s1, s2, ind1-1, ind2, row, col, dp);

        return dp[ind1][ind2] = (leaveOne + stay);
    } else {
        return dp[ind1][ind2] = subsequenceCountingUtilMemoization(s1, s2, ind1-1, ind2, row, col, dp);
    }
}

int subsequenceCountingMemoization(char t[], char s[], int lt, int ls)
{
    int dp[lt][ls];
    memset(dp, -1, sizeof(dp));
    int result = 0;

    result = subsequenceCountingUtilMemoization(t, s, lt-1, ls-1, lt, ls, dp);
    return result;
}

int subsequenceCountingTabulation(char s1[], char s2[], int lt, int ls)
{
    int dp[lt+1][ls+1];
    memset(dp, 0, sizeof(dp));

    for (int i = 0; i <= lt; i++) {
        dp[i][0] = 1;
    }

    for (int i = 0; i <= ls; i++) {
        dp[0][1] = 0;
    }

    for (int ind1 = 1; ind1 <= lt; ind1++) {
        for (int ind2 = 1; ind2 <= ls; ind2++) {

            if(s1[ind1 - 1] == s2[ind2 - 1]) {

                int leaveOne = dp[ind1-1][ind2-1];
                int stay = dp[ind1-1][ind2];
                dp[ind1][ind2] = (leaveOne + stay);
            } else {

                dp[ind1][ind2] = dp[ind1-1][ind2];
            }
        }
    }

    return dp[lt][ls];
}

int subsequenceCountingOptimization(char s1[], char s2[], int lt, int ls)
{
    int prev[ls+1];
    memset(prev, 0, sizeof(prev));

    int curr[ls+1];
    memset(curr, 0, sizeof(curr));

    prev[0] = 1;

    for (int ind1 = 1; ind1 <= lt; ind1++) {

        curr[0] = 1;
        for (int ind2 = 1; ind2 <= ls; ind2++) {

            if(s1[ind1 - 1] == s2[ind2 - 1]) {

                int leaveOne = prev[ind2-1];
                int stay = prev[ind2];
                curr[ind2] = (leaveOne + stay);
            } else {

                curr[ind2] = prev[ind2];
            }
        }

        for (int i = 1; i <= ls; i++) {
            prev[i] = curr[i];
        }
    }

    return prev[ls];
}

int subsequenceCountingOptimization2(char s1[], char s2[], int lt, int ls)
{
    int prev[ls+1];
    memset(prev, 0, sizeof(prev));

    prev[0] = 1;

    for (int ind1 = 1; ind1 <= lt; ind1++) {
        for (int ind2 = ls; ind2 >= 1; ind2--) {

            if(s1[ind1 - 1] == s2[ind2 - 1]) {

                int leaveOne = prev[ind2-1];
                int stay = prev[ind2];
                prev[ind2] = (leaveOne + stay);
            } else {

                prev[ind2] = prev[ind2];
            }
        }
    }

    return prev[ls];
}

int main() {

    char s1[] = "babgbag";
    char s2[] = "bag";
    int result = 0;
    int n = strlen(s1);
    int m = strlen(s2);

    result = subsequenceCountingMemoization(s1,s2,n,m);
    printf("[Memoization] The Count of Distinct Subsequences is :%d \r\n", result);

    result = subsequenceCountingTabulation(s1,s2,n,m);
    printf("[Tabulation] The Count of Distinct Subsequences is :%d \r\n", result);

    result = subsequenceCountingOptimization(s1,s2,n,m);
    printf("[Optimization] The Count of Distinct Subsequences is :%d \r\n", result);

    result = subsequenceCountingOptimization2(s1,s2,n,m);
    printf("[Optimization2] The Count of Distinct Subsequences is :%d \r\n", result);
}