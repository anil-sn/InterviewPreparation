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
Sqrt (int x)
{
    if (x < 2) {
        return x;
    }

    long num;
    int pivot, left = 2, right = x / 2;

    while (left <= right) {
        pivot = left + (right - left) / 2;
        num = (long)pivot * pivot;

        if (num > x) {
            right = pivot - 1;
        } else if (num < x) {
            left = pivot + 1;
        } else {
            return pivot;
        }
    }

    return right;
}

int
main (void)
{
    int x = 4;
    int result = 0;
    result = Sqrt (x);
    printf ("The square root of %d is : %d \r\n", x, result);
}