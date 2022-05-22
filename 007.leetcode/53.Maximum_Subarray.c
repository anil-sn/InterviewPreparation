/*
53. Maximum Subarray

Find the contiguous subarray within an array (containing at least one number) which has the largest sum.


For example, given the array [-2,1,-3,4,-1,2,1,-5,4],
the contiguous subarray [4,-1,2,1] has the largest sum = 6.


click to show more practice.

More practice:

If you have figured out the O(n) solution, try coding another solution using the divide and conquer approach, which is more subtle.
*/

// https://leetcode.com/problems/maximum-subarray/discuss/1595097/JAVA-or-Kadane's-Algorithm-or-Explanation-Using-Image
#define MAX(_a,_b) (_a > _b ? _a : _b)

int
maxSubArray (int *nums, int numsSize)
{
    int sum = 0;
    int max = INT_MIN;

    if (1 == numsSize) {
        return nums[0];
    }

    for (int i = 0; i < numsSize; i ++) {
        sum += nums[i];
        max = MAX (sum, max);

        if (sum < 0) {
            sum = 0;
        }
    }

    return max;
}


/*
Difficulty:Easy
Total Accepted:216.1K
Total Submissions:546K


Companies LinkedIn Bloomberg Microsoft
Related Topics Array Dynamic Programming Divide and Conquer
Similar Questions


                    Best Time to Buy and Sell Stock

                    Maximum Product Subarray
*/
