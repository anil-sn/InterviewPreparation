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
    Find the duplicate in an array of N+1 integers
    Problem Statement: Given an array of N + 1 size, where each element is between 1 and N.
    Assuming there is only one duplicate number, your task is to find the duplicate number.

    Examples:
        Example 1:
            Input: arr=[1,3,4,2,2]
            Output: 2
            Explanation: Since 2 is the duplicate number the answer will be 2.

        Example 2:
            Input: [3,1,3,4,2]
            Output:3
            Explanation: Since 3 is the duplicate number the answer will be 3.
*/

int findDuplicate(int nums[])
{
    int slow = nums[0];
    int fast = nums[0];
    do {
        slow = nums[slow];
        fast = nums[nums[fast]];
    } while (slow != fast);

    fast = nums[0];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }

    return slow;
}

int main( ) {
    int arr[] = {1,3,4,2,3};
    printf("The duplicate element is %d \r\n", findDuplicate(arr));
}