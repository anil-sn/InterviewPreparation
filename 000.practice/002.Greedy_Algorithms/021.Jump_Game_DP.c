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
    Minimum number of jumps to reach end

    Given an array of integers where each element represents the max number of steps
    that can be made forward from that element. Write a function to return the minimum
    number of jumps to reach the end of the array (starting from the first element).
    If an element is 0, they cannot move through that element.
    If the end isn’t reachable, return -1.

    Examples:

    Input: arr[] = {1, 3, 5, 8, 9, 2, 6, 7, 6, 8, 9}
    Output: 3 (1-> 3 -> 9 -> 9)

    Explanation: Jump from 1st element to 2nd element as there is only 1 step,
    now there are three options 5, 8 or 9.
    If 8 or 9 is chosen then the end node 9 can be reached. So 3 jumps are made.

    Input:  arr[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    Output: 10
    Explanation: In every step a jump
    is needed so the count of jumps is 10.
    The first element is 1, so can only go to 3. The second element is 3,
    so can make at most 3 steps eg to 5 or 8 or 9.

    Approach:

    1. In this way, make a jumps[] array from left to right such that jumps[i]
       indicate the minimum number of jumps needed to reach arr[i] from arr[0].
    2. To fill the jumps array run a nested loop inner loop counter is j and
       outer loop count is i.
    3. Outer loop from 1 to n-1 and inner loop from 0 to i.
    4. if i is less than j + arr[j] then
       set jumps[i] to minimum of jumps[i] and jumps[j] + 1. initially set jump[i] to INT MAX
    5. Finally, return jumps[n-1].
*/

// Returns minimum number of
// jumps to reach arr[n-1] from arr[0]
int minJumps(int arr[], int n)
{
    // jumps[n-1] will hold the result
    int jumps[n];
    int i, j;

    if (n == 0 || arr[0] == 0)
        return INT_MAX;

    jumps[0] = 0;

    // Find the minimum number of
    // jumps to reach arr[i]
    // from arr[0], and assign this
    // value to jumps[i]
    for (i = 1; i < n; i++) {
        jumps[i] = INT_MAX;
        for (j = 0; j < i; j++) {
            if (i <= j + arr[j] && jumps[j] != INT_MAX) {
                jumps[i] = min(jumps[i], jumps[j] + 1);
                break;
            }
        }
    }

    return jumps[n - 1];
}

// Driver program to test above function
int main()
{
    int arr[] = { 1, 3, 6, 3, 2, 3, 6, 8, 9, 5 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Minimum number of jumps to reach end is %d \r\n", minJumps(arr, n));
    return 0;
}