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
    Trapping Rainwater
    Problem Statement: Given an array of non-negative integers representation elevation of ground. Your task is to find the water that can be trapped after raining.

    Examples:

    Example 1:

    Input: height= [0,1,0,2,1,0,1,3,2,1,2,1]

    Output: 6

    Explanation: As seen from the diagram 1+1+2+1+1=6 unit of water can be trapped
          ▲
          │
        4 │
          │                                   ┌────┐
          │                                   │    │
        3 │                                   │    │
          │              ┌─────┐xxxxxxxxxxxxxx│    ├────┐xxxx┌────┐
          │              │     │xxxxxxxxxxxxxx│    │    │xxxx│    │
        2 │              │     │xxxxxxxxxxxxxx│    │    │xxxx│    │
          │    ┌────┐xxxx│     ├────┐xxxx┌────┤    │    ├────┤    ├────┐
          │    │    │xxxx│     │    │xxxx│    │    │    │    │    │    │
        1 │    │    │xxxx│     │    │xxxx│    │    │    │    │    │    │
          └────┴────┴────┴─────┴────┴────┴────┴────┴────┴────┴────┴────┴───►
             0    1    0    2    1    0    1    3    2    1    2    1

    Example 2:
    Input:  [4,2,0,3,2,5]
    Output:9
*/

#define MAX(a_, b_) (a_ > b_ ? a_ : b_)
#define MIN(a_, b_) (a_ < b_ ? a_ : b_)

int trap(int arr[], int n) {

    int waterTrapped = 0;
    for (int i = 0; i < n; i++) {

        int j = i;
        int leftMax = 0;
        int rightMax = 0;
        while (j >= 0) {
            leftMax = MAX(leftMax, arr[j]);
            j--;
        }

        j = i;
        while (j < n) {
            rightMax = MAX(rightMax, arr[j]);
            j++;
        }

        waterTrapped += (MIN(leftMax, rightMax) - arr[i]);
    }

    return waterTrapped;
}

int trapPreCompute(int arr[], int n)
{
    int prefix[n];
    int suffix[n];
    int waterTrapped = 0;

    prefix[0] = arr[0];
    for (int i = 1; i < n; i++) {
        prefix[i] = MAX(prefix[i - 1], arr[i]);
    }

    suffix[n - 1] = arr[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        suffix[i] = MAX(suffix[i + 1], arr[i]);
    }

    for (int i = 0; i < n; i++) {
        waterTrapped += MIN(prefix[i], suffix[i]) - arr[i];
    }

    return waterTrapped;
}

int trapTwoPointer(int height[], int n)
{
    int left = 0;
    int right = n - 1;

    int res = 0;
    int maxLeft = 0;
    int maxRight = 0;

    while (left <= right) {

        if (height[left] <= height[right]) {

            if (height[left] >= maxLeft) {
                maxLeft = height[left];
            } else {
                res += maxLeft - height[left];
            }

            left++;
        } else {

            if (height[right] >= maxRight) {
                maxRight = height[right];
            } else {
                res += maxRight - height[right];
            }

            right--;
        }
    }

    return res;
}

int main()
{
    int arr1[] = {0,1,0,2,1,0,1,3,2,1,2,1};
    int len1 = sizeof(arr1)/sizeof(arr1[0]);

    int arr2[] = {4,2,0,3,2,5};
    int len2 = sizeof(arr2)/sizeof(arr2[0]);

    printf("[Brute Force] The amount of water trapped is %d \r\n", trap(arr1,len1));
    printf("[Brute Force] The amount of water trapped is %d \r\n\r\n", trap(arr2,len2));

    printf("[PreCompute] The amount of water trapped is %d \r\n", trapPreCompute(arr1,len1));
    printf("[PreCompute] The amount of water trapped is %d \r\n\r\n", trapPreCompute(arr2,len2));

    printf("[TwoPointer] The amount of water trapped is %d \r\n", trapTwoPointer(arr1,len1));
    printf("[TwoPointer] The amount of water trapped is %d \r\n\r\n", trapTwoPointer(arr2,len2));

    return;
}