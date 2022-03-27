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
    Find position of the only set bit

    Given a number N having only one ‘1’ and all other ’0’s in its binary representation,
    find position of the only set bit. If there are 0 or more than 1 set bit the answer should be -1.
    Position of set bit ‘1’ should be counted starting with 1 from the LSB side in the binary
    representation of the number.

    Examples:-

    Input: N = 2
    Output: 2
    Explanation:
    2 is represented as "10" in Binary. As we see there's only one set bit and it's in Position 2

    Input: N = 5
    Output: -1
    Explanation:
    5 is represented as "101" in Binary. As we see there's two set bits and thus the Output -1.
*/

// A utility function to check whether n is power of 2 or not
uint32 isPowerOfTwo(uint32 n)
{
    return n && (!(n & (n - 1)));
}

// Returns position of the only set bit in 'n'
uint32 findPosition(uint32 n)
{
    dumpBinary(n, "Input");
    if (!isPowerOfTwo(n))
        return -1;

    uint32 count = 0;

    // One by one move the only set bit to right till it reaches end
    while (n) {
        n = n >> 1;

        // increment count of shifts
        ++count;
    }

    return count;
}

// Driver program to test above function
uint32 main(void)
{
    uint32 n = 0;
    uint32 pos = findPosition(n);
    (pos == -1) ? printf("n = %d, Invalid number\n", n) : printf("n = %d, Position %d \n", n, pos);

    n = 12;
    pos = findPosition(n);
    (pos == -1) ? printf("n = %d, Invalid number\n", n) : printf("n = %d, Position %d \n", n, pos);

    n = 128;
    pos = findPosition(n);
    (pos == -1) ? printf("n = %d, Invalid number\n", n) : printf("n = %d, Position %d \n", n, pos);

    return 0;
}