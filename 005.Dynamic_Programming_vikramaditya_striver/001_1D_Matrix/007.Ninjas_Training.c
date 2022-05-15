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
// https://takeuforward.org/data-structure/dynamic-programming-ninjas-training-dp-7/

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)

int solveMemoization(int day, int last, int points[][3], int dp[][4])
{
    if (dp[day][last] != -1) {
        return dp[day][last];
    }

    if (day == 0) {
        int maxi = 0;
        for (int i = 0; i <= 2; i++) {
            if (i != last) {
                maxi = MAX(maxi, points[0][i]);
            }
        }

        return dp[day][last] = maxi;
    }

    int maxi = 0;
    for (int i = 0; i <= 2; i++) {
        if (i != last) {
            int activity = points[day][i] + solveMemoization(day - 1, i, points, dp);
            maxi = MAX(maxi, activity);
        }

    }

    return dp[day][last] = maxi;
}

int ninjaTrainingMemoization(int points[][3], int n)
{

    int dp[n][4];
    memset(dp, -1, sizeof(dp));

    return solveMemoization(n - 1, 3, points, dp);
}

int ninjaTrainingTabulation(int points[][3], int n)
{
    int dp[n][4];
    memset(dp, 0, sizeof(dp));

    dp[0][0] = MAX(points[0][1], points[0][2]);
    dp[0][1] = MAX(points[0][0], points[0][2]);
    dp[0][2] = MAX(points[0][0], points[0][1]);
    dp[0][3] = MAX(points[0][0], MAX(points[0][1], points[0][2]));

    for (int day = 1; day < n; day++) {
        for (int last = 0; last < 4; last++) {
            dp[day][last] = 0;
            for (int task = 0; task <= 2; task++) {
                if (task != last) {
                    int activity = points[day][task] + dp[day - 1][task];
                    dp[day][last] = MAX(dp[day][last], activity);
                }
            }
        }
    }

    return dp[n - 1][3];
}

int ninjaTrainingOptimized(int points[][3], int n)
{
    int prev[4] = {0};
    int temp[4] = {0};

    prev[0] = MAX(points[0][1], points[0][2]);
    prev[1] = MAX(points[0][0], points[0][2]);
    prev[2] = MAX(points[0][0], points[0][1]);
    prev[3] = MAX(points[0][0], MAX(points[0][1], points[0][2]));

    for (int day = 1; day < n; day++) {
        for (int last = 0; last < 4; last++) {

            temp[last] = 0;
            for (int task = 0; task <= 2; task++) {
                if (task != last) {
                    temp[last] = MAX(temp[last], points[day][task] + prev[task]);
                }
            }
        }

        prev[0] = temp[0];
        prev[1] = temp[1];
        prev[2] = temp[2];
        prev[3] = temp[3];
    }

    return prev[3];
}

int main()
{
    int points[][3] = {{10,40,70},
                       {20,50,80},
                       {30,60,90}};

    int n = SIZEOF(points);
    printf("[Memoization] Maximum merit ninja gets is : %d \r\n",  ninjaTrainingMemoization(points, n));
    printf("[Tabulation] Maximum merit ninja gets is : %d \r\n",  ninjaTrainingTabulation(points, n));
    printf("[Optimized] Maximum merit ninja gets is : %d \r\n",  ninjaTrainingOptimized(points, n));
}