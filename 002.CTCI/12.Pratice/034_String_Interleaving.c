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
Given three strings A, B and C. Write a function that checks whether C is an interleaving of A and B. C is said to be interleaving A and B, if it contains all and only characters of A and B and order of all characters in individual strings is preserved.

Example:

    Input: strings: "XXXXZY", "XXY", "XXZ"
    Output: XXXXZY is interleaved of XXY and XXZ
            The string XXXXZY can be made by
            interleaving XXY and XXZ
            String:    XXXXZY
            String 1:    XX Y
            String 2:  XX  Z

    Input: strings: "XXY", "YX", "X"
    Output: XXY is not interleaved of YX and X
            XXY cannot be formed by interleaving YX and X.
            The strings that can be formed are YXX and XYX

    Simple Solution: Recursion.
        Approach: A simple solution is discussed here: Check whether a given string is an interleaving of two other given string.
        The simple solution doesn’t work if the strings A and B have some common characters. For example, let the given string be A and the other strings be B and C. Let A = “XXY”, string B = “XXZ” and string C = “XXZXXXY”. Create a recursive function that takes parameters A, B, and C. To handle all cases, two possibilities need to be considered.

        If the first character of C matches the first character of A, we move one character ahead in A and C and recursively check.
        If the first character of C matches the first character of B, we move one character ahead in B and C and recursively check.
        If any of the above function returns true or A, B and C are empty then return true else return false.
        Thanks to Frederic for suggesting this approach.
*/
bool isInterleaved(char* A, char* B, char* C)
{
    // Base Case: If all strings are empty
    if (!(*A) || !(*B) || !(*C))
        return true;

    // If C is empty and any of the
    // two strings is not empty
    if (*C == '\0')
        return false;

    // If both A and B are empty but not C
    if ((*A == '\0') && (*B == '\0'))
        return false;

    bool first = false;
    bool second = false;

    /* If first char of A = first char of C */
    if (*A == *C) {
        first = isInterleaved(A + 1, B, C + 1);
    }

    /* If first char of B = first char of C */
    if (*B == *C) {
        second = isInterleaved(A, B + 1, C + 1);
    }

    // If any of the above mentioned
    // two possibilities is true,
    // then return true, otherwise false
    return (first || second);
}
// A function to run test cases
void test(char* A, char* B, char* C)
{
    if (isInterleaved(A, B, C))
        printf("%s is interleaved of %s and %s \r\n", C, A, B);
    else
        printf("%s is not interleaved of %s and %s \r\n", C, A, B);
}

// Driver program to test above functions
int main()
{
    test("XXY", "XXZ", "XXZXXXY");
    test("XY", "WZ", "WZXY");
    test("XY", "X", "XXY");
    test("YX", "X", "XXY");
    test("XXY", "XXZ", "XXXXZY");
    test("xyz", "abcd", "zabyczd");
    return 0;
}