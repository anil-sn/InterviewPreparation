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
    Compute modulus division by a power-of-2-number

        Compute n modulo d without division(/) and modulo(%) operators, where d is a power of 2 number.
        Let ith bit from right is set in d. For getting n modulus d, we just need to return 0 to i-1
        (from right) bits of n as they are and other bits as 0.
        For example if n = 6 (00..110) and d = 4(00..100). Last set bit in d is at position 3 (from right side).
        So we need to return last two bits of n as they are and other bits as 0, i.e., 00..010.

*/

// This function will return n % d.
// d must be one of: 1, 2, 4, 8, 16, 32, …
unsigned int getModulo(unsigned int n, unsigned int d)
{
    return ( n & (d - 1) );
}

// Driver Code
int main()
{
    unsigned int n = 6;

    // d must be a power of 2
    unsigned int d = 4;
    printf("%u modulo %u is %u\r\n", n, d, getModulo(n, d));

    return 0;
}