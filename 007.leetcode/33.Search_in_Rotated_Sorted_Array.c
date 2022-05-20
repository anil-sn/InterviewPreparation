#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


int findMin(int nums[], int len, int key) {
    int start = 0;
    int end = len - 1;

    while (start <= end)  {

        int mid = start + (end - start) / 2;

        if (nums[mid] == key) {
            return mid;
        } else if (nums[mid] >= nums[start]) {
            if (key >= nums[start] && key < nums[mid]) {
                end = mid - 1;
            } else {
                start = mid + 1;
            }
        } else {
            if (key <= nums[end] && key > nums[mid]) {
                start = mid + 1;
            } else {
                end = mid - 1;
            }
        }
    }

    return -1;
}

int main (void) {
    int data[] = {6, 7, 8, 9, 10, 1, 2, 3, 4, 5};
    int len = sizeof(data) / sizeof(data[0]);
    int key = 9;
    int index = 0;

    //int index = findMin(data, len, key);
    //printf("The Minimum: %d is found at %d\r\n", key, index);

    int data2[] = {5, 1, 3};
    len = sizeof(data2) / sizeof(data2[0]);
    key = 3;
    index = findMin(data2, len, key);
    printf("The Minimum: %d is found at %d\r\n", key, index);
}