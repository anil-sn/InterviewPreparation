#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
5.3 Flip Bit to Win: You have an integer and you can flip exactly one bit from a O to a 1. Write code to
                     find the length of the longest sequence of 1 s you could create.
    EXAMPLE
        Input: 1775 (or: 11011101111)
        Output: 8

    Binary representation of 1775 is 11011101111.
    After flipping the highlighted bit, we get
    consecutive 8 bits. 11011111111.

    SOLUTION
        We can think about each integer as being an alternating sequence of Os and 1 s. Whenever a Os sequence
        has length one, we can potentially merge the adjacent 1 s sequences.

    Input : 12
    Output : 3

    Input : 15
    Output : 5

    Input : 71
    Output: 4

    Binary representation of 71 is 1000111.
    After flipping the highlighted bit, we get
    consecutive 4 bits. 1001111.

    A Simple Solution is to traverse the array, for every 0, count the number of 1s on both sides of it.
    Keep track of maximum count for any 0. Finally return index of the 0 with maximum number of 1s around it.
    The time complexity of this solution is O(n2).

    TEST CASE:
    0b000 -> 1
    0b100 -> 1
    0b1011011 -> 5

    ALGORITHM:
    1. Iterate through binary representation by using logical right shift on copy of int
    2. Keep 3 tallys: prev tally, current tally, max tally.
    3. Loop over binary representation:
         a. Count the number of 1s seen so far in current tally.
         b. If a zero is seen:
             i. add 1 to current tally
             ii. set max tally to sum of pre and post tally if the sum is > max tally
             iii. prev tally = current tally
         c. logical shift the input to the right with >>>
    4. In case where binary representation ends in 1, we have to do the check in b.ii. again.
    5. return max tally

    SPACE & TIME:
    Space: O(1): extra space we use does not scale with length of int
    Time: O(b) because we have to iterate thru each bit
*/

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

int flipBitToWin(int input) {

    int prevTally = 0;
    int currentTally = 0;
    int maxTally = 0;

    if (input == 0) return 1; // edge case where we do not need to run the algorithm

    while (input != 0) {
        if ((input & 0b1) == 1) {
            currentTally ++;
        } else {
            int sum = prevTally + currentTally + 1;
            if (sum > maxTally) {
                maxTally = sum;
            }
            prevTally = currentTally;
            currentTally = 0;
        }
        input = input >> 1;
    }

    int sum = prevTally + currentTally + 1;
    if (sum > maxTally) {
        maxTally = sum;
    }

    return maxTally;
}

int main () {

    pos = 0;
    memset(string, '\0', sizeof(string));
    printf("Number of 1s in %s[1775] after flip : %d\r\n", printBinary(1775), flipBitToWin(1775));

    pos = 0;
    memset(string, '\0', sizeof(string));
    printf("Number of 1s in %s[12] after flip : %d\r\n", printBinary(12), flipBitToWin(12));

    pos = 0;
    memset(string, '\0', sizeof(string));
    printf("Number of 1s in %s[15] after flip : %d\r\n", printBinary(15), flipBitToWin(15));

    pos = 0;
    memset(string, '\0', sizeof(string));
    printf("Number of 1s in %s[71] after flip : %d\r\n", printBinary(71), flipBitToWin(71));

    return;
}
