#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// https://takeuforward.org/dynamic-programming/striver-dp-series-dynamic-programming-problems/
// https://youtu.be/ekcwMsSIzVc

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999

int lisIterative(int n, int arr[])
{
    int dp[n];
    int hash[n];
    for (int i = 0; i < n; i++){
        dp[i] = 1;
        hash[i] = i;
    }

    int maxi = 1;
    int last = 0;
    for (int i = 0; i < n; i++){
        for (int prev = 0; prev < i; prev++) {
            if ((arr[prev] < arr[i]) && (1 + dp[prev] > dp[i])){
                dp[i] = 1 + dp[prev];
                hash[i] = prev;
            }
        }
        if (dp[i] > maxi){
            last = i;
            maxi = dp[i];
        }
    }

    int lis[maxi];

    int pos = 0;
    lis[pos++] = arr[last];

    while (hash[last] != last){
        last = hash[last];
        lis[pos++] = arr[last];
    }

    printf("LIS : \r\n");
    for (int i = pos-1; i >= 0; i--){
        printf("%3d", lis[i]);
    }
    printf("\r\n");

    return maxi;
}

/*  Intution for mid = low + (high - low) / 2;

    Assume x such that its is after low:
    mid = low + x

    ( high + low ) / 2 = low + x
    (high + low) = 2 (low + x)
    high + low = 2low + 2x
    2x + 2low = high + low
    2x = high + low - 2x
    2x = hight - low
    x = (hight - low) / 2

    mid = low + x
    mid = low + (high - low) / 2
*/
int lower_bound(int arr[], int N, int X)
{
    int mid;

    int low = 0;
    int high = N;

    while (low < high) {

        mid = low + (high - low) / 2;
        if (X <= arr[mid]) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    // if X is greater than arr[n-1]
    if(low < N && arr[low] < X) {
       low++;
    }

    // Return the lower_bound index
    return low;
}

int lisBinarySearch(int n, int arr[])
{
    int temp[n];
    int pos = 0;
    memset(temp, 0, sizeof(temp));

    temp[pos++] = arr[0];
    for (int i = 1; i < n; i++) {
        if(arr[i] > temp[pos - 1]) {
            temp[pos++] = arr[i];
        } else {
            int ind = lower_bound(temp, pos, arr[i]);
            temp[ind] = arr[i];
        }
    }

    return pos;
}

int main ()
{
    int arr[] = {1, 7, 8, 4, 5, 6, -1, 9};
    int len = SIZEOF(arr);

    printf("The Given Sequence of numbers : \r\n");
    for (int i = 0; i < len; i++) {
        printf("%4d", arr[i]);
    }
    printf("\r\n");

    int result = lisIterative(len, arr);
    printf("[iterative] The Longest Increasing Subsequence : %d \r\n", result);

    result = lisBinarySearch(len, arr);
    printf("[Binary Search] The Longest Increasing Subsequence : %d \r\n", result);

    return 0;
}