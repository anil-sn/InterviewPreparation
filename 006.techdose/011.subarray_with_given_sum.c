#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// Non negative integers array
void subArraySum(int n, int arr[n], int s)
{
    if(s == 0){ // handle sum = 0
        for(int i=0; i<n; i++) {
            if(arr[i] == 0){
                printf("Start : %d, End %d \r\n", i, i);
                return;
            }
        }
        printf("Start : %d, End %d \r\n", -1, -1);
        return;
    }

    int l = 0, r=0;
    int curr_sum = 0;
    while(l<n && r<=n && l<=r){
        if(curr_sum == s) {
            printf("Start : %d, End %d \r\n", l, r-1);
            return;
        }

        if(curr_sum < s)
            curr_sum += arr[r++];
        else
            curr_sum -= arr[l++];
    }

    printf("Start : %d, End %d \r\n", -1, -1);
    return;
}

// Driver code 
int main() 
{ 
	// arr stores sequence of points  that are to be visited 
	int arr[] = { 1,4,10,4,19,5 };
	int n = sizeof(arr) / sizeof(arr[0]); 
    int sum = 33;
	subArraySum(n, arr, sum);
} 