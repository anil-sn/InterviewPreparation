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
    18. 4Sum

    Given an array nums of n integers, return an array of all the unique quadruplets [nums[a], nums[b], nums[c], nums[d]] such that:
    0 <= a, b, c, d < n
    a, b, c, and d are distinct.
    nums[a] + nums[b] + nums[c] + nums[d] == target
    You may return the answer in any order.



    Example 1:

        Input: nums = [1,0,-1,0,-2,2], target = 0
        Output: [[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]
    Example 2:

    Input: nums = [2,2,2,2,2], target = 8
    Output: [[2,2,2,2]]


    Constraints:

        1 <= nums.length <= 200
        -109 <= nums[i] <= 109
        -109 <= target <= 109
        Accepted
        585,231
        Submissions

*/

int
comp (const void *p, const void *q)
{
    return (* (int *) p - * (int *) q);
}

int **
fourSum (int *nums, int numsSize, int target, int *returnSize)
{
    if (nums == NULL) {
        return 0;
    }

    if (numsSize < 4) {
        return 0;
    }

    int **Ans = (int **)malloc (200 * sizeof (int *));
    qsort (nums, numsSize, sizeof (int), comp);
    int a, b, c, d, count = 0;;

    for (a = 0; a < numsSize - 3; a++) {
        // check [a,x,x,x] mininum
        if (nums[a] + nums[a + 1] + nums[a + 2] + nums[a + 3] > target)
        { break; }

        // check [a,x,x,x] maxinum
        if (nums[a] + nums[numsSize - 3] + nums[numsSize - 2] + nums[numsSize - 1] < target)
        { continue; }

        for (b = a + 1; b < numsSize - 2; b++) {
            // check [a,b,x,x] mininu
            if (nums[a] + nums[b] + nums[b + 1] + nums[b + 2] > target)
            { break; }

            // check [a,b,x,x] maxinum
            if (nums[a] + nums[b] + nums[numsSize - 2] + nums[numsSize - 1] < target)
            { continue; }

            for (c = b + 1, d = (numsSize - 1); c < d;) {
                if (nums[a] + nums[b] + nums[c] + nums[d] > target) {
                    d--;
                } else if (nums[a] + nums[b] + nums[c] + nums[d] < target) {
                    c++;
                } else {
                    Ans[count] = (int *)malloc (4 * sizeof (int));
                    Ans[count][0] = nums[a];
                    Ans[count][1] = nums[b];
                    Ans[count][2] = nums[c];
                    Ans[count][3] = nums[d];

                    while (c < d && Ans[count][2] == nums[c]) {
                        c++;
                    }

                    while (c < d && Ans[count][3] == nums[d]) {
                        d--;
                    }

                    count++;

                    if (count % 200 == 0)
                    { Ans = (int **)realloc (Ans, sizeof (int *) * (count + 200)); }
                }
            }

            while (b < numsSize - 1 && nums[b + 1] == nums[b]) {
                b++;
            }
        }

        while (a < numsSize - 1 && nums[a + 1] == nums[a]) {
            a++;
        }
    }

    *returnSize = count;
    return (int **)realloc (Ans, sizeof (int *)*count);
}