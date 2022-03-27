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
    Compute the minimum or maximum of two integers without branching
    On some rare machines where branching is expensive, the below obvious approach to find minimum
    can be slow as it uses branching.

    Below are the methods to get minimum(or maximum) without using branching.
    Typically, the obvious approach is best, though.

    Method 1(Use XOR and comparison operator)

    Minimum of x and y will be : y ^ ((x ^ y) & -(x < y))

    It works because if x < y, then -(x < y) will be -1 which is all ones(11111….),
    so r = y ^ ((x ^ y) & (111111…)) = y ^ x ^ y = x.

    And if x>y, then-(x<y) will be -(0) i.e -(zero) which is zero,
    so r = y^((x^y) & 0) = y^0 = y.

    On some machines, evaluating (x < y) as 0 or 1 requires a branch instruction,
    so there may be no advantage.  find the maximum, use : x ^ ((x ^ y) & -(x < y));
*/

/*Function to find minimum of x and y*/
int min(int x, int y)
{
    return y ^ ((x ^ y) & -(x < y));
}

/*Function to find maximum of x and y*/
int max(int x, int y)
{
    return x ^ ((x ^ y) & -(x < y));
}

/* Driver program to test above functions */
int main()
{
    int x = 15;
    int y = 6;
    printf("Minimum of %d and %d is : %d \r\n", x, y, min(x, y));
    printf("Maximum of %d and %d is : %d \r\n", x, y, max(x, y));

    return 0;
}