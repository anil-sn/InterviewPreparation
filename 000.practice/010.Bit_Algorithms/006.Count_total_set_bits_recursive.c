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
Method (Recursive)

    Approach:

    For each number ‘n’, there will we a number a, a<=n and a is perfect power of two,
    like 1,2,4,8…..

    Let n = 11, now we can see that

    Numbers till n, are:
    0  -> 0000000
    1  -> 0000001
    2  -> 0000010
    3  -> 0000011
    4  -> 0000100
    5  -> 0000101
    6  -> 0000110
    7  -> 0000111
    8  -> 0001000
    9  -> 0001001
    10 -> 0001010
    11 -> 0001011

    Now we can see that, from 0 to pow(2,1)-1 = 1, we can pair elements top-most
    with bottom-most, and count of set bit in a pair is 1

    Similarly for pow(2,2)-1 = 4, pairs are:
    00 and 11
    01 and 10
    here count of set bit in a pair is 2, so in both pairs is 4

    Similarly we can see for 7, 15, ans soon.....

    so we can generalise that,
    count(x) = (x*pow(2,(x-1))),
    here x is position of set bit of the largest power of 2 till n
    for n = 8, x = 3
    for n = 4, x = 2
    for n = 5, x = 2

    so now for n = 11,
    we have added set bits count from 0 to 7 using count(x) = (x*pow(2,(x-1)))

    for rest numbers 8 to 11, all will have a set bit at 3rd index, so we can add
    count of rest numbers to our ans,
    which can be calculated using 11 - 8 + 1 = (n-pow(2,x) + 1)

    Now if notice that, after removing front bits from rest numbers,
    we get again number from 0 to some m
    so we can recursively call our same function for next set of numbers,
    by calling countSetBits(n - pow(2,x))
    8  -> 1000  -> 000 -> 0
    9  -> 1001  -> 001 -> 1
    10 -> 1010  -> 010 -> 2
    11 -> 1011  -> 011 -> 3
*/

int findLargestPower(int n)
{
    int x = 0;
    while ((1 << x) <= n)
        x++;
    return x - 1;
}

int countSetBits(int n)
{
    if (n <= 1)
        return n;
    int x = findLargestPower(n);
    return (x * pow(2, (x - 1))) + (n - pow(2, x) + 1) + countSetBits(n - pow(2, x));
}

int main()
{
    int n = 17;
    printf("Number of bits set from 1 to %d is %d\r\n",n,countSetBits(n));
    return 0;
}