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
    int count[n];
    for (int i = 0; i < n; i++){
        dp[i] = 1;
        count[i] = 1;
    }

    int maxi = 0;
    for (int i = 0; i < n; i++){
        for (int prev = 0; prev < i; prev++) {

            if ((arr[prev] < arr[i]) &&
                (1 + dp[prev] > dp[i])){

                dp[i] = 1 + dp[prev];

                // Inherit
                count[i] = count[prev];

            } else if ((arr[prev] < arr[i]) &&
                       (1 + dp[prev] == dp[i])) {

                // Increase the count
                count[i] += count[prev];
            }
        }

        maxi = MAX(maxi, dp[i]);
    }

    printf("DP array : \r\n");
    for (int i = 0; i < n; i++){
        printf("%4d", dp[i]);
    }
    printf("\r\n");

    printf("Count array : \r\n");
    for (int i = 0; i < n; i++){
        printf("%4d", count[i]);
    }
    printf("\r\n");

    int nos = 0;
    for (int i = 0; i < n; i++){
        if (dp[i] == maxi) {
            nos += count[i];
        }
    }

    return nos;
}

int main ()
{
    int arr[] = {1, 5, 4, 3, 2, 6, 7, 10, 8, 9 };
    int len = SIZEOF(arr);

    printf("The Given Sequence of numbers : \r\n");
    for (int i = 0; i < len; i++) {
        printf("%4d", arr[i]);
    }
    printf("\r\n");

    int result = lisIterative(len, arr);
    printf("[iterative] Number of Longest Increasing Subsequence : %d \r\n", result);

    return 0;
}