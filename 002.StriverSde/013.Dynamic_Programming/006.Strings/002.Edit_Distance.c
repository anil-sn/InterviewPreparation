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
// https://www.youtube.com/watch?v=fJaKO8FbDdo&list=PLgUwDviBIf0qUlt5H_kiKYaNSqJ81PMMY&index=34&ab_channel=takeUforward

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int editStringUtilMemoization(char s1[], char s2[], int ind1, int ind2, int row, int col, int dp[row][col])
{
    if(ind2 < 0) {
        return (ind1 + 1);
    }

    if(ind1 < 0) {
        return (ind2 + 1);
    }

    if(-1 != dp[ind1][ind2]) {
        return dp[ind1][ind2];
    }

    if(s1[ind1] == s2[ind2]){
        return dp[ind1][ind2] = 0 + editStringUtilMemoization(s1, s2, ind1-1, ind2-1, row, col, dp);
    } else {
        dp[ind1][ind2] = 1 + MIN(editStringUtilMemoization(s1, s2, ind1, ind2 - 1, row, col, dp),
                                MIN(editStringUtilMemoization(s1, s2, ind1 - 1, ind2, row, col, dp),
                                    editStringUtilMemoization(s1, s2, ind1 - 1, ind2 - 1, row, col, dp)));
    }

    return dp[ind1][ind2];
}

int editStringMemoization(char t[], char s[], int lt, int ls)
{
    int dp[lt][ls];
    memset(dp, -1, sizeof(dp));
    int result = 0;

    result = editStringUtilMemoization(t, s, lt-1, ls-1, lt, ls, dp);
    return result;
}

int editStringTabulation(char s1[], char s2[], int lt, int ls)
{
    int dp[lt+1][ls+1];
    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= lt; i++) {
        dp[i][0] = i;
    }

    for (int j = 1; j <= ls; j++) {
        dp[0][j] = j;
    }

    for (int ind1 = 1; ind1 <= lt; ind1++) {
        for (int ind2 = 1; ind2 <= ls; ind2++) {

            if(s1[ind1 - 1] == s2[ind2 - 1]){
                dp[ind1][ind2] = 0 + dp[ind1-1][ind2-1];
            } else {
                dp[ind1][ind2] = 1 + MIN(dp[ind1][ind2 - 1], MIN(dp[ind1 - 1][ind2], dp[ind1 - 1][ind2 - 1]));
            }
        }
    }

    for (int ind1 = 0; ind1 <= lt; ind1++) {
        for (int ind2 = 0; ind2 <= ls; ind2++) {
            printf("%3d",  dp[ind1][ind2]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return dp[lt][ls];
}

int editStringOptimization(char s1[], char s2[], int lt, int ls)
{
    int prev[ls+1];
    memset(prev, 0, sizeof(prev));

    int curr[ls+1];
    memset(curr, 0, sizeof(curr));

    for (int j = 0; j <= ls; j++) {
        prev[j] = j;
        printf("%3d", prev[j]);
    }
    printf("\r\n");

    for (int ind1 = 1; ind1 <= lt; ind1++) {

        curr[0] = ind1;
        for (int ind2 = 1; ind2 <= ls; ind2++) {

            if(s1[ind1 - 1] == s2[ind2 - 1]){
                curr[ind2] = 0 + prev[ind2-1];
            } else {
                curr[ind2] = 1 + MIN(prev[ind2], MIN(curr[ind2 - 1], prev[ind2 - 1]));
            }
        }

        for (int j = 0; j <= ls; j++) {
            prev[j] = curr[j];
            printf("%3d", prev[j]);
        }
        printf("\r\n");
    }

    return prev[ls];
}

int main() {

    char s1[] = "horse";
    char s2[] = "ros";
    int result = 0;
    int n = strlen(s1);
    int m = strlen(s2);

    result = editStringMemoization(s1, s2, n, m);
    printf("[Memoization] The Count of Distinct Subsequences is :%d \r\n", result);

    result = editStringTabulation(s1,s2,n,m);
    printf("[Tabulation] The Count of Distinct Subsequences is :%d \r\n", result);

    result = editStringOptimization(s1,s2,n,m);
    printf("[Optimization] The Count of Distinct Subsequences is :%d \r\n", result);

}