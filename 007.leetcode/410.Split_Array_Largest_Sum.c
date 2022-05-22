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
getTotalSum (int *nums, int numsSize)
{
    int count = 0;

    for (int i = 0; i < numsSize; i++) {
        count += nums[i];
    }

    return count;
}

int
getMaxItem (int *nums, int numsSize)
{
    int count = 0;

    for (int i = 0; i < numsSize; i++) {
        if (nums[i] > count) {
            count = nums[i];
        }
    }

    return count;
}

bool
feasible (int *nums, int numsSize, int currentTotal, int numsSplitsLimits)
{
    size_t total = 0, numsSplits = 1;

    for (int i = 0; i < numsSize; i++) {
        total += nums[i];

        if (total > currentTotal) {
            numsSplits++;
            total = nums[i];

            if (numsSplits > numsSplitsLimits) {
                return false;
            }
        }
    }

    return true;
}

int
splitArray (int *nums, int numsSize, int m)
{
    int left = getMaxItem (nums, numsSize);
    int right = getTotalSum (nums, numsSize);

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (feasible (nums, numsSize, mid, m)) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return left;
}