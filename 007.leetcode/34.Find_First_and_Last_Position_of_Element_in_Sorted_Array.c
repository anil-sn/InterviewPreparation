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
    The problem can be simply broken down as two binary searches for the begining and end of the range, respectively:

    First let's find the left boundary of the range. We initialize the range to [i=0, j=n-1]. In each step, calculate the middle element [mid = (i+j)/2]. Now according to the relative value of A[mid] to target, there are three possibilities:

    If A[mid] < target, then the range must begins on the right of mid (hence i = mid+1 for the next iteration)
    If A[mid] > target, it means the range must begins on the left of mid (j = mid-1)
    If A[mid] = target, then the range must begins on the left of or at mid (j= mid)
    Since we would move the search range to the same side for case 2 and 3, we might as well merge them as one single case so that less code is needed:

    2*. If A[mid] >= target, j = mid;

    Surprisingly, 1 and 2* are the only logic you need to put in loop while (i < j). When the while loop terminates, the value of i/j is where the start of the range is. Why?

    No matter what the sequence originally is, as we narrow down the search range, eventually we will be at a situation where there are only two elements in the search range. Suppose our target is 5, then we have only 7 possible cases:

    case 1: [5 7] (A[i] = target < A[j])
    case 2: [5 3] (A[i] = target > A[j])
    case 3: [5 5] (A[i] = target = A[j])
    case 4: [3 5] (A[j] = target > A[i])
    case 5: [3 7] (A[i] < target < A[j])
    case 6: [3 4] (A[i] < A[j] < target)
    case 7: [6 7] (target < A[i] < A[j])
    For case 1, 2 and 3, if we follow the above rule, since mid = i => A[mid] = target in these cases, then we would set j = mid. Now the loop terminates and i and j both point to the first 5.

    For case 4, since A[mid] < target, then set i = mid+1. The loop terminates and both i and j point to 5.

    For all other cases, by the time the loop terminates, A[i] is not equal to 5. So we can easily know 5 is not in the sequence if the comparison fails.

    In conclusion, when the loop terminates, if A[i]==target, then i is the left boundary of the range; otherwise, just return -1;

    For the right of the range, we can use a similar idea. Again we can come up with several rules:

    If A[mid] > target, then the range must begins on the left of mid (j = mid-1)
    If A[mid] < target, then the range must begins on the right of mid (hence i = mid+1 for the next iteration)
    If A[mid] = target, then the range must begins on the right of or at mid (i= mid)
    Again, we can merge condition 2 and 3 into:

    2* If A[mid] <= target, then i = mid;
    However, the terminate condition on longer works this time. Consider the following case:

    [5 7], target = 5
    Now A[mid] = 5, then according to rule 2, we set i = mid. This practically does nothing because i is already equal to mid. As a result, the search range is not moved at all!

    The solution is by using a small trick: instead of calculating mid as mid = (i+j)/2, we now do:

    mid = (i+j)/2+1
    Why does this trick work? When we use mid = (i+j)/2, the mid is rounded to the lowest integer. In other words, mid is always biased towards the left. This means we could have i == mid when j - i == mid, but we NEVER have j == mid. So in order to keep the search range moving, you must make sure the new i is set to something different than mid, otherwise we are at the risk that i gets stuck. But for the new j, it is okay if we set it to mid, since it was not equal to mid anyways. Our two rules in search of the left boundary happen to satisfy these requirements, so it works perfectly in that situation. Similarly, when we search for the right boundary, we must make sure i won't get stuck when we set the new i to i = mid. The easiest way to achieve this is by making mid biased to the right, i.e. mid = (i+j)/2+1.

    All this reasoning boils down to the following simple code:
