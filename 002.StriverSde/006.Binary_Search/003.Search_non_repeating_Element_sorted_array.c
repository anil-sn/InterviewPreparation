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
    Search Single Element in a sorted array
    Problem Statement: Given a sorted array of N integers, where every element except one appears
    exactly twice and one element appears only once. Search Single Element in a sorted array.

    Example 1:
        Input: N = 9, array[] = {1,1,2,3,3,4,4,8,8}
        Output: 2
        Explanation: Every element in this sorted array except 2 appears twice, therefore the answer returned will be 2.

    Example 2:
        Input: N = 7, array[] = {11,22,22,34,34,57,57}
        Output: 11
        Explanation: Every element in this sorted array except 11 appears twice, therefore the answer returned will be 11.
*/

int findSingleElementXOR(int nums[], int n)
{
    int elem = 0;
    for (int i = 0; i < n; i++) {
        elem = elem ^ nums[i];
    }

    return elem;
}

int findSingleElementBinarySearch(int nums[], int n)
{
    int low = 0;
    int high = n - 2;
    int mid = 0;

    while (low <= high) {

        mid = (low + high) / 2;
        if (mid % 2 == 0) {

            //Checking whether we are in right half
            if (nums[mid] != nums[mid + 1])
                high = mid - 1; //Shrinking the right half
            else
                low = mid + 1; //Shrinking the left half
        } else {

            //Checking whether we are in right half
            if (nums[mid] == nums[mid + 1])
                high = mid - 1; //Shrinking the right half
            else
                low = mid + 1; //Shrinking the left half
        }
    }

    return nums[low];
}

int main() {
    int v[] = {1,1,2,3,3,4,4,8,8};
    int n = sizeof(v)/sizeof(v[0]);

    int elem = findSingleElementXOR(v, n);

    printf("The single occurring element is %d \r\n", elem);
    return 0;
}