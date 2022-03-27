#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

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

int Add(int x, int y)
{
    dumpBinary(x, "x");
    dumpBinary(y, "y");

    // Iterate till there is no carry
    while (y != 0)
    {
        // carry now contains common
        //set bits of x and y
        unsigned carry = x & y;
        dumpBinary(carry, "carry = x & y");

        // Sum of bits of x and y where at
        //least one of the bits is not set
        x = x ^ y;
        dumpBinary(x, "x = x ^ y");

        // Carry is shifted by one so that adding
        // it to x gives the required sum
        y = carry << 1;
        dumpBinary(y, "y = carry << 1");
    }

    return x;
}

int main()
{
    printf("\r\nSum of 15 + 32 = %d \r\n\r\n", Add(15, 32));
    printf("\r\nSum of 115 + 132 = %d \r\n\r\n", Add(115, 132));
    return 0;
}