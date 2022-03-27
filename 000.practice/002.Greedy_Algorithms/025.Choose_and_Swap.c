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

/*
    You are given a string s of lower case english alphabets.
    You can choose any two characters in the string and replace all the occurences
    of the first character with the second character and
    replace all the occurences of the second character with the first character.
    Your aim is to find the lexicographically smallest string that can be obtained
    by doing this operation at most once.

    Example 1:
    Input:
        A = "ccad"
        Output: "aacd"
    Explanation:
        In ccad, we choose a and c and after
        doing the replacement operation once we get,
        aacd and this is the lexicographically
        smallest string possible.


    Example 2:
    Input:
        A = "abba"
        Output: "abba"
    Explanation:
        In abba, we can get baab after doing the
        replacement operation once for a and b
        but that is not lexicographically smaller
        than abba. So, the answer is abba.

    Approach:

    First we store the first appearance of every character in a string in a hash array chk[].
    The in order to find the lexicographically smaller string, the leftmost character must be
    replaced with some character which is smaller than it. This will only happen if the smaller
    character appears after it in the array.

    So, starting traversing the string from the left and for every character, find the smallest
    character (even smaller than the current character) that appears after swap all of their
    occurrences to get the required string.

    If no such character pair is found in the previous string then print the given string
    as it is the smallest string possible.

    Below is the implementation of the above approach:
*/

#define MAX 26

// Function to return the lexicographically
// smallest string after swapping all the
// occurrences of any two characters
char* smallestStr(char str[], int n)
{
    int i, j;

    // To store the first index of every character of str
    int chk[MAX];
    for (i = 0; i < MAX; i++)
        chk[i] = -1;

    // Store the first occurring index every character
    for (i = 0; i < n; i++) {

        // If current character is appearing for the first time in str
        if (chk[str[i] - 'a'] == -1)
            chk[str[i] - 'a'] = i;
    }

    // Starting from the leftmost character
    for (i = 0; i < n; i++) {

        bool flag = false;

        // For every character smaller than str[i]
        for (j = 0; j < str[i] - 'a'; j++) {

            // If there is a character in str which is
            // smaller than str[i] and appears after it
            if (chk[j] > chk[str[i] - 'a']) {
                flag = true;
                break;
            }
        }

        // If the required character pair is found
        if (flag)
            break;
    }

    // If swapping is possible
    if (i < n) {

        // Characters to be swapped
        char ch1 = str[i];
        char ch2 = (j + 'a');

        // For every character
        for (i = 0; i < n; i++) {

            // Replace every ch1 with ch2
            // and every ch2 with ch1
            if (str[i] == ch1)
                str[i] = ch2;

            else if (str[i] == ch2)
                str[i] = ch1;
        }
    }

    return str;
}

// Driver code
int main()
{
    char str[] = "ccad";
    int n = strlen(str);

    printf("The input string : %s\r\n", str);
    printf("The lexicographically smallest string : %s\r\n", smallestStr(str, n));
    return 0;
}