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
We have already discussed Overlapping Subproblems and Optimal Substructure properties.
Now, let us discuss the Longest Increasing Subsequence (LIS) problem as an example problem
that can be solved using Dynamic Programming.

The Longest Increasing Subsequence (LIS) problem is to find the length of
the longest subsequence of a given sequence such that all elements of the subsequence
are sorted in increasing order.
For example, the length of LIS for {10, 22, 9, 33, 21, 50, 41, 60, 80} is 6 and LIS is {10, 22, 33, 50, 60, 80}.

Examples:

    Input: arr[] = {3, 10, 2, 1, 20}
    Output: Length of LIS = 3
    The longest increasing subsequence is 3, 10, 20

    Input: arr[] = {3, 2}
    Output: Length of LIS = 1
    The longest increasing subsequences are {3} and {2}

    Input: arr[] = {50, 3, 10, 7, 40, 80}
    Output: Length of LIS = 4
    The longest increasing subsequence is {3, 7, 40, 80}

    Dynamic Programming.
        We can see that there are many subproblems in the above recursive solution which are
        solved again and again. So this problem has Overlapping Substructure property and
        recomputation of same subproblems can be avoided by either using Memoization or Tabulation.

        The simulation of approach will make things clear:

        Input  : arr[] = {3, 10, 2, 11}
        LIS[] = {1, 1, 1, 1} (initially)
        Iteration-wise simulation :

        arr[2] > arr[1] {LIS[2] = max(LIS [2], LIS[1]+1)=2}
        arr[3] < arr[1] {No change}
        arr[3] < arr[2] {No change}
        arr[4] > arr[1] {LIS[4] = max(LIS [4], LIS[1]+1)=2}
        arr[4] > arr[2] {LIS[4] = max(LIS [4], LIS[2]+1)=3}
        arr[4] > arr[3] {LIS[4] = max(LIS [4], LIS[3]+1)=3}
        We can avoid recomputation of subproblems by using tabulation
*/

/* lis() returns the length of the longest
  increasing subsequence in arr[] of size n */
int lis(int arr[], int n)
{
    int lis[n];
    int max = 0;
    lis[0] = INT_MIN;

    /* Compute optimized LIS values in
       bottom up manner */
    for (int i = 1; i < n; i++) {
        lis[i] = 1;
        for (int j = 0; j < i; j++)
            if (arr[i] > arr[j] && lis[i] < lis[j] + 1)
                lis[i] = lis[j] + 1;
    }

    // Return maximum value in lis[]
    printf("{ 10, 22, 9, 33, 21, 50, 41, 60 }\r\n    ");
    for (int i = 1; i < n; i++) {
        printf("%3d ", lis[i]);
        if (lis[i] > max) {
            max = lis[i];
        }
    }
    printf("\r\nSubSequence is : ");

    int temp = max;
    int itor = 1;
    int prev = lis[0];
    while (itor <= n-1) {
        if (prev != lis[itor]) {
            printf("%d ", arr[itor]);
            prev = lis[itor];
        }
       itor++;
    }
    printf("\r\n");

    return max;
}

/* Driver program to test above function */
int main()
{
    int arr[] = { 10, 22, 9, 33, 21, 50, 41, 60 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Length of lis is %d\n", lis(arr, n));

    return 0;
}