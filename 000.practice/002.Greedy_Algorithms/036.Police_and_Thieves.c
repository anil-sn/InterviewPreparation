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
    Policemen catch thieves
    Given an array of size n that has the following specifications:

    Each element in the array contains either a policeman or a thief.
    Each policeman can catch only one thief.
    A policeman cannot catch a thief who is more than K units away from the policeman.
    We need to find the maximum number of thieves that can be caught.

    Examples:
    Input : arr[] = {'P', 'T', 'T', 'P', 'T'},
                k = 1.
    Output : 2.
    Here maximum 2 thieves can be caught, first
    policeman catches first thief and second police-
    man can catch either second or third thief.

    Input : arr[] = {'T', 'T', 'P', 'P', 'T', 'P'},
                k = 2.
    Output : 3.

    Input : arr[] = {'P', 'T', 'P', 'T', 'T', 'P'},
                k = 3.
    Output : 3.

    This approach takes the following steps:

    First find the left most police and thief and store the indices. There can be two cases:
    CASE 1: If the distance between the police and thief <= k (given), the thief can be caught,
            so increment the res counter
    CASE 2: If the distance between the police and thief >= k, the current thief cannot be caught
            by the current police
    For CASE 2, if the police is behind the thief, we need to find the next police and
                    check if it can catch the current thief
                if the thief is behind the police, we need to find the next thief and
                    check if the current police can catch the thief

    Repeat the process until we find the next police and thief pair,
    and increment result counter if conditions are met, i,e, CASE 1.

    Algorithm:
    1. Initialize the current lowest indices of policeman in pol and thief in thi variable as -1.
    2. Find the lowest index of policeman and thief.
    3. If lowest index of either policeman or thief remain -1 then return 0.
    4. If |pol – thi| <= k then make an allotment and find the next policeman and thief.
    5. Else increment the min(pol , thi) to the next policeman or thief found.
    6. Repeat the above two steps until we can find the next policeman and thief.
    7. Return the number of allotments made.
*/

int policeThief(char arr[], int n, int k)
{
    // Initialize the current lowest indices of
    // policeman in pol and thief in thi variable as -1
    int pol = -1, thi = -1, res = 0;

    // Find the lowest index of policemen
    for (int i = 0; i < n; i++) {
        if (arr[i] == 'P') {
            pol = i;
            break;
        }
    }

    // Find the lowest index of thief
    for (int i = 0; i < n; i++) {
        if (arr[i] == 'T') {
            thi = i;
            break;
        }
    }

    // If lowest index of either policemen or thief remain
    // -1 then return 0
    if (thi == -1 || pol == -1)
        return 0;

    while (pol < n && thi < n) {

        // can be caught
        if (abs(pol - thi) <= k) {

            pol = pol + 1;
            while (pol < n && arr[pol] != 'P') {
                pol = pol + 1;
            }

            thi = thi + 1;
            while (thi < n && arr[thi] != 'T') {
                thi = thi + 1;
            }

            res++;
        }

        // increment the current min(pol , thi) to
        // the next policeman or thief found
        else if (thi < pol) {
            thi = thi + 1;
            while (thi < n && arr[thi] != 'T') {
                thi = thi + 1;
            }
        } else {
            pol = pol + 1;
            while (pol < n && arr[pol] != 'P') {
                pol = pol + 1;
            }
        }
    }

    return res;
}

// Driver Code Starts.
int main()
{
    int k, n;

    char arr1[] = { 'P', 'T', 'T', 'P', 'T' };
    k = 2;
    n = sizeof(arr1) / sizeof(arr1[0]);
    printf("Maximum thieves caught: %d\r\n", policeThief(arr1, n, k));

    char arr2[] = { 'T', 'T', 'P', 'P', 'T', 'P' };
    k = 2;
    n = sizeof(arr2) / sizeof(arr2[0]);
    printf("Maximum thieves caught: %d\r\n", policeThief(arr2, n, k));

    char arr3[] = { 'P', 'T', 'P', 'T', 'T', 'P' };
    k = 3;
    n = sizeof(arr3) / sizeof(arr3[0]);
    printf("Maximum thieves caught: %d\r\n", policeThief(arr3, n, k));

    return 0;
}
