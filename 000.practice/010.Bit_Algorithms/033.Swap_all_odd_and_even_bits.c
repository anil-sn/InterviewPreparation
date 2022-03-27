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
    Swap all odd and even bits

    Given an unsigned integer, swap all odd bits with even bits.
    For example, if the given number is 23 (00010111), it should be converted to 43 (00101011).
    Every even position bit is swapped with adjacent bit on right side
    (even position bits are highlighted in binary representation of 23),
    and every odd position bit is swapped with adjacent on left side.

    If we take a closer look at the example,
    we can observe that we basically need to right shift (>>) all even bits
    (In the above example, even bits of 23 are highlighted) by 1 so that they become odd bits
    (highlighted in 43), and left shift (<<) all odd bits by 1 so that they become even bits.

    The following solution is based on this observation.
    The solution assumes that input number is stored using 32 bits.
    Let the input number be x
    1) Get all even bits of x by doing bitwise and of x with 0xAAAAAAAA.
        The number 0xAAAAAAAA is a 32 bit number with all even bits set as 1 and all odd bits as 0.
    2) Get all odd bits of x by doing bitwise and of x with 0x55555555.
        The number 0x55555555 is a 32 bit number with all odd bits set as 1 and all even bits as 0.
    3) Right shift all even bits.
    4) Left shift all odd bits.
    5) Combine new even and odd bits and return.
*/

// Function to swap even
// and odd bits
uint32 swapBits(uint32 x)
{
    // Get all even bits of x
    uint32 even_bits = x & 0xAAAAAAAA;

    // Get all odd bits of x
    uint32 odd_bits  = x & 0x55555555;

    even_bits >>= 1;  // Right shift even bits
    odd_bits <<= 1;   // Left shift odd bits

    return (even_bits | odd_bits); // Combine even and odd bits
}

// Driver program to test above function
int main()
{
    uint32 x = 23; // 00010111
    uint32 result; // 00010111

    dumpBinary(x, "Input");
    result = swapBits(x);
    dumpBinary(result, "Output");
    // Output is 43 (00101011)

    return 0;
}