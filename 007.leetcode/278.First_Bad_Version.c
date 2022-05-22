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

Scenario #1: isBadVersion(mid) => false
 1 2 3 4 5 6 7 8 9
 G G G G G G B B B       G = Good, B = Bad
 |       |       |
left    mid    right

Scenario #2: isBadVersion(mid) => true

 1 2 3 4 5 6 7 8 9
 G G G B B B B B B       G = Good, B = Bad
 |       |       |
left    mid    right

*/

bool
isBadVersion (int index)
{
    if (index >= 7) {
        return true;
    }

    return false;
}

int
firstBadVersion (int n)
{
    int left = 1;
    int right = n;

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (isBadVersion (mid)) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    return left;
}

int
main (void)
{
    int n = 9;
    printf ("The first bad version is : %d \r\n", firstBadVersion (n));
    return 0;
}