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
    Minimum number of platforms required for a railway
    Problem Statement: We are given two arrays that represent the arrival and departure times of
        trains that stop at the platform. We need to find the minimum number of platforms needed at
        the railway station so that no train has to wait.

    Examples 1:

    Input: N=6,
        arr[] = {9:00,  9:45,  9:55, 11:00, 15:00, 18:00}
        dep[] = {9:20, 12:00, 11:30, 11:50, 19:00, 20:00}

    Output:3
        Explanation: There are at-most three trains at a time. The train at 11:00 arrived but
        the trains which had arrived at 9:45 and 9:55 have still not departed.
        So, we need at least three platforms here.

    Example 2:
        Input Format: N=2,
            arr[]={10:20,12:00}
            dep[]={10:50,12:30}

    Output: 1
    Explanation: Before the arrival of the new train, the earlier train already departed.
        So, we don’t require multiple platforms.
*/

int cmp (const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    return (x-y);
}

int findPlatform(int arr[], int dep[], int totalTrains)
{
    qsort(arr, totalTrains, sizeof(arr[0]), cmp);
    qsort(dep, totalTrains, sizeof(dep[0]), cmp);

    int platNeeded = 1,
    int maxPlatUsed = 1;
    int i = 1;
    int j = 0;

    while (i < totalTrains && j < totalTrains) {

        if (arr[i] <= dep[j]) {
            platNeeded++;
            i++;
        }

        else if (arr[i] > dep[j]) {
            platNeeded--;
            j++;
        }

        if (platNeeded > maxPlatUsed) {
            maxPlatUsed = platNeeded;
        }
    }

    return maxPlatUsed;
}

 int main () {

    int arr[] = {900,  945,  955, 1100, 1500, 1800};
    int dep[] = {920, 1200, 1130, 1150, 1900, 2000};
    int totalTrains = sizeof(arr)/sizeof(arr[0]);

    int totalCount = findPlatform(arr, dep, totalTrains);
    printf("Minimum number of Platforms required %d \r\n",totalCount);
}
