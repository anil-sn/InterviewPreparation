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
    Given an array of non-negative integers nums, you are initially positioned at the first index of the array.
    Each element in the array represents your maximum jump length at that position.
    Your goal is to reach the last index in the minimum number of jumps.
    You can assume that you can always reach the last index.

    Example 1:
        Input: nums = [2,3,1,1,4]
        Output: 2
        Explanation: The minimum number of jumps to reach the last index is 2. Jump 1 step from index 0 to 1, then 3 steps to the last index.
    Example 2:
        Input: nums = [2,3,0,1,4]
        Output: 2
*/

int
jump (int *nums, int numsSize)
{
    if (numsSize < 2) {
        return 0;
    }

    if (nums[0] >= numsSize - 1) {
        return 1;
    }

    int count = 0;
    int max = 0;
    int end = 0;

    for (int i = 0; i < numsSize - 1; i++) {
        max = nums[i] + i > max ? nums[i] + i : max;

        if (i == end) {
            end = max;
            count++;
        }
    }

    return count;
}

/*
The approach is not much different with the main stream solution, but the way I do it is recursion. In this case, you write shorter code and save a bit more time.
It was actually 17 lines, but to give an attractive title I compressed it to 10 line :)
*/

int
jump (int *n, int ns)
{
    if (ns <= 1) {
        return 0;
    }

    int r = n[0];

    if (r >= ns - 1) {
        return 1;
    }

    int max = r + n[r];
    int maxid = r;

    for (int i = 1; i <= r; i++)
        if (n[i] + i > max)
        { max = n[i] + i, maxid = i; }

    return jump (&n[maxid], ns - maxid) + 1;
}