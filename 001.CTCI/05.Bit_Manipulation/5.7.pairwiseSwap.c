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
    5.7 Pairwise Swap Write a program to swap odd and even bits in an integer with as few instructions as possible
            (e.g., bit O and bit 1 are swapped, bit 2 and bit 3 are swapped, and so on).

    SOLUTION
        Like many of the previous problems, it's useful to think about this problem in a different way. Operating on
        individual pairs of bits would be difficult, and probably not that efficient either. So how else can we think
        about this problem?
        We can approach this as operating on the odds bits first, and then the even bits. Can we take a number n
        and move the odd bits over by 1? Sure. We can mask all odd bits with 10101010 in binary (which is 0xAA),

        then shift them right by 1 to put them in the even spots. For the even bits, we do an equivalent operation.
        Finally, we merge these two values. This takes a total of five instructions. The code below implements this approach.
        int swapOddEvenBits(int x) {
            return ( ((x & 0xaaaaaaaa) >>> 1) I ((x & 0x55555555) << 1) );
        }
        Note that we use the logical right shift, instead of the arithmetic right shift. This is because we want the sign
        bit to be filled with a zero.
        We've implemented the code above for 32-bit integers in Java. If you were working with 64-bit integers, you
        would need to change the mask. The logic, however, would remain the same.

*/

u_int32_t pairwiseSwap(u_int32_t x){
    //const int even_mask_1010 = 0b10101010101010101010101010101010;
    //const int odd_mask_0101 = 0b01010101010101010101010101010101;
    return ( ((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1) );
    //return ((x & even_mask_1010) >> 1 | (x & odd_mask_0101) << 1);
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
    printf("Pairwise Swap \r\nInput  : %s[%d] \r\n", printBinary(14), (14));

    pos = 0;
    memset(string, '\0', sizeof(string));
    temp = pairwiseSwap((14));
    printf ("Output : %s[%d] \r\n", printBinary(temp), temp);
    return;
}

