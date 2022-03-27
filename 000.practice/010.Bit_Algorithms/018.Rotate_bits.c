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
    Rotate bits of a number

    Bit Rotation: A rotation (or circular shift) is an operation similar to shift except that
    the bits that fall off at one end are put back to the other end.
    In left rotation, the bits that fall off at left end are put back at right end.
    In right rotation, the bits that fall off at right end are put back at left end.

    Example:
    Let n is stored using 8 bits. Left rotation of n = 11100101 by 3 makes n = 00101111
    (Left shifted by 3 and first 3 bits are put back in last ). If n is stored using 16 bits or 32 bits
    then left rotation of n (000…11100101) becomes 00..0011100101000.

    Right rotation of n = 11100101 by 3 makes n = 10111100 (Right shifted by 3 and last 3 bits
    are put back in first ) if n is stored using 8 bits. If n is stored using 16 bits or 32 bits
    then right rotation of n (000…11100101) by 3 becomes 101000..0011100.
*/

#define INT_BITS 32

/*Function to left rotate n by d bits*/
int leftRotate(int n, unsigned int d)
{
    /* In n<<d, last d bits are 0. To put first 3 bits of n at
       last, do bitwise or of n<<d with n >>(INT_BITS - d) */
    return (n << d)|(n >> (INT_BITS - d));
}

/*Function to right rotate n by d bits*/
int rightRotate(int n, unsigned int d)
{
    /* In n>>d, first d bits are 0. To put last 3 bits of at
       first, do bitwise or of n>>d with n <<(INT_BITS - d) */
    return (n >> d)|(n << (INT_BITS - d));
}

/* Driver program to test above functions */
int main()
{
    int n = 7;
    int d = 2;
    int result = 0;

    dumpBinary(n, "Input");

    printf("Left Rotation of %d by %d is ", n, d);
    result = leftRotate(n, d);
    printf("%d \r\n", result);
    dumpBinary(result, "left Rotate by 2");

    printf("\nRight Rotation of %d by %d is ", n, d);
    result = rightRotate(n, d);
    printf("%d \r\n", result);
    dumpBinary(result, "right Rotate by 2");

    return 0;
}