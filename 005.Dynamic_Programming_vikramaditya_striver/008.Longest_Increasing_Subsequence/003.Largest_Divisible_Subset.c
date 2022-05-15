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


int ldsIterative(int n, int arr[])
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
            if ((0 == (arr[i] % arr[prev])) &&
                (1 + dp[prev] > dp[i])) {

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

int cmp(const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    return (x - y);
}

int main ()
{
    int arr[] = {16, 4, 7, 8, 1};
    int n = SIZEOF(arr);

    qsort(arr, n, sizeof(arr[0]), cmp);
    for (int i = 0; i < n; i++){
        printf("%4d", arr[i]);
    }
    printf("\r\n");

    int lds = ldsIterative(n, arr);
    printf("The length of longest divisable subsequence is : %d \r\n", lds);
    return 0;
}