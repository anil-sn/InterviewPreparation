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
    Find the Majority Element that occurs more than N/2 times
        Problem Statement: Given an array of N integers, write a program to return an
        element that occurs more than N/2 times in the given array. You may consider
        that such an element always exists in the array.

    Example 1:
        Input Format: N = 3, nums[] = {3,2,3}
        Result: 3
        Explanation: When we just count the occurrences of each number
        and compare with half of the size of the array, you will get 3
        for the above solution.

    Example 2:
        Input Format:  N = 7, nums[] = {2,2,1,1,1,2,2}
        Result: 2
        Explanation: After counting the number of times each
        element appears and comparing it with half of array size, we get 2 as
        result.

    Example 3:
        Input Format:  N = 10, nums[] = {4,4,2,4,3,4,4,3,2,4}
        Result: 4
*/

/* Moore’s Voting Algorithm */
int majorityElement(int nums[], int n) {
    int count = 0;
    int candidate = 0;

    for (int i = 0; i < n; i++) {
        if (count == 0) {
            candidate = nums[i];
        }
        if(nums[i] == candidate) {
            count += 1;
        } else {
            count -= 1;
        }
    }

    return candidate;
}

int main ()
{
    int list[] = {4,4,2,4,3,4,4,3,2,4};
    int count = 10;

    printf("Majority number is %d \r\n", majorityElement(list, count));
}