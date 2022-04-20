#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int maxSubArray(int nums[], int n, int subarray[100])
{
    int maxSoFar = nums[0];
    int maxEndHere = 0;
    int start = 0;
    int pos = 0;

    for (int i = 0; i < n; i++) {

        maxEndHere += nums[i];
        if (maxEndHere > maxSoFar) {

            pos = 0;
            memset(subarray, 0, sizeof(int) * 100);

            maxSoFar = maxEndHere;
            subarray[pos++] = start;
            subarray[pos++] = i;
        }

        if (maxEndHere < 0) {
            maxEndHere = 0;
            start = i + 1;
        }
    }

    return maxSoFar;
}

int main()
{

    int arr[] = {-2,-3,4,-1,-2,-1,5,4};
    int subarray[100] = {0};
    int n = sizeof(arr)/sizeof(arr[0]);

    int lon = maxSubArray(arr, n, subarray);
    printf("The longest subarray with maximum sum is : %d\r\n", lon);
    printf("The subarray is : \r\n");
    for (int i = subarray[0]; i <= subarray[1]; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");
}