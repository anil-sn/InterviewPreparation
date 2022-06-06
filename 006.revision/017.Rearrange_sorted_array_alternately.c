#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


/* Trick Store two numbers in one location
    ME = max element value + 1
    
    if i is even number
        arr[i] = arr[i] + arr[maxi] % ME * ME
    else 
        arr[i] = arr[i] + arr[mini] % ME * ME

    arr[0] = 1 + (5 % 6) * 6
    arr[0] = 1 + (5) * 6
    arr[0] = 1 + 30
    arr[0] = 31

      1   2   3   4   5
    mini             maxi

    Finally :
    arr[i] % ME = original Vlaue 
    arr[i] / ME = alternate Value
*/

void reArrangeSortedArray(int alen, int a[alen])
{
    int mini = 0;
    int maxi = alen-1;
    int me = a[maxi] + 1;

    if (alen <= 1)
        return;

    for (int i = 0; i < alen; i++) {
        if (i%2 == 0) {
            a[i] = a[i] + (a[maxi] % me) * me;
            maxi -= 1;
        } else {
            a[i] = a[i] + (a[mini] % me) * me;
            mini += 1;
        }
    }

    printf("Original Array: \r\n");
    for (int i = 0; i < alen; i++) {
        printf("%d ", a[i] % me);
    }
    printf("\r\n");

    printf("Trasposed Array: \r\n");
    for (int i = 0; i < alen; i++) {
        printf("%d ", a[i] / me);
    }
    printf("\r\n");

    return;
}

int main(void) {

    int a[] = {1, 3, 5, 7, 9, 11, 13};
    int alen = sizeof(a)/sizeof(a[0]);
    
    reArrangeSortedArray(alen, a);

    return 0;
}

