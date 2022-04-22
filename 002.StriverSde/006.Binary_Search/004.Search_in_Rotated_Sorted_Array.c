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
    Search Element in a Rotated Sorted Array

    Problem Statement: There is an integer array nums sorted in ascending order (with distinct values).
        Given the array nums after the possible rotation and an integer target, return the index of target
        if it is in nums, or -1 if it is not in nums. We need to search a given element in a rotated sorted
        array.

    Example 1:
    Input: nums = [4,5,6,7,0,1,2,3], target = 0
    Output: 7
    Explanation: Here, the target is 0. We can see that 0 is present in the given rotated sorted array,
    nums. Thus, we get output as 4, which is the index at which 0 is present in the array.

    Example 2:
    Input: nums = [4,5,6,7,0,1,2], target = 3
    Output: -1
    Explanation: Here, the target is 3. Since 3 is not present in the given rotated sorted array. Thus, we get output as -1.
*/

int searchLinear(int nums[], int len, int target)
{
    for(int i=0; i<len; i++) {

        if(nums[i]==target) {
            return i;
        }
    }

    return -1;
}

int searchBinary(int nums[], int len, int target)
{
    int low = 0;
    int high = len - 1;

    while (low <= high) {

        int mid = (low + high) >> 1;

        if (nums[mid] == target) {
            return mid;
        }

        if (nums[low] <= nums[mid]) {

            if (nums[low] <= target && nums[mid] >= target) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        } else {

            if (nums[mid] <= target && target <= nums[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }

    return -1;
}

int main() {
    int nums[] = {4,5,6,7,0,1,2,3};
    int len = sizeof(nums)/sizeof(nums[0]);
    int target = 0;

    printf("The index in which the target is present is %d \r\n", searchLinear(nums, len, target));
    return 0;
}