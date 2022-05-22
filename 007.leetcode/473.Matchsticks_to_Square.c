#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


int
cmp (const void *x, const void *y)
{
    int *a = (int *)x;
    int *b = (int *)y;
    return *a > *b;
}

bool
dfs (int sidesLength[4], int matchsticksSize, int matchsticks[matchsticksSize], int index, const int target)
{
    if (index == matchsticksSize) {
        return  sidesLength[0] == sidesLength[1] &&
                sidesLength[1] == sidesLength[2] &&
                sidesLength[2] == sidesLength[3];
    }

    for (int i = 0; i < 4; ++i) {
        if (sidesLength[i] + matchsticks[index] > target) {
            continue;
        }

        int j = i;

        while (--j >= 0) {
            if (sidesLength[i] == sidesLength[j])  {
                break;
            }
        }

        if (j != -1) {
            continue;
        }

        sidesLength[i] += matchsticks[index];

        if (dfs (sidesLength, matchsticksSize, matchsticks, index + 1, target)) {
            return true;
        }

        sidesLength[i] -= matchsticks[index];
    }

    return false;
}

bool
makesquare (int *matchsticks, int matchsticksSize)
{
    if (matchsticksSize < 4) {
        return false;
    }

    int sum = 0;

    for (int i = 0; i < matchsticksSize; i++) {
        sum += matchsticks[i];
    }

    if (sum % 4 != 0) {
        return false;
    }

    qsort (matchsticks, matchsticksSize, sizeof (matchsticks[0]), cmp);
    int sidesLength[4] = {0};
    return dfs (sidesLength, matchsticksSize, matchsticks, 0, sum / 4);
}