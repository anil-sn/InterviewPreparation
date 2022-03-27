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
    Write an Efficient Method to Check if a Number is Multiple of 3

    The very first solution that comes to our mind is the one that we learned in school.
    If sum of digits in a number is multiple of 3 then number is multiple of 3
    e.g., for 612 sum of digits is 9 so it’s a multiple of 3.
    But this solution is not efficient. You have to get all decimal digits one by one,
    add them and then check if sum is multiple of 3.

    There is a pattern in binary representation of the number that can be used to find
    if number is a multiple of 3. If difference between count of odd set bits
    (Bits set at odd positions) and even set bits is multiple of 3 then is the number.
*/

int static dp[1001];

/* Function to check if n is a multiple of 3*/
int isMultipleOf3(int n)
{
    int odd_count = 0;
    int even_count = 0;

    // Base Cases
    if (n < 0)
        n = -n;

    if (n == 0)
        return 1;

    if (n == 1)
        return 0;

    // If a value is already present
    // in dp, return it
    if(dp[n] != -1)
        return dp[n];

    dumpBinary(n, "Recursive Input");
    while (n) {
        /* If odd bit is set then
        increment odd counter */
        if (n & 1)
            odd_count++;

        /* If even bit is set then
        increment even counter */
        if (n & 2)
            even_count++;
        n = n >> 2;
    }

    printf("Odd Bits count : %d, Even Bits count %d \r\n", odd_count, even_count);
    dp[n] = isMultipleOf3(abs(odd_count - even_count));

    // return dp
    return dp[n];
}

/* Program to test function isMultipleOf3 */
int main()
{
    int num = 980;

    memset(dp, -1, sizeof(dp));

    dumpBinary(num, "Input");
    if (isMultipleOf3(num))
        printf("%d is multiple of 3 \r\n", num);
    else
        printf("%d is not a multiple of 3 \r\n", num);
    return 0;
}