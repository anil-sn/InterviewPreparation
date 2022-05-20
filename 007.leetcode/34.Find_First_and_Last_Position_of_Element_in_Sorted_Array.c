#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int findBound(int nums[], int len, int key, bool isFirst) {
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

int main (void) {
    int data[] = {1, 2, 3, 4, 5, 6, 6, 7, 8, 9, 10};
    int len = sizeof(data) / sizeof(data[0]);

    int firstOccurrence = findBound(data, len, 6, true);

    if (firstOccurrence == -1) {
        printf("{-1, -1} \r\n");
    }

    int lastOccurrence = findBound(data, len, 6, false);
    printf("{%d, %d} \r\n", firstOccurrence, lastOccurrence);
}
