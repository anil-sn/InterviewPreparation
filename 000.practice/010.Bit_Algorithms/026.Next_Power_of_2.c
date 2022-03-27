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
    Smallest power of 2 greater than or equal to n

    Write a function that, for a given no n, finds a number p which is greater than or
    equal to n and is a smallest power of 2.

    Examples :

        Input : n = 5
        Output: 8

        Input  : n = 17
        Output : 32

        Input  : n = 32
        Output : 32

    1. Subtract n by 1
       n = n -1
    2. Set all bits after the leftmost set bit.
        // Below solution works only if integer is 32 bits
                    n = n | (n >> 1);
                    n = n | (n >> 2);
                    n = n | (n >> 4);
                    n = n | (n >> 8);
                    n = n | (n >> 16);
    3. Return n + 1

    Example :

    Steps 1 & 3 of above algorithm are to handle cases
    of power of 2 numbers e.g., 1, 2, 4, 8, 16,

        Let us try for 17(10001)
        step 1
        n = n - 1 = 16 (10000)
        step 2
        n = n | n >> 1
        n = 10000 | 01000
        n = 11000
        n = n | n >> 2
        n = 11000 | 00110
        n = 11110
        n = n | n >> 4
        n = 11110 | 00001
        n = 11111
        n = n | n >> 8
        n = 11111 | 00000
        n = 11111
        n = n | n >> 16
        n = 11110 | 00000
        n = 11111

        step 3: Return n+1
        We get n + 1 as 100000 (32)
*/

// Finds next power of two
// for n. If n itself is a
// power of two then returns n
uint32 nextPowerOf2(uint32 n)
{
    dumpBinary(n-1, "Step1 n--");
    n--;

    dumpBinary((n|n >> 1), "Step2 (n|n >> 1)");
    n |= n >> 1;

    dumpBinary((n|n >> 2), "Step3 (n|n >> 2)");
    n |= n >> 2;

    dumpBinary((n|n >> 4), "Step4 (n|n >> 4)");
    n |= n >> 4;

    dumpBinary((n|n >> 8), "Step5 (n|n >> 8)");
    n |= n >> 8;

    dumpBinary((n|n >> 16), "Step6 (n|n >> 16)");
    n |= n >> 16;

    dumpBinary((n+1), "Step7 (n++)");
    n++;

    return n;
}

// Driver Code
int main()
{
    uint32 n = 0xa0a0a0;
    uint32 result = 0;

    printf("Next power of two for 0X%x\r\n",n);
    dumpBinary(n, "Input");
    result = nextPowerOf2(n);
    dumpBinary(result, "result");
    printf("Next power of two for 0X%x is : 0X%x \r\n",n, result);
    return 0;
}