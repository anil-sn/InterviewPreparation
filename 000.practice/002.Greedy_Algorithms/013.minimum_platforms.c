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

/*
    Given arrival and departure times of all trains that reach a railway station.
    Find the minimum number of platforms required for the railway station
    so that no train is kept waiting.
    Consider that all the trains arrive on the same day and leave on the same day.
    Arrival and departure time can never be the same for a train but we can have arrival
    time of one train equal to departure time of the other.
    At any given instance of time, same platform can not be used for both departure of a train
    and arrival of another train. In such cases, we need different platforms.

    Input: n = 6
    arr[] = {0900, 0940, 0950, 1100, 1500, 1800}
    dep[] = {0910, 1200, 1120, 1130, 1900, 2000}
    Output: 3
    Explanation:
    Minimum 3 platforms are required to safely arrive and depart all trains.

    Input: n = 3
    arr[] = {0900, 1100, 1235}
    dep[] = {1000, 1200, 1240}
    Output: 1
    Explanation: Only 1 platform is required to safely manage the arrival and departure of all trains.

    Your Task:
    Your task is to complete the function findPlatform() which takes the array arr[]
    (denoting the arrival times), array dep[] (denoting the departure times) and the
    size of the array as inputs and returns the minimum number of platforms required at the
    railway station such that no train waits.

    Note: Time intervals are in the 24-hour format(HHMM) ,
    where the first two characters represent hour (between 00 to 23 )
    and the last two characters represent minutes (this may be > 59).
*/
/*
    Naive Solution:
        Approach: The idea is to take every interval one by one and find the number
        of intervals that overlap with it. Keep track of the maximum number of intervals
        that overlap with an interval. Finally, return the maximum value.
    Algorithm:
        Run two nested loops the outer loop from start to end and the inner loop from i+1 to end.
        For every iteration of the outer loop find the count of intervals that intersect with the
        current interval.
        Update the answer with the maximum count of overlap in each iteration of the outer loop.
*/
int findPlatform(int arr[], int dep[], int n)
{
    // plat_needed indicates number of platforms
    // needed at a time
    int plat_needed = 1, result = 1;
    int i = 1, j = 0;

    // run a nested  loop to find overlap
    for (int i = 0; i < n; i++) {

        // minimum platform
        plat_needed = 1;

        for (int j = i + 1; j < n; j++) {

            // check for overlap
            if ((arr[i] >= arr[j] && arr[i] <= dep[j]) ||
                (arr[j] >= arr[i] && arr[j] <= dep[i]))
                plat_needed++;
        }

        // update result
        result = max(result, plat_needed);
    }

    return result;
}

int main () {
    int n = 6;
    int arr[6] = { 900,  940,  950, 1100, 1500, 1800};
    int dep[6] = { 910, 1200, 1120, 1130, 1900, 2000};

    int result = findPlatform(arr, dep, n);
    printf ("Minimuim number of trains required is %d\r\n", result);

    return 0;
}