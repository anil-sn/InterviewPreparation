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
    How to turn off a particular bit in a number?

    Given a number n and a value k, turn off the k’th bit in n.
    Please note that k = 1 means the rightmost bit.

    Examples:
        Input:  n = 15, k = 1
        Output: 14

        Input:  n = 14, k = 1
        Output: 14
        The rightmost bit was already off, so no change.

        Input:  n = 15, k = 2
        Output: 13

        Input:  n = 15, k = 3
        Output: 11

        Input:  n = 15, k = 4
        Output: 7

        Input:  n = 15, k >= 5
        Output: 15

    The idea is to use bitwise <<, & and ~ operators.
    Using expression “~(1 << (k – 1))“, we get a number which has all bits set, except the k’th bit.
    If we do bitwise & of this expression with n, we get a number which has all bits same as n except
    the k’th bit which is 0.
*/

// Returns a number that has all bits same as n
// except the k'th bit which is made 0
uint32 turnOffK(uint32 n, uint32 k)
{
    // k must be greater than 0
    if (k <= 0) return n;

    // Do & of n with a number with all set bits except
    // the k'th bit
    return (n & ~(1 << (k - 1)));
}

// Driver program to test above function
uint32 main()
{
    uint32 n = 15;
    uint32 k = 4;

    dumpBinary(n, "Input");
    uint32 result = turnOffK(n, k);
    dumpBinary(result, "Output");
    return 0;
}