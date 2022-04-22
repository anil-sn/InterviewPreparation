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
// https://takeuforward.org/data-structure/3-d-dp-ninja-and-his-friends-dp-13/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int maxChocoUtilMemoization(int i, int j1, int j2, int n, int m, int grid[3][4], int dp[3][4][4])
{
    if (j1 < 0 || j1 >= m || j2 < 0 || j2 >= m) {
        return -1e8;
    }

    if (i == n - 1) {
        if (j1 == j2) {
            return grid[i][j1];
        } else {
            return grid[i][j1] + grid[i][j2];
        }
    }

    if (dp[i][j1][j2] != -1) {
        return dp[i][j1][j2];
    }

    int maxi = -1e8;
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            int ans;
            if (j1 == j2) {
                ans = grid[i][j1] + maxChocoUtilMemoization(i + 1, j1 + di, j2 + dj, n, m, grid, dp);
            } else {
                ans = grid[i][j1] + grid[i][j2] + maxChocoUtilMemoization(i + 1, j1 + di, j2 + dj, n, m, grid, dp);
            }
            maxi = MAX(maxi, ans);
        }
    }

    return dp[i][j1][j2] = maxi;
}

int maximumChocolatesMemoization(int n, int m, int grid[3][4]) {

    int dp[n][m][m];
    memset(dp, -1, sizeof(dp));

    return maxChocoUtilMemoization(0, 0, m - 1, n, m, grid, dp);
}

int maximumChocolatesTabulation(int n, int m, int grid[3][4]) {

    int dp[n][m][m];
    memset(dp, -1, sizeof(dp));

    for (int j1 = 0; j1 < m; j1++) {
        for (int j2 = 0; j2 < m; j2++) {
        if (j1 == j2)
            dp[n - 1][j1][j2] = grid[n - 1][j1];
        else
            dp[n - 1][j1][j2] = grid[n - 1][j1] + grid[n - 1][j2];
        }
    }

    //Outer Nested Loops for travering DP Array
    for (int i = n - 2; i >= 0; i--) {
        for (int j1 = 0; j1 < m; j1++) {
            for (int j2 = 0; j2 < m; j2++) {

                int maxi = -1e8;

                //Inner nested loops to try out 9 options
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {

                        int ans;

                        if (j1 == j2) {
                            ans = grid[i][j1];
                        } else {
                            ans = grid[i][j1] + grid[i][j2];
                        }

                        if ((j1 + di < 0 || j1 + di >= m) || (j2 + dj < 0 || j2 + dj >= m)) {
                            ans = -1e8;
                        } else {
                            ans += dp[i + 1][j1 + di][j2 + dj];
                        }

                        maxi = MAX(ans, maxi);
                    }
                }
                dp[i][j1][j2] = maxi;
            }
        }
    }

    return dp[0][0][m - 1];
}

int maximumChocolatesOptimization(int n, int m, int grid[3][4]) {

    int front[m][m];
    memset(front, 0, sizeof(front));

    int cur[m][m];
    memset(cur, 0, sizeof(cur));

    for (int j1 = 0; j1 < m; j1++) {
        for (int j2 = 0; j2 < m; j2++) {
            if (j1 == j2) {
               front[j1][j2] = grid[n - 1][j1];
            } else {
                front[j1][j2] = grid[n - 1][j1] + grid[n - 1][j2];
            }
        }
    }

    //Outer Nested Loops for travering DP Array
    for (int i = n - 2; i >= 0; i--) {
        for (int j1 = 0; j1 < m; j1++) {
            for (int j2 = 0; j2 < m; j2++) {

                int maxi = -1e8;

                //Inner nested loops to try out 9 options
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {

                        int ans;

                        if (j1 == j2) {
                            ans = grid[i][j1];
                        } else {
                            ans = grid[i][j1] + grid[i][j2];
                        }

                        if ((j1 + di < 0 || j1 + di >= m) || (j2 + dj < 0 || j2 + dj >= m)) {
                            ans = -1e8;
                        } else {
                            ans += front[j1 + di][j2 + dj];
                        }

                        maxi = MAX(ans, maxi);
                    }
                }
                cur[j1][j2] = maxi;
            }
        }

        for (int j1 = 0; j1 < m; j1++) {
            for (int j2 = 0; j2 < m; j2++) {
                front[j1][j2] = cur[j1][j2];
            }
        }
    }

    return front[0][m - 1];
}

int main() {

    int matrix[][4] = {{2,3,1,2},
                       {3,4,2,2},
                       {5,6,3,5}};

    int n = 3;
    int m = 4;

    printf("[Memoization] Maximum choclates collected by Alice and Bob : %d \r\n", maximumChocolatesMemoization(n, m, matrix));
    printf("[Tabulation] Maximum choclates collected by Alice and Bob : %d \r\n", maximumChocolatesTabulation(n, m, matrix));
    printf("[Optimization] Maximum choclates collected by Alice and Bob : %d \r\n", maximumChocolatesOptimization(n, m, matrix));

    return 0;
}
