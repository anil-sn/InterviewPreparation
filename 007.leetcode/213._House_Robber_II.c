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
    This problem is a little tricky at first glance. However, if you have finished the House Robber problem,
    this problem can simply be decomposed into two House Robber problems.
    Suppose there are n houses, since house 0 and n - 1 are now neighbors, we cannot rob them together and thus the solution is now the maximum of

    Rob houses 0 to n - 2;
    Rob houses 1 to n - 1.
    The code is as follows. Some edge cases (n < 2) are handled explicitly.
*/

int
robber (int houses[], int l, int r)
{
    int pre = 0, cur = 0;

    for (int i = l; i <= r; i++) {
        int temp = max (pre + houses[i], cur);
        pre = cur;
        cur = temp;
    }

    return cur;
}

int
rob (int houses[], int n)
{
    if (n < 2) {
        return n;
    }

    return MAX (robber (houses, 0, n - 2), robber (houses, 1, n - 1));
}