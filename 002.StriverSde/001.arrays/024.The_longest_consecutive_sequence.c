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
    Longest Consecutive Sequence in an Array
    Problem Statement: You are given an array of ‘N’ integers. You need to find the length of
    the longest sequence which contains the consecutive elements.

    Examples:

        Example 1:
            Input: [100, 200, 1, 3, 2, 4]
            Output: 4
            Explanation: The longest consecutive subsequence is 1, 2, 3, and 4.

        Example 2:
            Input: [3, 8, 5, 7, 6]
            Output: 4
            Explanation: The longest consecutive subsequence is 5, 6, 7, and 8.
*/
int cmp (const void *a, const void *b)
{
    int x = *((int *)a);
    int y = *((int *)b);
    return (x - y);
}

int max(int a, int b)
{
    return (a > b ? a : b);
}

int longestConsecutive(int arr[], int n)
{
    if ((NULL == arr) || (0 == n)) {
        return 0;
    }

    qsort(arr, n, sizeof(int), cmp);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");

    int count = 1;
    int result = INT_MIN;

    for (int i = 0; i < n; i++) {
        if (arr[i+1] == (arr[i] + 1)) {
            count++;
            result = max(count, result);
        } else {
            count = 1;
        }
    }

    return result;
}

int main()
{
    int arr[] = {100,200,1,2,3,4};
    int n = sizeof(arr)/sizeof(arr[0]);

    int lon = longestConsecutive(arr, n);

    printf("The longest consecutive sequence is %d \r\n", lon);

}