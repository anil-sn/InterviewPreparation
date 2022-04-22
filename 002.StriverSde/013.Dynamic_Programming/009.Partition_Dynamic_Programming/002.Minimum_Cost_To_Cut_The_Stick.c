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
// https://youtu.be/vRVfmbCFW7Y

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int cmp(const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    return (x - y);
}

int mincutsUtil(int i, int j, int cuts[])
{
    if (i > j) {
        return 0;
    }

    int mini = INT_MAX;
    for (int ind = i; ind <= j; ind++){
        int cost = ((cuts[j+1] - cuts[i-1])
                   + mincutsUtil(i, ind - 1, cuts)
                   + mincutsUtil(ind + 1 , j, cuts));

        mini = MIN(mini, cost);
    }

    return mini;
}

int minCuts(int c, int cuts[])
{
    return mincutsUtil(1, c, cuts);
}

int mincutsUtilMemoization(int i, int j, int cuts[], int row, int col, int dp[row][col])
{
    if (i > j) {
        return 0;
    }

    if (-1 != dp[i][j]){
        return dp[i][j];
    }

    int mini = INT_MAX;
    for (int ind = i; ind <= j; ind++){
        int cost = ((cuts[j+1] - cuts[i-1])
                   + mincutsUtilMemoization(i, ind-1, cuts, row, col, dp)
                   + mincutsUtilMemoization(ind +1 , j, cuts, row, col, dp));

        mini = MIN(mini, cost);
    }

    dp[i][j] = mini;
    return dp[i][j];
}

int minCutsMemoization(int c, int cuts[])
{
    int dp[c+1][c+1];
    memset(dp, -1, sizeof(dp));

    return mincutsUtilMemoization(1, c, cuts, c+1, c+1, dp);
}

int minCutsTabulation(int c, int cuts[])
{
    int dp[c+2][c+2];
    memset(dp, 0, sizeof(dp));

    for (int i = c; i >= 1; i--) {
        for( int j = 1; j <= c; j++) {

            if (i > j) {
                continue;
            }

            int mini = INT_MAX;
            for (int ind = i; ind <= j; ind++){
                int cost = ((cuts[j+1] - cuts[i-1])
                            + dp[i][ind-1]
                            + dp[ind+1][j]);

                mini = MIN(mini, cost);
            }
            dp[i][j] = mini;
        }
    }

    return dp[1][c];
}

void prependAppend(int stickLength, int oldLen, int oldArr[], int newLen, int newArr[])
{
    int i;
    newArr[0] = 0;
    for (i = 1; i <= oldLen; i++){
        newArr[i] = oldArr[i-1];
    }
    newArr[i] = stickLength;

    printf("Input Array : \r\n");
    for (i = 0; i < newLen; i++){
        printf("%4d", newArr[i]);
    }
    printf("\r\n");

    qsort(newArr, newLen, sizeof(newArr[0]), cmp);
    printf("Sorted Array : \r\n");
    for (i = 0; i < newLen; i++){
        printf("%4d", newArr[i]);
    }
    printf("\r\n");

    return;
}

// Cost is length of stick which is being cut
int main ()
{
    int arr[] = {1, 3, 4, 7};
    int n = SIZEOF(arr);
    int stickLength = 10;
    int cutsCost = 0;

    int newArr[n+2];
    int newN = n+2;

    prependAppend(stickLength, n, arr, newN, newArr);

    cutsCost = minCuts(n, newArr);
    printf("[recursion] The minimum number of cuts : %d \r\n", cutsCost);

    cutsCost = minCutsMemoization(n, newArr);
    printf("[memoization] The minimum number of cuts : %d \r\n", cutsCost);

    cutsCost = minCutsTabulation(n, newArr);
    printf("[tabulation] The minimum number of cuts : %d \r\n", cutsCost);

    return 0;
}