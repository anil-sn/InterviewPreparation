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
peek (int nums[], int len)
{
    int l = 0, r = len - 1;

    while (l < r) {
        int mid = (l + r) / 2;

        if (nums[mid] > nums[mid + 1]) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    return l;
}

int
search (int nums[], int l, int r)
{
    if (l == r) {
        return l;
    }

    int mid = (l + r) / 2;

    if (nums[mid] > nums[mid + 1]) {
        return search (nums, l, mid);
    }

    return search (nums, mid + 1, r);
}

int
findRecursive (int nums[], int len)
{
    return search (nums, 0, len - 1);
}

int
main (void)
{
    int nums[] = {1, 2, 3, 1};
    int len = sizeof (nums) / sizeof (nums[0]);
    printf ("The peek element is : %d \r\n", peek (nums, len));
    printf ("[Recursive] The peek element is : %d \r\n", findRecursive (nums, len));
    int nums2[] = {2, 1};
    len = sizeof (nums) / sizeof (nums[0]);
    printf ("The peek element is : %d \r\n", peek (nums2, len));
    printf ("[Recursive] The peek element is : %d \r\n", findRecursive (nums2, len));
}