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
    Program to find whether a given number is power of 2

    Given a positive integer, write a function to find if it is a power of two or not.
    Examples :
        Input : n = 4
        Output : Yes
        22 = 4

        Input : n = 7
        Output : No

        Input : n = 32
        Output : Yes
        25 = 32

    If we subtract a power of 2 numbers by 1 then all unset bits after the only set bit
    become set; and the set bit becomes unset.
    For example for 4 ( 100) and 16(10000), we get the following after subtracting 1
    3 –> 011
    15 –> 01111

    So, if a number n is a power of 2 then bitwise & of n and n-1 will be zero.
    We can say n is a power of 2 or not based on the value of n&(n-1).
    The expression n&(n-1) will not work when n is 0.
    To handle this case also, our expression will become n& (!n&(n-1))

    Below is the implementation of this method.
    Time complexity : O(1)
    Space complexity : O(1)
*/

/* Function to check if x is power of 2*/
bool isPowerOfTwo (int x)
{
    /* First x in the below expression is for the case when x is 0 */
    return x && (!(x&(x-1)));
}

/*Driver program to test above function*/
int main()
{
    printf("Is 31 power of 2 : ");
    isPowerOfTwo(31)? printf("Yes\r\n"): printf("No\r\n");

    printf("Is 64 power of 2 : ");
    isPowerOfTwo(64)? printf("Yes\r\n"): printf("No\r\n");
    return 0;
}