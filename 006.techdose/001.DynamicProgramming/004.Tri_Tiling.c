#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// https://open.kattis.com/problems/tritiling
// https://youtu.be/yn2jnmlepY8

int solve (int n)
{
    int dp[n + 1][1 << 3];
    memset(dp, 0, sizeof(dp));

    dp[0][7] = 1;
    for (int i = 1; i < n + 1; i++) {
      // The number of empty states for this column is the number
      // of full states in the previous column.
      dp[i][0] += dp[i - 1][7];

      dp[i][1] += dp[i - 1][6];

      // State 2 doesn't contribute to the number of tilings
      // dp[i][2] += dp[i-1][5];

      dp[i][3] += dp[i - 1][7];
      dp[i][3] += dp[i - 1][4];

      dp[i][4] += dp[i - 1][3];

      // State 5 doesn't contribute to the number of tilings
      // dp[i][5] += dp[i-1][2];

      dp[i][6] += dp[i - 1][7];
      dp[i][6] += dp[i - 1][1];

      dp[i][7] += dp[i - 1][3];
      dp[i][7] += dp[i - 1][6];
      dp[i][7] += dp[i - 1][0];
    }

    for (int i = 0; i < n+1; i++) {
      for (int j = 0; j < 1 << 3; j++) {
        printf("% 5d,", dp[i][j]);
      }
      printf("\r\n");
    }

    return dp[n][7];
}


int main()
{
    int size_of_grid = 8;
    printf ("Number of ways for board of is : %d\r\n", solve(size_of_grid));
}