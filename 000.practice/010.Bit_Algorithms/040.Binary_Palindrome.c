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
    Check if binary representation of a number is palindrome

    Given an integer ‘x’, write a C function that returns true
    if binary representation of x is palindrome else return false.
    For example a numbers with binary representation as 10..01 is palindrome and
    number with binary representation as 10..00 is not palindrome.
    The idea is similar to checking a string is palindrome or not.
    We start from leftmost and rightmost bits and compare bits one by one.
    If we find a mismatch, then return false.
*/

// This function returns true if k'th bit in x
// is set (or 1). For example if x (0010) is 2
// and k is 2, then it returns true
bool isKthBitSet(uint32 x, uint32 k)
{
    return (x & (1 << (k - 1))) ? true : false;
}

// This function returns true if binary
// representation of x is palindrome.
// For example (1000...001) is palindrome
bool isPalindrome(uint32 x)
{
    int l = 1; // Initialize left position
    int r = sizeof(uint32) * 8; // initialize right position

    // One by one compare bits
    while (l < r)
    {
        if (isKthBitSet(x, l) != isKthBitSet(x, r))
            return false;
        l++;
        r--;
    }
    return true;
}

// Driver Code
int main()
{
    uint64 x = 5;

    dumpBinary(x, "Input");
    printf("Is number 0X%llx a palindrome : %s \r\n", x,  isPalindrome(x) ? "TRUE" : "FALSE");

    x = 9;
    dumpBinary(x, "Input");
    printf("Is number 0X%llx a palindrome : %s \r\n", x,  isPalindrome(x) ? "TRUE" : "FALSE");

    return 0;
}