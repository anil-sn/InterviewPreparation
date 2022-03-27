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
void bin(int n, char reason[]) {
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
    Given a positive integer n, count the total number of set bits in binary
    representation of all numbers from 1 to n.

    Examples:

    Input: n = 3
    Output:  4

    Input: n = 6
    Output: 9

    Input: n = 7
    Output: 12

    Input: n = 8
    Output: 13

    Method (Simple and efficient than Method 1)
    If we observe bits from rightmost side at distance i than bits get inverted after 2^i position in vertical sequence.
    for example n = 5;
    0 = 0000
    1 = 0001
    2 = 0010
    3 = 0011
    4 = 0100
    5 = 0101
    Observe the right most bit (i = 0) the bits get flipped after (2^0 = 1)
    Observe the 3rd rightmost bit (i = 2) the bits get flipped after (2^2 = 4)
    So, We can count bits in vertical fashion such that at i’th right most position bits will be get flipped after 2^i iteration;
*/

// Function which counts set bits from 0 to n
int countSetBits(int n)
{
    int i = 0;

    // ans store sum of set bits from 0 to n
    int ans = 0;

    // while n greater than equal to 2^i
    while ((1 << i) <= n) {

        // This k will get flipped after 2^i iterations
        bool k = 0;

        // change is iterator from 2^i to 1
        int change = 1 << i;

        // This will loop from 0 to n for every bit position
        for (int j = 0; j <= n; j++) {

            ans += k;

            if (change == 1) {
                k = !k; // When change = 1 flip the bit
                change = 1 << i; // again set change to 2^i
            }
            else {
                change--;
            }
        }

        // increment the position
        i++;
    }

    return ans;
}

// Main Function
int main()
{
    int n = 17;
    printf("Number of bits set from 1 to %d is %d\r\n",n,countSetBits(n));
    return 0;
}