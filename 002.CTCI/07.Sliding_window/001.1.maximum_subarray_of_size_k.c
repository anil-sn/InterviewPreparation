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

// Returns maximum sum in a subarray of size k.
int maxSum(int arr[], int n, int k)
{
    // n must be greater
    if (n < k) {
        printf("Invalid\r\n");
        return -1;
    }

    // Compute sum of first window of size k
    int max_sum = 0;
    for (int i = 0; i < k; i++) {
        max_sum += arr[i];
    }

    // Compute sums of remaining windows by removing first element of previous
    // window and adding last element of current window.
    int window_sum = max_sum;
    for (int i = k; i < n; i++) {
        window_sum += arr[i] - arr[i - k];
        max_sum = max(max_sum, window_sum);
    }

    return max_sum;
}

// Driver code
int main()
{
    int arr[] = { 1, 4, 2, 10, 2, 3, 1, 0, 20 };
    int k = 4;
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Maximum sum of sub array with size %d is %d\r\n", k, maxSum(arr, n, k));
    return 0;
}