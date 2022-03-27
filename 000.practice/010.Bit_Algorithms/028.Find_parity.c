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
    Program to find parity

    Parity: Parity of a number refers to whether it contains an odd or even number of 1-bits.
    The number has “odd parity”, if it contains odd number of 1-bits and is “even parity” if
    it contains even number of 1-bits.

    Main idea of the below solution is – Loop while n is not 0 and in loop unset one of the
    set bits and invert parity.

    Algorithm: getParity(n)
    1. Initialize parity = 0
    2. Loop while n != 0
        a. Invert parity
                parity = !parity
        b. Unset rightmost set bit
                n = n & (n-1)
    3. return parity

    Example:
    Initialize: n = 13 (1101)   parity = 0

    n = 13 & 12     = 12 (1100)   parity = 1
    n = 12 & 11     = 8  (1000)   parity = 0
    n = 8 & 7       = 0  (0000)   parity = 1
*/

/* Function to get parity of number n. It returns 1
   if n has odd parity, and returns 0 if n has even
   parity */
bool getParity(unsigned int n)
{
    bool parity = 0;
    while (n)
    {
        parity = !parity;
        n      = n & (n - 1);
    }
    return parity;
}

/* Driver program to test getParity() */
int main()
{
    unsigned int n = 7;
    printf("Parity of no %d = %s \r\n",  n, (getParity(n) ? "odd": "even"));
    return 0;
}