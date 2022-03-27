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
    Count number of bits to be flipped to convert A to B

    Given two numbers ‘a’ and b’. Write a program to count number of bits needed to be
    flipped to convert ‘a’ to ‘b’.

    Example :
    Input : a = 10, b = 20
    Output : 4
    Binary representation of a is 00001010
    Binary representation of b is 00010100
    We need to flip highlighted four bits in a to make it b.

    Input : a = 7, b = 10
    Output : 3
    Binary representation of a is 00000111
    Binary representation of b is 00001010
    We need to flip highlighted three bits in a
    to make it b.

    1. Calculate XOR of A and B.
            a_xor_b = A ^ B
    2. Count the set bits in the above
        calculated XOR result.
            countSetBits(a_xor_b)
    XOR of two number will have set bits only at those places where A differs from B.
*/

// Function that count set bits
int countSetBits(int n)
{
    int count = 0;
    while (n > 0)
    {
        count++;
        n &= (n-1);
    }
    return count;
}

// Function that return count of
// flipped number
int FlippedCount(int a, int b)
{
    // Return count of set bits in
    // a XOR b
    return countSetBits(a^b);
}

// Driver code
int main()
{
    int a = 10;
    int b = 20;
    dumpBinary(a, "Input A");
    dumpBinary(b, "Input B");
    printf("Number of bits to be flipped to make 0X%x to 0X%x is : %d\r\n",a, b, FlippedCount(a, b));
    return 0;
}