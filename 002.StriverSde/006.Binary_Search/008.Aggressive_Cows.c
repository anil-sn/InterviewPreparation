#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Aggressive Cows : Detailed Solution
    Problem Statement: There is a new barn with N stalls and C cows.
        The stalls are located on a straight line at positions x1,….,xN (0 <= xi <= 1,000,000,000).
        We want to assign the cows to the stalls, such that the minimum distance between any two of
        them is as large as possible. What is the largest minimum distance?

    Examples:
        Input: No of stalls = 5
            Array: {1,2,8,4,9}
            And number of cows: 3

        Output: One integer, the largest minimum distance 3
*/
#define MAX(a_, b_) (a_ > b_ ? a_ : b_)
#define MIN(a_, b_) (a_ < b_ ? a_ : b_)

bool isCompatible(int inp[], int n, int dist, int cows)
{
    int k = inp[0];
    cows--;

    for (int i = 1; i < n; i++) {
        if (inp[i] - k >= dist) {
            cows--;
            if (!cows) {
                return true;
            }

            k = inp[i];
        }
    }

    return false;
}

int cmp (const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    return (x-y);
}

int getMinimumDistance(int inp[], int n, int m)
{
    int ans = INT_MIN;

    qsort(inp, n, sizeof(int), cmp);
    int maxD = inp[n - 1] - inp[0];

    for (int d = 1; d <= maxD; d++) {
        bool possible = isCompatible(inp, n, d, m);
        if (possible) {
            ans = MAX(ans, d);
        }
    }

    return ans;
}

int main()
{
    int m = 3;
    int inp[] = {1,2,8,4,9};
    int n = sizeof(inp)/sizeof(inp[0]);

    printf("The largest minimum distance is %d\r\n", getMinimumDistance(inp, n, m));
}