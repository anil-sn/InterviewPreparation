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
    5.6 Conversion: Write a function to determine the number of bits you would need to flip to convert integer A to integer B.

    EXAMPLE
        Input: 29 (or: 11101), 15 (or: 01111)
        Output: 2

    SOLUTION
    This seemingly complex problem is actually rather straightforward. To approach this, ask yourself how you
    would figure out which bits in two numbers are different. Simple: with an XOR.

*/

int conversion_brute_force(int A, int B){
    int count = 0;
    while(A > 0 || B > 0) {
        if(A & 1) {
            if (!(B & 1)){
                count ++;
            }
        } else {
            if (B & 1){
                count ++;
            }
        }
        A >>= 1;
        B >>= 1;
    }
    return count;
}

int bitSwapRequired_1(int a, int b) {
    int count = 0;
    for (int c = a ^ b; c != 0; c = c >> 1) {
        count+= c & 1;
    }

    return count;
}

int bitSwapRequired(int a, int b) {
    int count = 0;
    for (int c = a ^ b; c != 0; c = c & (c-1)) {
        count++;
    }

    return count;
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
    printf("Integer A %s[%d] ", printBinary(29), 29);

    pos = 0;
    memset(string, '\0', sizeof(string));
    printf("Integer B %s[%d] \r\n", printBinary(15), 15);

    printf ("Brute Force : %d \r\n", conversion_brute_force(29, 15));

    printf ("XOR Shift : %d \r\n",bitSwapRequired_1(29, 15));

    printf ("XOR Decrement %d \r\n", bitSwapRequired(29, 15));

    return;
}
