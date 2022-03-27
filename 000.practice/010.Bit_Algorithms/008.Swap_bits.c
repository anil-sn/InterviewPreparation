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

/*
    Swap bits in a given number

    Given a number x and two positions (from the right side) in the binary representation of x,
    write a function that swaps n bits at given two positions and returns the result.
    It is also given that the two sets of bits do not overlap.

    Method 1
    Let p1 and p2 be the two given positions.

    Example 1
        Input:
            x = 47 (00101111)
            p1 = 1 (Start from the second bit from the right side)
            p2 = 5 (Start from the 6th bit from the right side)
            n = 3 (No of bits to be swapped)
        Output:
            227 (11100011)
            The 3 bits starting from the second bit (from the right side) are
            swapped with 3 bits starting from 6th position (from the right side)

    Example 2
        Input:
            x = 28 (11100)
            p1 = 0 (Start from first bit from right side)
            p2 = 3 (Start from 4th bit from right side)
            n = 2 (No of bits to be swapped)
        Output:
            7 (00111)
            The 2 bits starting from 0th position (from right side) are
            swapped with 2 bits starting from 4th position (from right side)

    Solution
    We need to swap two sets of bits. XOR can be used in a similar way as it is used to
    swap 2 numbers.

    Following is the algorithm.

    1) Move all bits of the first set to the rightmost side
        set1 =  (x >> p1) & ((1U << n) - 1)
        Here the expression (1U << n) - 1 gives a number that
        contains last n bits set and other bits as 0. We do &
        with this expression so that bits other than the last
        n bits become 0.
    2) Move all bits of second set to rightmost side
        set2 =  (x >> p2) & ((1U << n) - 1)
    3) XOR the two sets of bits
        xor = (set1 ^ set2)
    4) Put the xor bits back to their original positions.
        xor = (xor << p1) | (xor << p2)
    5) Finally, XOR the xor with original number so
        that the two sets are swapped.
        result = x ^ xor
*/
int swapBits(unsigned int x, unsigned int p1, unsigned int p2, unsigned int n)
{
    dumpBinary(x, "Input");

    /* Move all bits of first set to rightmost side */
    unsigned int set1 = (x >> p1) & ((1U << n) - 1);
    dumpBinary(set1, "set1");

    /* Move all bits of second set to rightmost side */
    unsigned int set2 = (x >> p2) & ((1U << n) - 1);
    dumpBinary(set2, "set2");

    /* XOR the two sets */
    unsigned int xor = (set1 ^ set2);
    dumpBinary(xor, "(set1 ^ set2)");

    /* Put the xor bits back to their original positions */
    xor = (xor << p1) | (xor << p2);
    dumpBinary(xor, "(xor << p1) | (xor << p2)");

    /* XOR the 'xor' with the original number so that the
       two sets are swapped */
    unsigned int result = x ^ xor;
    dumpBinary(result, "x ^ xor");

    return result;
}

/* Driver program to test above function*/
int main()
{
    int res = swapBits(28, 0, 3, 2);
    printf("\nResult = %d \r\n", res);
    return 0;
}