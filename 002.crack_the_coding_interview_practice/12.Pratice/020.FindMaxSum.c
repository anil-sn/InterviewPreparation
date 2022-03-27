#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Dont practice until you get it right
    practice until you can't get it worng
*/

/*Function to return max sum such that no two elements
are adjacent */
int FindMaxSum(int arr[], int n) {
    int incl = arr[0];
    int excl = 0;
    int excl_new;
    int i;

    printf ("[5, 1, 10, 100, 10, 5]\r\n\r\n");
    for (i = 1; i < n; i++) {

        printf ("Start -> arr[%d] = %d, incl = %d; excl = %d \r\n", i, arr[i], incl, excl);

        /* current max excluding i */
        excl_new = (incl > excl)? incl: excl;
        printf ("Current Max excluding i -> %d \r\n", excl_new);

        /* current max including i */
        incl = excl + arr[i];
        excl = excl_new;
        printf ("Current Max including i -> %d \r\n", incl);

        printf ("End -> i = %d, incl = %d; excl = %d \r\n\r\n", i, incl, excl);
    }

    /* return max of incl and excl */
    return ((incl > excl)? incl : excl);
}

/* Driver program to test above function */
int main() {
    int arr[] = {5, 1, 10, 100, 10, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("So the Max Sum is : %d \r\n", FindMaxSum(arr, n));
    return 0;
}