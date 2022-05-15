#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
     PROBLEM:
         Given a positive integer, return the next smallest and next largest
         numbers that have the same number of 1s in their binary representations.

     TEST CASES:
     getPrev(0b0000) -> -1
     getNext(0b0000) -> -1
     getPrev(0b1000) -> 0b0100
     getNext(0b1000) -> 0b10000
     getPrev(0b0001) -> -1
     getNext(0b0001) -> 0b0010
     getPrev(0b0101) -> 0b0011
     getNext(0b0101) -> 0b0110

     ALGORITHM:
     getNext():
         1. Use bit shifting to find the least significant 1.
         2. Use bit shifting to find the least significant 0 *to the left of* the least significant 1
         3. Flip the 0 position to 1.
         4. Clear all 1s to the right of the flipped bit.
         5. Add in zeroIndex - oneIndex - 1 ones in the *least significant* positions that have been cleared out

     getPrev():
         1. Use bit shifting to find least significant 0.
         2. Use bit shifting to find least significant 1 *to the left of* the least significant 0.
         3. Flip the 1 position to 0.
         4. Clear all 1s to the right of the flipped bit.
         5. Add in zeroIndex + 1 ones in the *most significant* positions that have been cleared out.

     SPACE & TIME:
     Space: O(b) - we need to allocate masks the same size as the input
     Time: O(b) - we need to iterate through the binary representation bit by bit

*/


u_int32_t getNext(u_int32_t positiveInteger) {

    u_int32_t oneIndex = 0;   // index of the rightmost 1
    u_int32_t zeroIndex = 0;  // index of the rightmost 0 that has 1s to the right of it

    if (positiveInteger == 0 || positiveInteger == UINT32_MAX) return (u_int32_t)(-1);

    // find rightmost (i.e. least significant) "1" in bit sequence
    u_int32_t positiveIntegerCopy = positiveInteger;
    while (positiveIntegerCopy != 0 && (positiveIntegerCopy & 1) != 1) {
        oneIndex ++;
        positiveIntegerCopy >>= 1;
    }

    // find rightmost non-trailing "0" in bit sequence that is more significant than "1" position
    positiveIntegerCopy = positiveInteger >> (oneIndex + 1);  // for zero to be non-trailing, we need to right shift before checking
    zeroIndex = oneIndex + 1;
    while (positiveIntegerCopy != 0 && (positiveIntegerCopy & 1) != 0) {
        zeroIndex ++;
        positiveIntegerCopy >>= 1;
    }

    // flip the zeroIndex bit to 1
    positiveInteger = positiveInteger | (1 << zeroIndex);

    // clear all bits to the right of zeroIndex
    positiveInteger = positiveInteger & (UINT32_MAX << zeroIndex);

    // add in zeroIndex - oneIndex - 1 number of 1s to the end of the number (this accounts for examples like
    // 0b11011001111100 where zeroIndex and oneIndex are separated by ones that need to be added in least significant
    // places.
    positiveInteger = positiveInteger | ((1 << (zeroIndex - oneIndex - 1)) - 1);
    return positiveInteger;
}

u_int32_t getPrev(u_int32_t positiveInteger) {
    u_int32_t oneIndex = 0;   // index of the rightmost 1
    u_int32_t zeroIndex = 0;  // index of the rightmost 0 that has 1s to the right of it

    if (positiveInteger == 0 || positiveInteger >= UINT32_MAX) return (u_int32_t)(-1);

    // find the least significant zero
    u_int32_t positiveIntegerCopy = positiveInteger;
    while (positiveIntegerCopy != 0 && (positiveIntegerCopy & 1) != 0) {
        zeroIndex ++;
        positiveIntegerCopy >>= 1;
    }

    // find the least significant 1 *to the left of* the least significant zero
    positiveIntegerCopy = positiveInteger >> (zeroIndex + 1);
    if (positiveIntegerCopy == 0) return (u_int32_t)(-1);  // check for 0b0...001111 edge case
    oneIndex += zeroIndex + 1;  // account for the right shift before computing one index
    while (positiveIntegerCopy != 0 && (positiveIntegerCopy & 1) != 1) {
        oneIndex ++;
        positiveIntegerCopy >>= 1;
    }

    // flip the oneIndex bit to 0
    positiveInteger = positiveInteger & (~(1 << oneIndex));

    // clear all the bits to the right of oneIndex
    positiveInteger = positiveInteger & (UINT32_MAX << oneIndex);

    // add in zeroIndex + 1 number of ones in most significant positions to the right of oneIndex
    u_int32_t ones = (1 << (zeroIndex + 1)) - 1;
    ones <<= (oneIndex - zeroIndex - 1);
    positiveInteger |= ones;
    return positiveInteger;
}

int pos = 0;
char string[100] = {'\0'};

char*
printBinary(u_int32_t number) {
    if (number >> 1) {
        printBinary(number >> 1);
    }

    string[pos++] = ((number & 1) ? '1' : '0');
    return string;
}

int main () {

    u_int32_t temp = 0;

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = getPrev(0b0000);
    printf("Get Prev of 0b0000[%d] is %s[%d]\r\n", 0b0000, printBinary(temp), temp);

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = getNext(0b0000);
    printf("Get Next of 0b0000[%d] is %s[%d]\r\n", 0b1000, printBinary(temp), temp);

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = getPrev(0b1000);
    printf("Get Prev of 0b1000[%d] is %s[%d]\r\n", 0b1000, printBinary(temp), temp);

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = getNext(0b1000);
    printf("Get Next of 0b1000[%d] is %s[%d]\r\n", 0b1000, printBinary(temp), temp);

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = getPrev(0b0001);
    printf("Get Prev of 0b1000[%d] is %s[%d]\r\n", 0b0001, printBinary(temp), temp);

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = getNext(0b0001);
    printf("Get Next of 0b1000[%d] is %s[%d]\r\n", 0b0001, printBinary(temp), temp);

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = getPrev(0b0101);
    printf("Get Prev of 0b0101[%d] is %s[%d]\r\n", 0b0101, printBinary(temp), temp);

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = getNext(0b0101);
    printf("Get Next of 0b0101[%d] is %s[%d]\r\n", 0b0101, printBinary(temp), temp);

    return;
}
