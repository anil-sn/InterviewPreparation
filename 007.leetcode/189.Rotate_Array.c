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
    The basic idea is that, for example, nums = [1,2,3,4,5,6,7] and k = 3, first we reverse [1,2,3,4], it becomes[4,3,2,1]; then we reverse[5,6,7], it becomes[7,6,5], finally we reverse the array as a whole, it becomes[4,3,2,1,7,6,5] ---> [5,6,7,1,2,3,4].
*/
void
rotate (int nums[], int n, int k)
{
    if (nums == NULL || n < 2) {
        return;
    }

    k = k % n;
    reverse (nums, 0, n - k - 1);
    reverse (nums, n - k, n - 1);
    reverse (nums, 0, n - 1);
}

void
reverse (int nums[], int i, int j)
{
    int tmp = 0;

    while (i < j) {
        tmp = nums[i];
        nums[i] = nums[j];
        nums[j] = tmp;
        i++;
        j--;
    }
}