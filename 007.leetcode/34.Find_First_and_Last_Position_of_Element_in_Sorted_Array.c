#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

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