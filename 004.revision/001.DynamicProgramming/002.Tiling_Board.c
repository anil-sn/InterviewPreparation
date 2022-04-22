#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// https://open.kattis.com/problems/magicalcows
// https://youtu.be/gQszF5qdZ-0


int solve (int ind, int dp[]) {
    if (0 > ind) {
        return 0;
    }

    if (0 == ind) {
        return 1;
    }

    if (-1 != dp[ind]){
        return dp[ind];
    }

    int ways = solve(ind - 1, dp) + solve(ind - 2, dp);
    return (ways);
}

int solveTabulation (int ind, int dp[]) {
    if ((0 == ind) || (1 == ind)) {
        return 1;
    }

    dp[0] = 1;
    dp[1] = 1;

    for (int i = 2; i <= ind; i++){
        dp[i] = dp[i-1] + dp[i-2];
    }

    return dp[ind];
}

int main ()
{
    int board = 5;
    int dp[board+1];
    memset(dp, -1, sizeof(dp));

    int ways = solve(board, dp);
    printf("[resursion] Number of ways board of size %d can be places with size 1 & 2 tiles : %d \r\n", board, ways);

    ways = solveTabulation(board, dp);
    printf("[tabulation] Number of ways board of size %d can be places with size 1 & 2 tiles : %d \r\n", board, ways);

    return 0;
}