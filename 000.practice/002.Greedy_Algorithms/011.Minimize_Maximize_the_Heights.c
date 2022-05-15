#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Swap two integers
void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}
/*
    Given an array arr[] denoting heights of N towers and a positive integer K, you have to modify the height of each tower either by increasing or decreasing them by K only once.
    Find out what could be the possible minimum difference of the height of shortest and longest towers after you have modified each tower.
    Note: Assume that height of the tower can be negative.
    A slight modification of the problem can be found here.

    Examples:

        Input  : arr[] = {1, 15, 10}, k = 6
        Output :  Maximum difference is 5.
        Explanation : We change 1 to 7, 15 to
        9 and 10 to 4. Maximum difference is 5
        (between 4 and 9). We can't get a lower
        difference.

        Input : arr[] = {1, 5, 15, 10}
                k = 3
        Output : Maximum difference is 8
        arr[] = {4, 8, 12, 7}

        Input : arr[] = {4, 6}
                k = 10
        Output : Maximum difference is 2
        arr[] = {14, 16} OR {-6, -4}

        Input : arr[] = {6, 10}
                k = 3
        Output : Maximum difference is 2
        arr[] = {9, 7}

        Input : arr[] = {1, 10, 14, 14, 14, 15}
                k = 6
        Output: Maximum difference is 5
        arr[] = {7, 4, 8, 8, 8, 9}

        Input : arr[] = {1, 2, 3}
                k = 2
        Output: Maximum difference is 2
        arr[] = {3, 4, 5}

    First, we try to sort the array and make each height of the tower maximum.
    We do this by decreasing the height of all the towers towards the right by k
    and increasing all the height of the towers towards the left (by k).

    It is also possible that the tower you are trying to increase the height doesn’t
    have the maximum height. Therefore we only need to check whether it has the maximum
    height or not by comparing it with the last element towards the right side which is
    a[n]-k. Since the array is sorted if the tower’s height is greater than the a[n]-k
    then it’s the tallest tower available. Similar reasoning can also be applied for
    finding the shortest tower.

    Note:- We need not consider where a[i]<k because the height of the tower can’t be
    negative so we have to neglect that case.
*/

int getMinDiff(int arr[], int n, int k)
{
    int i, j,temp;
    for (i = 0; i < n-1; i++) {
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    // Maximum possible height difference
    int ans = arr[n - 1] - arr[0];

    int tempmin, tempmax;
    tempmin = arr[0];
    tempmax = arr[n - 1];

    for (int i = 1; i < n; i++) {
        // Minimum element when we add k to whole array
        tempmin= min(arr[0] + k, arr[i] - k);

        // Maximum element when we subtract k from whole array
        tempmax = max(arr[i - 1] + k, arr[n - 1] - k);

        ans = min(ans, tempmax - tempmin);
    }
    return ans;
}

int main ()
{
    int k = 5;
    int n = 10;
    int arr[] = {2, 6, 3, 4, 7, 2, 10, 3, 2, 1};

    for (int i=0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\r\n");
    printf("%d\r\n", getMinDiff(arr, n, k));

    return 0;
}