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
    The Idea is Simple , the array is called arithmetic if it consists of at least 3 elements.
    So, firstly if arr_size is < 3 , we will return false.
    The second requirement of the question is, that the difference b/w any two consecutive elements should be same.
    so running a loop from i = 0 to numsSize - 2 as we need at least 2 consecutive elements.
    now we will store the difference of first 2 elements in a diff variable.
    Again, we will traverse a loop to find how many consecutive pairs are with same diff.
    running a inner loop from i + 2 as the outer for loop has calculcated the diff of first 2 elements for us.
    now if the difference of arr[j] - arr[j-1] ,current and previous element is same , this means we find a consecutive pair, with same diff, therefore we will increase the count.
    If the diff is not same then we will break the loop, as we need consecutive elements diff to be same.
    Return the Count.
*/

int
numberOfArithmeticSlices (int *nums, int numsSize)
{
    // if nums size is less than 3 return false
    if (numsSize < 3) {
        return 0;
    }

    int cnt = 0, diff;

    for (int i = 0; i < numsSize - 2; ++i) {
        // storing diff of first 2 elements
        diff = nums[i + 1] - nums[i];

        // checking for consecutive elements with same difference.
        for (int j = i + 2; j < numsSize; ++j) {
            // if we find the same diff of next 2 elements
            // this means we  find consecutive elements
            // increase the Count
            if (nums[j] - nums[j - 1] == diff) {
                ++cnt;
            } else
                // break as we need to cnt for consecutive diff elements
            {
                break;
            }
        }
    }

    // return cnt
    return cnt;
}

