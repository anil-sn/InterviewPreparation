#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

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
    printf(": %s ", reason);
    printf("\r\n");
}

/*
    Find the element that appears once

    Given an array where every element occurs three times, except one element which occurs only once.
    Find the element that occurs once. The expected time complexity is O(n) and O(1) extra space.

    Examples:

    Input: arr[] = {12, 1, 12, 3, 12, 1, 1, 2, 3, 3}
    Output: 2
    In the given array all element appear three times except 2 which appears once.

    Input: arr[] = {10, 20, 10, 30, 10, 30, 30}
    Output: 20
    In the given array all element appear three times except 20 which appears once.

    We can use sorting to do it in O(nLogn) time.
    We can also use hashing, it has the worst-case time complexity of O(n), but requires extra space.
    The idea is to use bitwise operators for a solution that is O(n) time and uses O(1) extra space. The solution is not easy like other XOR-based solutions, because all elements appear an odd number of times here. The idea is taken from here.

    Run a loop for all elements in the array.
    At the end of every iteration, maintain the following two values.
    ones: The bits that have appeared 1st time or 4th time or 7th time .. etc.
    twos: The bits that have appeared 2nd time or 5th time or 8th time .. etc.

    Finally, we return the value of ‘ones’

    How to maintain the values of ‘ones’ and ‘twos’?
    ‘ones’ and ‘twos’ are initialized as 0.
    For every new element in the array, find out the common set bits in the new element and
    the previous value of ‘ones’. These common set bits are actually the bits that should be
    added to ‘twos’. So do bitwise XOR of the common set bits with ‘twos’. ‘twos’ also gets
    some extra bits that appear the third time. These extra bits are removed later.
    Update ‘ones’ by doing XOR of new element with the previous value of ‘ones’.
    There may be some bits that appear 3rd time. These extra bits are also removed later.
    Both ‘ones’ and ‘twos’ contain those extra bits which appear 3rd time.
    Remove these extra bits by finding out common set bits in ‘ones’ and ‘twos’.
*/

int getSingle(int arr[], int n)
{
    int ones = 0;
    int twos = 0;
    int common_bit_mask = 0;

    // Let us take the example of {3, 3, 2, 3} to understand this
    for (int i = 0; i < n; i++) {

        printf("\r\n");
        bin(arr[i], "<< New Item arr[i]");

        /* The expression "one & arr[i]" gives the bits that are
           there in both 'ones' and new element from arr[].  We
           add these bits to 'twos' using bitwise OR
           Value of 'twos' will be set as 0, 3, 3 and 1 after 1st,
           2nd, 3rd and 4th iterations respectively */
        twos = twos | (ones & arr[i]);
        bin(twos, "twos = twos | (ones & arr[i]);");

        /* XOR the new bits with previous 'ones' to get all bits
           appearing odd number of times
           Value of 'ones' will be set as 3, 0, 2 and 3 after 1st,
           2nd, 3rd and 4th iterations respectively */
        ones = ones ^ arr[i];
        bin(ones, "ones = ones ^ arr[i]");

        /* The common bits are those bits which appear third time
           So these bits should not be there in both 'ones' and 'twos'.
           common_bit_mask contains all these bits as 0, so that the bits can
           be removed from 'ones' and 'twos'
           Value of 'common_bit_mask' will be set as 00, 00, 01 and 10
           after 1st, 2nd, 3rd and 4th iterations respectively */
        common_bit_mask = ~(ones & twos);
        bin(common_bit_mask, "common_bit_mask = ~(ones & twos)");

        /* Remove common bits (the bits that appear third time) from 'ones'
           Value of 'ones' will be set as 3, 0, 0 and 2 after 1st,
           2nd, 3rd and 4th iterations respectively */
        ones &= common_bit_mask;
        bin(ones, "ones &= common_bit_mask");

        /* Remove common bits (the bits that appear third time) from 'twos'
           Value of 'twos' will be set as 0, 3, 1 and 0 after 1st,
           2nd, 3rd and 4th iterations respectively */
        twos &= common_bit_mask;
        bin(twos, "twos &= common_bit_mask");
        printf("\r\n");
    }

    return ones;
}

int main()
{
    int arr[] = { 1, 2, 3, 1, 2, 3, 4 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("The element with single occurrence is %d \r\n", getSingle(arr, n));
    return 0;
}