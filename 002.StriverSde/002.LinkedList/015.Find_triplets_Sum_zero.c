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
    3 Sum : Find triplets that add up to a zero
    Problem Statement: Given an array of N integers, your task is to find unique triplets that add up
    to give sum zero. In short, you need to return an array of all the unique
    triplets [arr[a], arr[b], arr[c]] such that i!=j, j!=k, k!=i, and their sum is equal to zero.

    Examples:
        Example 1:
            Input: nums = [-1,0,1,2,-1,-4]
            Output: [[-1,-1,2],[-1,0,1]]
            Explanation: Out of all possible unique triplets possible,
            [-1,-1,2] and [-1,0,1] satisfy the condition of summing up to zero with i!=j!=k

        Example 2:
            Input: nums=[-1,0,1,0]
            Output: Output: [[-1,0,1],[-1,1,0]]
            Explanation: Out of all possible unique triplets possible,
            [-1,0,1] and [-1,1,0] satisfy the condition of summing up to zero with i!=j!=k
*/

int cmp (const void *a, const void *b)
{
    int x = *(int *)a;
    int y = *(int *)b;

    return (x-y);
}

void threeSum(int num[], int len)
{
    qsort(num, len, sizeof(int), cmp);

    // moves for a
    for (int i = 0; i < (len-2); i++) {

        /* Skip Same value integers as we have considered it already for 3 SUM */
        if (i > 0 && num[i] == num[i-1]) {
            continue;
        }

        int lo = i+1;
        int hi = (len -1);
        int sum = 0 - num[i];

        while (lo < hi) {
            if (num[lo] + num[hi] == sum) {

                printf("array[%d]=%2d, array[%d]=%2d, array[%d]=%2d \r\n", i, num[i], lo, num[lo], hi, num[hi]);

                /* Skip Same value integers as we have considered it already for 3 SUM */
                while (lo < hi && num[lo] == num[lo+1]) {
                    lo++;
                }

                /* Skip Same value integers as we have considered it already for 3 SUM */
                while (lo < hi && num[hi] == num[hi-1]) {
                    hi--;
                }

                lo++;
                hi--;
            }
            else if (num[lo] + num[hi] < sum) {
                lo++;
            }
            else {
                hi--;
            }
        }

    }

    return;
}

int main() {
    int arr[] = {-1,0,1,2,-1,-4};
    int len = sizeof(arr)/sizeof(arr[0]);

    threeSum(arr, len);
    return 0;
}