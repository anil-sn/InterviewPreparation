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
    Next higher number with same number of set bits

    Given a number x, find next number with same number of 1 bits in it’s binary representation.
    For example, consider x = 12, whose binary representation is 1100
    (excluding leading zeros on 32 bit machine).
    It contains two logic 1 bits.
    The next higher number with two logic 1 bits is 17 (10001).

    Algorithm:
    When we observe the binary sequence from 0 to 2n – 1 (n is # of bits),
    right most bits (least significant) vary rapidly than left most bits.
    The idea is to find right most string of 1’s in x, and shift the pattern to right extreme,
    except the left most bit in the pattern.
    Shift the left most bit in the pattern (omitted bit) to left part of x by one position.

    An example makes it more clear,
    x = 156
    x = 10011100

    10011100
    00011100 - right most string of 1's in x
    00000011 - right shifted pattern except left most bit ------> [A]
    00010000 - isolated left most bit of right most 1's pattern
    00100000 - shiftleft-ed the isolated bit by one position ------> [B]
    10000000 - left part of x, excluding right most 1's pattern ------> [C]
    10100000 - add B and C (OR operation) ------> [D]
    10100011 - add A and D which is required number 163

    After practicing with few examples, it easy to understand.
    Use the below given program for generating more sets.

    Program Design:
    We need to note few facts of binary numbers.
    The expression x & -x will isolate right most set bit in x
    (ensuring x will use 2’s complement form for negative numbers).
    If we add the result to x, right most string of 1’s in x will be reset,
    and the immediate ‘0’ left to this pattern of 1’s will be set,
    which is part [B] of above explanation.

    For example if x = 156, x & -x will result in 00000100,
    adding this result to x yields 10100000 (see part D).
    We left with the right shifting part of pattern of 1’s (part A of above explanation).

    There are different ways to achieve part A.
    Right shifting is essentially a division operation.
    What should be our divisor? Clearly, it should be multiple of 2 (avoids 0.5 error in right shifting),
    and it should shift the right most 1’s pattern to right extreme.
    The expression (x & -x) will serve the purpose of divisor.
    An EX-OR operation between the number X and expression which is used to reset right most bits,
    will isolate the rightmost 1’s pattern.

    A Correction Factor:
    Note that we are adding right most set bit to the bit pattern.
    The addition operation causes a shift in the bit positions.
    The weight of binary system is 2, one shift causes an increase by a factor of 2.
    Since the increased number (rightOnesPattern in the code) being used twice,
    the error propagates twice. The error needs to be corrected.
    A right shift by 2 positions will correct the result.
    The popular name for this program is same number of one bits.
*/

// this function returns next higher number with same number of set bits as x.
u_int32_t snoob(u_int32_t x)
{

  u_int32_t rightOne;
  u_int32_t nextHigherOneBit;
  u_int32_t rightOnesPattern;

  u_int32_t next = 0;

  if(x) {

    // right most set bit
    rightOne = x & -(signed)x;

    // reset the pattern and set next higher bit
    // left part of x will be here
    nextHigherOneBit = x + rightOne;

    // nextHigherOneBit is now part [D] of the above explanation.

    // isolate the pattern
    rightOnesPattern = x ^ nextHigherOneBit;

    // right adjust pattern
    rightOnesPattern = (rightOnesPattern)/rightOne;

    // correction factor
    rightOnesPattern >>= 2;

    // rightOnesPattern is now part [A] of the above explanation.

    // integrate new pattern (Add [D] and [A])
    next = nextHigherOneBit | rightOnesPattern;
  }

  return next;
}

int main()
{
  int x = 156;
  printf("Next higher number of %d with same number of set bits is %d\r\n", x, snoob(x));
  return 0;
}