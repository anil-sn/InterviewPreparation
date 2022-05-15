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
Given an array of integers where each element represents the max number of steps
that can be made forward from that element. Write a function to return the minimum
number of jumps to reach the end of the array (starting from the first element).
If an element is 0, they cannot move through that element.
If the end isn’t reachable, return -1.

Examples:

    Input: arr[] = {1, 3, 5, 8, 0, 2, 6, 7, 6, 8, 9}
    Output: 3 (1-> 3 -> 9 -> 9)
    Explanation: Jump from 1st element
    to 2nd element as there is only 1 step,
    now there are three options 5, 8 or 9.
    If 8 or 9 is chosen then the end node 9
    can be reached. So 3 jumps are made.

    Input:  arr[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    Output: 10
    Explanation: In every step a jump
    is needed so the count of jumps is 10.
    The first element is 1, so can only go to 3. The second element is 3, so can make at most 3 steps eg to 5 or 8 or 9.
*/

int min(int a, int b){
    return (a < b ? a : b);
}

// Function to return the minimum number
// of jumps to reach arr[h] from arr[l]
int minJumps(int arr[], int n)
{

    // Base case: when source and destination are same
    if (n == 1)
        return 0;

    // Traverse through all the points reachable from arr[l]
    // Recursively, get the minimum number of jumps needed to reach arr[h]
    // from these reachable points
    int res = INT_MAX;
    for (int i = n - 2; i >= 0; i--) {
        if (i + arr[i] >= n - 1) {
            int sub_res = minJumps(arr, i + 1);
            if (sub_res != INT_MAX)
                res = min(res, sub_res + 1);
        }
    }

    return res;
}

// Driver Code
int main()
{
    int arr[] = { 1, 3, 6, 3, 2, 3, 6, 8, 9, 5 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Minimum number of jumps to reach the end is %d\r\n",minJumps(arr, n));
    return 0;
}