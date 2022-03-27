#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"
#include "time.h"

/* Major data types typedef. */
typedef unsigned char           boolean;

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned int            uint32;
typedef unsigned long long      uint64;

typedef char                    int8;
typedef short                   int16;
typedef int                     int32;
typedef long long               int64;

typedef float                   float_t;
typedef double                  double_t;

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
    Check for Integer Overflow

    Write a “C” function, int addOvf(int* result, int a, int b) If there is no overflow,
    the function places the resultant = sum a+b in “result” and returns 0. Otherwise
    it returns -1. The solution of casting to long and adding to find detecting the
    overflow is not allowed.

    Method:
    There can be overflow only if signs of two numbers are same, and sign of sum is opposite
    to the signs of numbers.
    1)  Calculate sum
    2)  If both numbers are positive and sum is negative then return -1
        Else
            If both numbers are negative and sum is positive then return -1
            Else return 0
*/

int addOvf(int* result, int a, int b)
{
    if (a > INT_MAX - b)
        return -1;
    else {
        *result = a + b;
        return 0;
    }
}

int main()
{
    int* res = (int*)malloc(sizeof(int));
    int x = 2147483640;
    int y = 10;

    printf("Check if adding two integer will overflow %d \r\n", addOvf(res, x, y));
    printf("Value : %d\r\n", *res);

    return 0;
}