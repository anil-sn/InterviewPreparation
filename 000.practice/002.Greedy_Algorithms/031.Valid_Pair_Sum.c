#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    Given an array of size N, find the number of distinct pairs {i, j} (i != j) in the array
    such that the sum of a[i] and a[j] is greater than 0.

    Example 1:
        Input: N = 3, a[] = {3, -2, 1}
        Output: 2
        Explanation: {3, -2}, {3, 1} are two
        possible pairs.
    Example 2:
        Input: N = 4, a[] = {-1, -1, -1, 0}
        Output: 0
        Explanation: There are no possible pairs.

    Approach:

    The idea is to use the concept of sorting and two pointer technique.
    For this problem, sorting is used because for the sum arr[i] + arr[j] > 0 where i, j are some random
    indices in the array, either arr[i] > 0 or arr[j] > 0 or both arr[i] and arr[j] > 0.

    Therefore, once the array is sorted, since we need to find the unique pairs.
    For every ‘i’ such that arr[i] > 0, we need to find the number of j’s such that arr[j] + arr[j] > 0.
    Here, it is easy to find the count of pairs by using two pointer technique because the array is sorted.
    We just need to find the leftmost position of ‘j’ for which the condition holds true.
    This is found using the lower_bound of -arr[i] + 1.

    For example, let the array arr[] = {-4, 4, -5, 5, 3, -2, -3, -1, 2, 1}. This array is sorted.
    Therefore, the array becomes, {-5, -4, -3, -2, -1, 1, 2, 3, 4, 5}. For some random i,
    lets assume arr[i] = 4. Therefore, the index of -3 is found in the array which is 2.
    Now, we can be sure that for all the values between the indices 2 and 8,
    the value of arr[i] + arr[j] > 0.
*/

int cmp (const void *x, const void *y){
    int *a = (int *)x;
    int *b = (int *)y;

    return *a > *b;
}

int lower_bound(int arr[], int n, int val)
{
    int start = 0, end = n;

    /*
        using the Binary search technique , since our
        array is sorted
    */
    while (start < end) {
        int mid = (start + end) >> 1;

        if (val > arr[mid])
            start = mid + 1;
        else
            end = mid;
    }

    // when we dont find the answer return -1
    if (start == n)
        return -1;

    return start;
}


int countValidPairSum(int arr[], int n){

    int dp[n];
    dp[0] = -1;

    qsort(arr, n, sizeof(arr[0]), cmp);

    // Variable to store the count of pairs
    int ans = 0;

    // Loop to iterate through the array
    for (int i = 0; i < n; ++i) {

        // Ignore if the value is negative
        if (arr[i] <= 0)
            continue;

        /*
        minReqVal val is the min value ,which will
        give >=1 after adding with the arr[i]
        */
        int minReqVal = -arr[i] + 1;
        int j = lower_bound(arr, n, minReqVal);

        if (j >= 0)
            ans += i - j;
    }
    return ans;
}

// Driver code
int main()
{
  int arr[] = {-4, 4, -5, 5, 3, -2, -3, -1, 2, 1};
  int n = sizeof(arr) / sizeof(arr[0]);

  // Function call
  printf("Number of valid pair sum is %d\r\n", countValidPairSum(arr,n));

  int arr2[] = {3, -2, 1};
  int m = sizeof(arr2) / sizeof(arr2[0]);

  // Function call
  printf("Number of valid pair sum is %d\r\n", countValidPairSum(arr2,m));

}