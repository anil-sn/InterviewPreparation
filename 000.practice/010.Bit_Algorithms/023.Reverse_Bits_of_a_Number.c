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
    Write an Efficient C Program to Reverse Bits of a Number
    Given an unsigned integer, reverse all bits of it and return the number with reversed bits.

    Input : n = 1
    Output : 2147483648
    On a machine with size of unsigned bit as 32.
    Reverse of 0....001 is 100....0.

    Input : n = 2147483648
    Output : 1
*/

uint32 reverseBits(uint32 num)
{
    uint32 count = sizeof(num) * CHAR_BIT - 1;
    dumpBinary(count, "count");
    uint32 reverse_num = num;

    num >>= 1;
    while(num)
    {
       reverse_num <<= 1;
       reverse_num |= num & 1;
       num >>= 1;
       count--;
    }
    reverse_num <<= count;
    return reverse_num;
}

int main()
{
    uint32 x = 1;
    uint32 rev = 0;
    printf ("Input : 0x%x \r\n", x);
    dumpBinary(x, "Input");
    rev = reverseBits(x);
    printf ("Output : 0x%x \r\n", rev);
    dumpBinary(rev, "Output");

    x = 0xa0a0;
    rev = 0;
    printf ("Input : 0x%x \r\n", x);
    dumpBinary(x, "Input");
    rev = reverseBits(x);
    printf ("Output : 0x%x \r\n", rev);
    dumpBinary(rev, "Output");
}