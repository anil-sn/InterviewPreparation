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
    Count of elements that are binary searchable in the given array
    Given an array arr[] consisting of N integers,
    the task is to find the maximum count of integers that are binary
    searchable in the given array.

    Examples:

    Input: arr[] = {1, 3, 2}
    Output: 2
    Explanation: arr[0], arr[1] can be found.

    Input: arr[] = {3, 2, 1, 10, 23, 22, 21}
    Output: 3
    Explanation: arr[1], arr[3], arr[5] can be found using binary search irrespective
    of whether the array is sorted or not.


    Approach: The given problem can be solved by searching for each element separately
    in the array using the Binary Search approach and increment the count of those
    integers that exist in the array. Follow the below steps to solve the problem:

    Make a variable count = 0, that will store the count of elements that are binary searchable.
    For each element perform the binary search in the range [0, N) as:
    Initialize the variable l as 0 and r as N-1 and perform the binary search for arr[i].
    For each iteration of the while loop till l is less than equal to r, calculate the mid-value denoted by (l + r)/2.
    If arr[mid] equals arr[i] then increment count by 1.
    If arr[mid] is less than arr[i], then change l as mid + 1.
    Otherwise, change r as mid – 1.
    The final answer will be stored in the variable count.
*/

// Function to find the total count of
// elements that are binary searchable
int totalBinarySearchable(int arr[], int N)
{
    // Stores the count of element that
    // are binary searchable
    int count = 0;

    // For each element check if it can
    // be found by doing a binary search
    for (int i = 0; i < N; i++) {

        // Binary search range
        int l = 0, r = N - 1;

        // Do a binary Search
        while (l <= r) {
            int mid = (l + r) / 2;

            // Array element found
            if (arr[mid] == arr[i]) {
                count++;
                break;
            }
            if (arr[mid] < arr[i]) {
                l = mid + 1;
            }
            else {
                r = mid - 1;
            }
        }
    }

    // Return the total count
    return count;
}

// Driver Code
int main()
{
    int arr[] = { 3, 2, 1, 10, 23, 22, 21 };
    int n = sizeof(arr)/sizeof(arr[0]);

    printf("The total number of binary searchable elements are : %d\r\n", totalBinarySearchable(arr, n));

    return 0;
}