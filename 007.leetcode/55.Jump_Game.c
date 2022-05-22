#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX(_a,_b) (_a > _b ? _a : _b)
bool
canJump (int *nums, int numsSize)
{
    int i = 0;

    for (int reach = 0; i < numsSize && i <= reach; ++i) {
        reach = MAX (i + nums[i], reach);
    }

    return i == numsSize;
}