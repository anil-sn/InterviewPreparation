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
    Compute the integer absolute value (abs) without branching

    We need not to do anything if a number is positive. We want to change only negative numbers.
    Since negative numbers are stored in 2’s complement form, to get the absolute value of a
    negative number we have to toggle bits of the number and add 1 to the result.

    For example -2 in a 8 bit system is stored as follows 1 1 1 1 1 1 1 0 where leftmost bit is the
    sign bit. To get the absolute value of a negative number, we have to toggle all bits and add 1
    to the toggled number i.e, 0 0 0 0 0 0 0 1 + 1 will give the absolute value of 1 1 1 1 1 1 1 0.
    Also remember, we need to do these operations only if the number is negative (sign bit is set).
*/

/* This function will return absolute value of n*/
unsigned int getAbs(int n)
{
    int const mask = n >> (sizeof(int) * CHAR_BIT - 1);
    return ((n + mask) ^ mask);
}

/* Driver program to test above function */
int main()
{
    int n = -6;
    printf("Absolute value of %d is %u \r\n", n, getAbs(n));

    return 0;
}