*/
int *
searchRange (int *A, int numsSize, int target, int *returnSize)
{
    int *ret = (int *)calloc (2, sizeof (int));
    ret[0] = -1, ret[1] = -1;
    *returnSize = 2;

    if ((NULL == A) || (0 == numsSize)) {
        return ret;
    }

    int i = 0, j = numsSize - 1;

    // Search for the left one
    while (i < j) {
        int mid = (i + j) / 2;

        if (A[mid] < target) {
            i = mid + 1;
        } else {
            j = mid;
        }
    }

    if (A[i] != target) {
        return ret;
    } else {
        ret[0] = i;
    }

    // Search for the right one
    j = numsSize - 1; // We don't have to set i to 0 the second time.

    while (i < j) {
        int mid = (i + j) / 2 + 1;  // Make mid biased to the right

        if (A[mid] > target) {
            j = mid - 1;
        } else {
            i = mid;    // So that this won't make the search range stuck.
        }
    }

    ret[1] = j;
    return ret;
}

int
findBound (int nums[], int len, int key, bool isFirst)
{
    int begin = 0, end = len - 1;

    while (begin <= end) {
        int mid = (begin + end) / 2;

        if (nums[mid] == key) {
            if (isFirst) {
                // This means we found our lower bound.
                if (mid == begin || nums[mid - 1] != key) {
                    return mid;
                }

                // Search on the left side for the bound.
                end = mid - 1;
            } else {
                // This means we found our upper bound.
                if (mid == end || nums[mid + 1] != key) {
                    return mid;
                }

                // Search on the right side for the bound.
                begin = mid + 1;
            }
        } else if (nums[mid] > key) {
            end = mid - 1;
        } else {
            begin = mid + 1;
        }
    }

    return -1;
}

int
main (void)
{
    int data[] = {1, 2, 3, 4, 5, 6, 6, 7, 8, 9, 10};
    int len = sizeof (data) / sizeof (data[0]);
    int firstOccurrence = findBound (data, len, 6, true);

    if (firstOccurrence == -1) {
        printf ("{-1, -1} \r\n");
    }

    int lastOccurrence = findBound (data, len, 6, false);
    printf ("{%d, %d} \r\n", firstOccurrence, lastOccurrence);
}


int *
searchRange (int *nums, int numsSize, int target, int *returnSize)
{
    int *start = NULL, *end = NULL;
    *returnSize = 2;
    start = nums, end = nums + numsSize - 1;
    int *result = (int *)malloc (sizeof (int) * (*returnSize));
    result[0] = -1, result[1] = -1;

    while (start <= end) {
        if (*start != target) {
            start++;
        } else {
            result[0] = start - nums;
        }

        if (*end != target) {
            end--;
        } else {
            result[1] = end - nums;
        }

        if (result[0] != -1 && result[1] != -1) {
            break;
        }
    }

    return result;
}

int *
searchRange (int *nums, int numsSize, int target, int *returnSize)
{
    int start, mid, end;
    int front_gap = 0;
    int end_gap = 0;
    int found = 0;
    int *result;
    start = 0;
    end = numsSize - 1;
    result = calloc (2, sizeof (int));
    *returnSize = 2;

    while (!found) {
        if (start > end) {
            result[0] = -1;
            result[1] = -1;
            return result;
        }

        mid = (start + end) / 2;

        if (nums[mid] == target) {
            found = 1;
            break;
        } else if (nums[mid] > target) {
            end = mid - 1;
        } else {
            start = mid + 1;
        }
    }

    while (((mid + front_gap) >= 0) && nums[mid + front_gap] == target || ((mid + end_gap) <= numsSize - 1) &&  nums[mid + end_gap] == target) {
        if ((mid + front_gap >= 0) && nums[mid + front_gap] == target) {
            if ((mid + front_gap >= 0)) {
                front_gap--;
            }
        }

        if ((mid + end_gap <= (numsSize - 1)) && nums[mid + end_gap] == target) {
            if (mid + end_gap <= (numsSize - 1)) {
                end_gap++;
            }
        }
    }

    result[0] = front_gap + 1 + mid;
    result[1] = end_gap - 1 + mid;
    return result;
}