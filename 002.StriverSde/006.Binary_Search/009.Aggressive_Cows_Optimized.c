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
    int cntCows = 1;
    int lastPlacedCow = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] - lastPlacedCow >= minDist) {
            cntCows++;
            lastPlacedCow = a[i];
        }
    }

    if (cntCows >= cows) {
        return true;
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

    int mid = 0;
    int low = 1;
    int high = inp[n-1] - inp[0];

    while (low <= high) {
        mid = (low + high) >> 1;
        bool possible = isCompatible(inp, n, mid, m);
        if (possible) {
            ans = MAX(ans, mid);
            low = mid + 1;
        } else {
            high = mid - 1;
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