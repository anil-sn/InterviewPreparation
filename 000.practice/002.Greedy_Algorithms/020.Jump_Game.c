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

    Approach: A naive approach is to start from the first element and recursively call for all
    the elements reachable from first element. The minimum number of jumps to reach end from
    first can be calculated using minimum number of jumps needed to reach end from the elements
    reachable from first.

    minJumps(start, end) = Min ( minJumps(k, end) ) for all k reachable from start
*/

// Returns minimum number of
// jumps to reach arr[h] from arr[l]
int minJumps(int arr[], int l, int h)
{
    // Base case: when source and destination are same
    if (h == l)
        return 0;

    // When nothing is reachable from the given source
    if (arr[l] == 0)
        return INT_MAX;

    // Traverse through all the points
    // reachable from arr[l]. Recursively
    // get the minimum number of jumps
    // needed to reach arr[h] from these
    // reachable points.
    int min = INT_MAX;
    for (int i = l + 1; i <= h && i <= l + arr[l]; i++) {
        int jumps = minJumps(arr, i, h);
        if (jumps != INT_MAX && jumps + 1 < min) {
            min = jumps + 1;
        }
    }

    return min;
}

// Driver program to test above function
int main()
{
    int arr[] = { 1, 3, 6, 3, 2, 3, 6, 8, 9, 5 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf(
        "Minimum number of jumps to reach end is %d \r\n",
        minJumps(arr, 0, n - 1));
    return 0;
}