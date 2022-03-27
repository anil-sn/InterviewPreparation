#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define SWAP(a_m, b_m)  (((a_m) ^= (b_m)), ((b_m) ^= (a_m)), ((a_m) ^= (b_m)))
#define MIN(a_m,b_m)    (b_m ^ ((a_m ^ b_m) & -(a_m < b_m)))
#define MAX(a_m,b_m)    (a_m ^ ((a_m ^ b_m) & -(a_m < b_m)))
#define SETBIT(flag_m, mask_m, val_m) (val_m = (val_m & ~mask_m) | (-flag_m & mask_m))
#define MERGE(a_m,b_m,r_m,mask_m) (r_m = a_m ^ ((a_m ^ b_m) & mask_m))

// Print a digit in binary
void dumpBinary(int n, char reason[]) {
    int i;
    printf("0");
    for (i = 1 << 30; i > 0; i = i / 2) {
        if((n & i) != 0) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("[ %d ]: %s ", n, reason);
    printf("\r\n");
}
/*
    A Boolean Array Puzzle
    Input: A array arr[] of two elements having value 0 and 1
    Output: Make both elements 0.

    Specifications: Following are the specifications to follow.

    1) It is guaranteed that one element is 0 but we do not know its position.
    2) We can’t say about another element it can be 0 or 1.
    3) We can only complement array elements, no other operation like
       and, or, multi, division, ... etc.
    4) We can’t use if, else and loop constructs.
    5) Obviously, we can’t directly assign 0 to array elements.
*/

void changeToZero(int a[2]) {
    a[a[1]] = a[!a[1]];
}

int main()
{
    int a[] = {1, 0};
    changeToZero(a);

    printf(" arr[0] = %d \r\n", a[0]);
    printf(" arr[1] = %d \r\n", a[1]);
    getchar();
    return 0;
}