
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
searchInsert (int *nums, int numsSize, int target)
{
    int lo = 0;
    int hi = numsSize - 1;
    int mid = 0;

    while (lo <= hi) {
        mid = (lo) + (hi - lo) / 2;

        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    return lo;
}