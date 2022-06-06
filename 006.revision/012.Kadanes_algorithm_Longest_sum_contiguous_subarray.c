#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int maxSubArray(int n, int a[n]) {
    
    int meh = 0;
    int msf = INT_MIN;
    for(int i = 0; i < n; i++)
    {
        meh = meh + a[i];
        if (meh < a[i]) {
            meh = a[i];
        }

        if (msf < meh) {
            msf = meh;
        }
    }
    
    return msf;
}

// Driver code 
int main() 
{ 
	// arr stores sequence of points  that are to be visited 
	int arr[] = { -2, -3, 4, -1, -2, 1, 5, -3 };
	int n = sizeof(arr) / sizeof(arr[0]);     
	printf("Maximum SubArray Sum is : %d\r\n", maxSubArray(n, arr));
} 