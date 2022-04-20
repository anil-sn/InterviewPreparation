#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "sys/types.h"


/*
   Palindrome Permutation: Given a string, write a function to check if it is a permutation of
   a palindrome. A palindrome is a word or phrase that is the same forwards and backwards. A
   permutation is a rearrangement of letters. The palindrome does not need to be limited to just
   dictionary words.

   EXAMPLE
Input: Tact Coa
Output: True (permutations: "taco cat'; "atc o eta; etc.)

Solution
we don't actually need to know the counts. We just need to know if the count is even or odd.
Think about flipping a light on/off (that is initially off). If the light winds up in the off state,
we don't know how many times we flipped it, but we do know it was an even count.
Given this, we can use a single integer (as a bit vector). When we see a letter, we map it to an integer
between O and 26 (assuming an English alphabet). Then we toggle the bit at that value. At the end of the
iteration, we check that at most one bit in the integer is set to 1.
We can easily check that no bits in the integer are 1: just compare the integer to 0. There is actually a very
elegant way to check that an integer has exactly one bit set to 1.

Picture an integer like 00010000. We could of course shift the integer repeatedly to check that there's only
a single 1. Alternatively, if we subtract 1 from the number, we'll get 00001111. What's notable about this
is that there is no overlap between the numbers (as opposed to say 00101000, which, when we subtract 1
from, we get 00100111.) So, we can check to see that a number has exactly one 1 because if we subtract 1
from it and then AND it with the new number, we should get 0.

00010000 - 1 = 00001111
00010000 & 00001111 = 0
 */

/* Toggle the ith bit in the integer. */
int toggle(int bitVector, int index) {
    if (index < 0) {
        return bitVector;
    }

    int mask= 1 << index;

    if ((bitVector & mask) == 0) {
        bitVector |= mask;
    } else {
        bitVector &= ~mask;
    }

    return bitVector;
}

/* Check that exactly one bit is set by subtracting one from the integer and ANDing it with the original integer. */
bool checkExactlyOneBitSet(int bitVector) {
    return (bitVector & (bitVector - 1)) == 0;
}

/* Create a bit vector for the string. For each letter with value i, toggle the * ith bit. */
int createBitVector(char* phrase) {
    int32_t bitVector = 0;
    int16_t i = 0, x;

    for (i = 0; i < strlen(phrase); i++) {
        x = phrase[i];
        bitVector = toggle(bitVector, x);
    }

    return bitVector;
}

bool isPermutationOfPalindrome(char * phrase) {

    int bitVector = createBitVector(phrase);

    return (bitVector == 0 || checkExactlyOneBitSet(bitVector));
}

int main () {

    u_int16_t i = 0;
    int32_t bitmap = 0;

    char in[100] = {'\0'};

    printf ("Enter Sentance to Be Test :\r\n");
    scanf ("%[^\n]s", in);

    if (isPermutationOfPalindrome(in)) {
        printf ("The String can form a palindrome\r\n");
    } else {
        printf ("The String cannot form a palindrome\r\n");
    }

    return 0;
}
