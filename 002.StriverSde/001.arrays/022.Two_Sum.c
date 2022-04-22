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
    Two Sum : Check if a pair with given sum exists in Array
    Problem Statement: Given an array of integers nums[] and an integer target,
            return indices of the two numbers such that their sum is equal to the target.
    Note: Assume that there is exactly one solution, and you are not allowed to use the same element twice. Example: If target is equal to 6 and num[1] = 3, then nums[1] + nums[1] = target is not a solution.

    Example 1:
        Input: nums = [2,7,11,15], target = 9
        Output: [0,1]
        Explanation: Because nums[0] + nums[1] == 9,
        which is the required target, we return indexes [0,1]. (0-based indexing)

    Example 2:
        Input Format: nums = [3,2,4,6], target = 6
        Output: [1,2]
        Explanation: Because nums[1] + nums[2] == 6,
        which is the required target, we return indexes [1,2].
*/
void twoSum(int array[], int target, int len) {

    bool isFound = false;

    for (int i = 0; i < len; ++i) {
   	 for (int j = i + 1; j < len; ++j) {
   		if (array[i] + array[j] == target) {
            printf("array[%d]=%d, array[%d]=%d \r\n", i, array[i], j, array[j]);
            isFound = true;
   			break;
   		}
   	}

   	if (isFound)
   		break;
    }

    return;
}

int cmp(const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;
    return x < y ? -1 : x > y ?  1 : 0;
}

void twoSumPointer(int array[], int target, int len) {

    bool isFound = false;
    qsort(array, len, sizeof(int), cmp);

    int low = 0;
    int high = len - 1;

    while (low < high) {
        if ((array[low] + array[high]) < target ) {
            low++;
        } else if ((array[low] + array[high]) > target ) {
            high--;
        } else {
            printf("array[%d]=%d, array[%d]=%d \r\n", low, array[low], high, array[high]);
            break;
        }
    }

    return;
}

int main () {
    int array[] = {2,7,11,15};
    int len = sizeof(array)/sizeof(array[0]);
    int target = 9;

    twoSum(array, target, len);
    twoSumPointer(array, target, len);
}