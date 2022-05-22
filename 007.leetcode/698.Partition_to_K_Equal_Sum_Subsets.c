#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


/* Solution 1: Back Tracking Slow Process */
bool
canPartition (int start, int len, int a[len], bool seen[len], int k, int sum, int target)
{
    if (k == 1) {
        return true;
    }

    if (sum == target) {
        return canPartition (0, len, a, seen, k - 1, 0, target);
    }

    for (int i = start; i < len; i++) {
        if (false == seen[i]) {
            seen[i] = true;

            if (canPartition (i + 1, len, a, seen, k, sum + a[i], target)) {
                return true;
            }

            seen[i] = false;
        }
    }

    return false;
}

bool
canPartitionKSubsets (int *nums, int numsSize, int k)
{
    bool result = false;
    bool seen[numsSize];
    memset (seen, false, sizeof (seen));

    if (0 == k) {
        return false;
    }

    int sum = 0;

    for (int i = 0; i < numsSize; i++) {
        sum += nums[i];
    }

    if (sum % k != 0)  {
        return false;
    }

    result = canPartition (0, numsSize, nums, seen, k, 0, sum / k);
    return result;
}

/*
    Before diving into solution, Let's first try to understand what Bitmask means. Mask in Bitmask means hiding something. Bitmask is nothing but a binary number that represents something. Let's take an example.

    Consider the set A {1,2,3,4,5}. We can represent any subset of A using a bitmask of length 5, with an assumption that if ith (0<=i<=4) bit is set then it means ith element is present in subset. So the bitmask 01010 represents the subset {2,4}.

    Now the benefit of using bitmask. We can set the ith bit, unset the ith bit, check if ith bit is set in just one step each. Let's say the bitmask, b = 01010.

    Set the ith bit: b(1<<i). Let i=0, so,
    (1<<i) = 00001
    01010 | 00001 = 01011

    So now the subset includes the 0th element also, so the subset is {1,2,4}.

    Unset the ith bit: b & !(1<<i). Let i=1, so,
    (1<<i) = 00010
    !(1<<i) = 11101
    01010 & 11101 = 01000

    Now the subset does not include the 1st element, so the subset is {4}.

    Check if ith bit is set: b&(1<<i), doing this operation, if ith bit is set, we get a non zero integer otherwise, we get zero. Let i = 3
    (1<<i) = 01000
    01010 & 01000 = 01000

    Clearly the result is non-zero, so that means 3rd element is present in the subset.

    Now coming to our problem, we can represent each number and sum in binary.
    We'll use dynamic programming to find whether the array can be partitioned into k subsets of equal sum. For this, we create two arrays of
    size = power set of array elements. why?

    because, we need to consider all sum subsets.
    dp[i] indicates whether array of length i can partitioned into k subsets of equal sum. Using this technique, the last index of this dp array
    will tell whether the whole array can be partitioned into k subsets of equal sum.

    total[i] stores the sum of subset with sum less than equal to target sum (total sum/k why? because we need to split array into k subset).
*/

int
cmp (const void *x, const void *y)
{
    int *a = (int *)x;
    int *b = (int *)y;
    return *a > *b;
}

bool
canPartitionKSubsets (int *nums, int numsSize, int k)
{
    if (nums == NULL || numsSize == 0) {
        return false;
    }

    if (k == 1) {
        return true;
    }

    int bitMaxLen = 1 << numsSize;
    int n = numsSize;
    bool dp[bitMaxLen];
    int total[bitMaxLen];
    memset (dp, false, sizeof (dp));
    memset (total, 0, sizeof (total));
    dp[0] = true;
    int sum = 0;

    for (int i = 0; i < numsSize; i++) {
        sum += nums[i];
    }

    if (sum % k != 0)  {
        return false;
    }

    qsort (nums, numsSize, sizeof (nums[0]), cmp);
    sum = sum / k;

    if (nums[numsSize - 1] > sum) {
        return false;
    }

    // Loop over power set
    for (int i = 0; i < (bitMaxLen); i++) {
        if (dp[i]) {
            // Loop over each element to find subset
            for (int j = 0; j < n; j++) {
                // set the jth bit
                int temp = i | (1 << j);

                if (temp != i) {
                    /* if total sum is less than target store in dp and total array
                       to make sure that only j with condition: nums[j] smaller than ( or equal to) sum- total[i] can be selected and then we continue. The % is very interesting. When nums[j] equals to sum- total[i], nums[j] is added to total[i], now we have finished finding one of k groups. Now we move to find next group, so % can make sure we ignore the sum of existed groups in the condition for next group.
                    */
                    if (nums[j] <= (sum - (total[i] % sum))) {
                        dp[temp] = true;
                        total[temp] = nums[j] + total[i];
                    } else {
                        break;
                    }
                }
            }
        }
    }

    return dp[ (bitMaxLen) - 1];
}