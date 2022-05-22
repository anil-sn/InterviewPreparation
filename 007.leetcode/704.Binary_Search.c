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
search (int nums[], int numsSize, int target)
{
    int low = 0;
    int high = numsSize - 1;
    int mid = 0;

    while (low <= high) {
        mid = low + (high - low) / 2;

        if (target == nums[mid]) {
            return mid;
        }

        if (target > nums[mid]) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

int
main (void)
{
    int data[] = {6, 7, 8, 9, 10, 1, 2, 3, 4, 5};
    int len = sizeof (data) / sizeof (data[0]);
    int key = 9;
    int index = search (data, len, key);
    printf ("The Minimum: %d is found at %d\r\n", key, index);
}