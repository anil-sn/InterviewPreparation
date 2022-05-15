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
// https://youtu.be/ZmlQ3vgAOMo

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int editStringUtilMemoization(char pattren[], char text[], int ind1, int ind2, int row, int col, int dp[row][col])
{
    if((ind1 < 0) && (ind2 < 0)) {
        return true;
    }

    if ((ind1 < 0) && (ind2 >= 0)) {
        return false;
    }

    if ((ind2 < 0) && (ind1 >= 0)) {
        for (int i = 0; i <= ind1; i++) {
            if (pattren[i] != '*') {
                return false;
            }
        }
        return true;
    }

    if(-1 != dp[ind1][ind2]) {
        return dp[ind1][ind2];
    }

    if((pattren[ind1] == text[ind2]) || (pattren[ind1] == '?')) {
        return dp[ind1][ind2] = editStringUtilMemoization(pattren, text, ind1-1, ind2-1, row, col, dp);
    }
    if (pattren[ind1] == '*') {
        return dp[ind1][ind2] = (editStringUtilMemoization(pattren, text, ind1 - 1, ind2, row, col, dp) ||
                          editStringUtilMemoization(pattren, text, ind1, ind2 - 1, row, col, dp));
    }

    return dp[ind1][ind2] = false;
}

int editStringMemoization(char t[], char s[], int lt, int ls)
{
    int dp[lt][ls];
    memset(dp, -1, sizeof(dp));
    int result = 0;

    result = editStringUtilMemoization(t, s, lt-1, ls-1, lt, ls, dp);
    return result;
}

int editStringTabulation(char pattren[], char text[], int lt, int ls)
{
    int dp[lt+1][ls+1];
    memset(dp, 0, sizeof(dp));

    dp[0][0] = true;
    for (int j = 1; j <= ls; j++) {
        dp[0][j] = false;
    }
    for (int i = 1; i <= lt; i++) {
        bool flag = true;
        for (int j = 1; j <= i; j++) {
            if (pattren[j - 1] != '*') {
                flag = false;
                break;
            }
        }
        dp[i][0] = flag;
    }

    for (int ind1 = 1; ind1 <= lt; ind1++) {
        for (int ind2 = 1; ind2 <= ls; ind2++) {

            if((pattren[ind1 - 1] == text[ind2 - 1]) || (pattren[ind1 - 1] == '?')) {
                dp[ind1][ind2] = dp[ind1-1][ind2-1];
            }
            else if (pattren[ind1 - 1] == '*') {
                dp[ind1][ind2] = (dp[ind1 - 1][ind2] || dp[ind1][ind2 - 1]);
            } else {
                dp[ind1][ind2] = false;
            }
        }
    }

    return dp[lt][ls];
}

int main() {

    char pattren[] = "ab*c?g";
    char text[] = "abdefcdg";
    bool result = false;
    int n = strlen(pattren);
    int m = strlen(text);

    result = editStringMemoization(pattren, text, n, m);
    printf("[Memoization] Is Wild Card Matching Success : %s \r\n", result ? "True" : "False" );

    result = editStringTabulation(pattren, text, n, m);
    printf("[Tabulation] Is Wild Card Matching Success : %s \r\n", result ? "True" : "False" );

    return 0;
}