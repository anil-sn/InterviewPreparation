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
    Find whether a given number is a power of 4 or not
    Given an integer n, find whether it is a power of 4 or not.

    Example :
    Input : 16
    Output : 16 is a power of 4

    Input : 20
    Output : 20 is not a power of 4

    A number n is a power of 4 if the following conditions are met.
    a) There is only one bit set in the binary representation of n (or n is a power of 2)
    b) The count of zero bits before the (only) set bit is even.

    For example 16 (10000) is the power of 4 because there is only one bit set and count of 0s
    before the set bit is 4 which is even.

    Decoding (n & (n - 1)) :
    It's figuring out if n is either 0 or an exact power of two.
    It works because a binary power of two is of the form 1000...000 and
    subtracting one will give you 111...111. Then, when you AND those together,
    you get zero, such as with:

      1000 0000 0000 0000
    &  111 1111 1111 1111
      ==== ==== ==== ====
    = 0000 0000 0000 0000

    Any non-power-of-two input value (other than zero) will not give you
    zero when you perform that operation.

    For example, let's try all the 4-bit combinations:

        <----- binary ---->
    n      n    n-1   n&(n-1)
    --   ----   ----   -------
    0    0000   0111    0000 *
    1    0001   0000    0000 *
    2    0010   0001    0000 *
    3    0011   0010    0010
    4    0100   0011    0000 *
    5    0101   0100    0100
    6    0110   0101    0100
    7    0111   0110    0110
    8    1000   0111    0000 *
    9    1001   1000    1000
    10   1010   1001    1000
    11   1011   1010    1010
    12   1100   1011    1000
    13   1101   1100    1100
    14   1110   1101    1100
    15   1111   1110    1110
    You can see that only 0 and the powers of two (1, 2, 4 and 8) result in a 0000
    false bit pattern, all others are non-zero or true.
*/

bool isPowerOfFour(unsigned int n)
{
    int count = 0;

    /* Check if there is only one bit set in n */
    if ( n && !(n & (n-1)) )
    {
        /* count 0 bits before set bit */
        while(n > 1)
        {
            n  >>= 1;
            count += 1;
        }

        /*If count is even then return true else false*/
        return ((count % 2 == 0) ? 1 : 0);
    }

    /* If there are more than 1 bit set then n is not a power of 4*/
    return 0;
}

/*Driver program to test above function*/
int main()
{
    int test_no = 64;

    if(isPowerOfFour(test_no)) {
        printf("%d is a power of 4 \r\n", test_no);
    } else {
        printf("%d is not a power of 4 \r\n", test_no);
    }

    return 0;
}