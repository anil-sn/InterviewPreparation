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
  	Find value of k-th bit in binary representation
		Given a number n and k (1 <= k <= 32), find the value of k-th bit in the binary representation of n. 
		Bits are numbered from right (Least Significant Bit) to left (Most Significant Bit).

	Examples :

		Input : n = 13, k = 2
		Output : 0

	Explanation: Binary representation of 13 is 1101.
		     Second bit from right is 0.

		Input : n = 14, k = 3
		Output : 1
	
	Explanation: Binary representation of 14 is 1110.
		     Third bit from right is 1.
*/

char string[100] = {'\0'};
int pos = 0;

void
printBinary(u_int32_t number) {
    if (number >> 1) {
        printBinary(number >> 1);
    }

    string[pos++] = ((number & 1) ? '1' : '0');
    return;
}

void printKthBit(unsigned int n, unsigned int k)
{
    	pos = 0;
	memset(string, '\0', sizeof(string));
	printBinary(n);
    	printf("%s\r\n", string);

    	pos = 0;
	memset(string, '\0', sizeof(string));
	printBinary((k - 1));
    	printf("%s\r\n", string);

    	pos = 0;
	memset(string, '\0', sizeof(string));
	printBinary((1 << (k - 1)));
    	printf("%s\r\n", string);

    	pos = 0;
	memset(string, '\0', sizeof(string));
	printBinary((n & (1 << (k - 1))));
    	printf("%s\r\n", string);

    printf("is Bit set : %s\r\n", ((n & (1 << (k - 1))) >> (k - 1)) ? "TRUE" : "FALSE");
}

int main() {
    unsigned int n = 13, k = 2;

    // Function Call
    printKthBit(n, k);
    return 0;
}
