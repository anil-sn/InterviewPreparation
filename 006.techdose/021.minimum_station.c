#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int cmp (const void *a, const void *b) 
{
    int x = *(int *)a;
    int y = *(int *)b; 

    return (x - y);
}

// Returns minimum number of platforms reqquired 
int findPlatform(int arr[], int dep[], int n) 
{ 
    // Sort arrival and departure arrays 
    qsort(arr, n, sizeof(int), cmp); 
    qsort(dep, n, sizeof(int), cmp); 

    // plat_needed indicates number of platforms 
    // needed at a time 
    int plat_needed = 1, result = 1; 
    int i = 1, j = 0; 

    // Similar to merge in merge sort to process 
    // all events in sorted order 
    while (i < n && j < n) 
    { 
        // If next event in sorted order is arrival, 
        // increment count of platforms needed 
        if (arr[i] <= dep[j]) 
        { 
            plat_needed++; 
            i++; 

            // Update result if needed 
            if (plat_needed > result) 
                result = plat_needed; 
        } 
        // Else decrement count of platforms needed 
        else
        { 
            plat_needed--; 
            j++; 
        } 
    } 

    return result; 
} 

// Driver program to test methods of graph class 
int main() 
{ 
	int arr[] = {900, 940, 950, 1100, 1500, 1800}; 
	int dep[] = {910, 1200, 1120, 1130, 1900, 2000}; 
	int n = sizeof(arr)/sizeof(arr[0]); 
	printf("Minimum Number of Platforms Required = %d\r\n", findPlatform(arr, dep, n)); 
	return 0; 
} 