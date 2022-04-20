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
    int max_sum = 0;
    int pos = 0;

    if (n == 1) {
        return nums[0];
    }

    int i, j;
    for (i = 0; i <= n - 1; i++) {
        for (j = i; j <= n - 1; j++) {

            int sum = 0;
            for (int k = i; k <= j; k++) {

                sum = sum + nums[k];
            }

            if (sum > max_sum) {

                memset(subarray, 0, sizeof(int) * 100);
                max_sum = sum;
                pos = 0;
                subarray[pos++] = i;
                subarray[pos++] = j;
            }
        }
    }
    return max_sum;
}

int maxSubArray2(int nums[], int n, int subarray[100])
{
    int max_sum = INT_MIN;
    int pos = 0;

    if (n == 1) {
        return nums[0];
    }

    int i, j;
    for (i = 0; i <= n - 1; i++) {

        int curr_sum = 0;
        for (j = i; j <= n - 1; j++) {

            curr_sum = curr_sum + nums[j];
            if (curr_sum > max_sum) {

                memset(subarray, 0, sizeof(int) * 100);
                max_sum = curr_sum;
                pos = 0;
                subarray[pos++] = i;
                subarray[pos++] = j;
            }
        }
    }
    return max_sum;
}

int main()
{

    int arr[] = {-2,1,-3,4,-1,2,1,-5,4};
    int subarray[100] = {0};
    int n = sizeof(arr)/sizeof(arr[0]);

    int lon = maxSubArray(arr, n, subarray);
    printf("The longest subarray with maximum sum is : %d\r\n", lon);
    printf("The subarray is : \r\n");
    for (int i = subarray[0]; i <= subarray[1]; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");

    lon = maxSubArray2(arr, n, subarray);
    printf("The longest subarray with maximum sum is : %d\r\n", lon);
    printf("The subarray is : \r\n");
    for (int i = subarray[0]; i <= subarray[1]; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");
}