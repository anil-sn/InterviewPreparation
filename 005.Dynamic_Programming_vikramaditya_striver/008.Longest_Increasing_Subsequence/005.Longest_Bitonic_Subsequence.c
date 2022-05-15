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

int lbsIterative(int n, int arr[])
{
    int dpFront[n];
    int dpBack[n];
    for (int i = 0; i < n; i++){
        dpFront[i] = 1;
        dpBack[i] = 1;
    }

    for (int i = 0; i < n; i++){
        for (int prev = 0; prev < i; prev++) {
            if ((arr[prev] < arr[i]) && (1 + dpFront[prev] > dpFront[i])){
                dpFront[i] = 1 + dpFront[prev];
            }
        }
    }

    for (int i = n-1; i >= 0; i--){
        for (int prev = n-1; prev > i; prev--) {
            if ((arr[prev] < arr[i]) && (1 + dpBack[prev] > dpBack[i])){
                dpBack[i] = 1 + dpBack[prev];
            }
        }
    }

    int maxi = 0;
    for (int i = 0; i < n; i++) {
        maxi = MAX(maxi, (dpFront[i] + dpBack[i] - 1));
    }

    return maxi;
}

int main ()
{
    int arr[] = {1, 11, 2, 10, 4, 5, 2, 1};
    int n = SIZEOF(arr);

    int lbs = lbsIterative(n, arr);
    printf("The length of longest bitonic subsequence is : %d \r\n", lbs);
    return 0;
}