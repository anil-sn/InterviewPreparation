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
    Approach: The idea is to consider all events in sorted order.
    Once the events are in sorted order, trace the number of trains at any time keeping
    track of trains that have arrived, but not departed.

    For example, consider the above example.

    arr[]  = {9:00,  9:40, 9:50,  11:00, 15:00, 18:00}
    dep[]  = {9:10, 12:00, 11:20, 11:30, 19:00, 20:00}

    All events are sorted by time.
    Total platforms at any time can be obtained by
    subtracting total departures from total arrivals by that time.

    Time      Event Type     Total Platforms Needed
                                at this Time
    9:00       Arrival                  1
    9:10       Departure                0
    9:40       Arrival                  1
    9:50       Arrival                  2
    11:00      Arrival                  3
    11:20      Departure                2
    11:30      Departure                1
    12:00      Departure                0
    15:00      Arrival                  1
    18:00      Arrival                  2
    19:00      Departure                1
    20:00      Departure                0

    Minimum Platforms needed on railway station
    = Maximum platforms needed at any time
    = 3

    1. Sort the arrival and departure times of trains.
    2. Create two pointers i=0, and j=0 and a variable to store ans and current count plat
    3. Run a loop while i<n and j<n and compare the ith element of arrival array
       and jth element of departure array.
    4. If the arrival time is less than or equal to departure
       then one more platform is needed so increase the count,
       i.e. plat++ and increment i
    5. Else if the arrival time greater than departure then one less platform
       is needed so decrease the count, i.e. plat - and increment j
    6. Update the ans, i.e ans = max(ans, plat).
*/

int cmp(const void *a, const void *b)
{
    int *i = (int *)a;
    int *j = (int *)b;

    return *i > *j;
}

int findPlatform(int arr[], int dep[], int n)
{
    // Sort arrival and departure arrays
    qsort(arr, n, sizeof(arr[0]), cmp);
    qsort(dep, n, sizeof(dep[0]), cmp);

    // plat_needed indicates number of platforms
    // needed at a time
    int plat_needed = 1, result = 1;
    int i = 1, j = 0;

    // Similar to merge in merge sort to process
    // all events in sorted order
    while (i < n && j < n) {
        // If next event in sorted order is arrival,
        // increment count of platforms needed
        if (arr[i] <= dep[j]) {
            plat_needed++;
            i++;
        }

        // Else decrement count of platforms needed
        else if (arr[i] > dep[j]) {
            plat_needed--;
            j++;
        }

        // Update result if needed
        if (plat_needed > result)
            result = plat_needed;
    }

    return result;
}

int main (){
    int n = 6;
    int arr[6] = { 900,  940,  950, 1100, 1500, 1800};
    int dep[6] = { 910, 1200, 1120, 1130, 1900, 2000};

    int result = findPlatform(arr, dep, n);
    printf ("Minimuim number of trains required is %d\r\n", result);

    return 0;
}