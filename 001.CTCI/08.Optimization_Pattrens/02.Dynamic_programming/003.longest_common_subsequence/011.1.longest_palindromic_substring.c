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
    Given a string, find the longest substring which is palindrome.

    For example,

        Input: Given string :"forgeeksskeegfor",
        Output: "geeksskeeg"

        Input: Given string :"Geeks",
        Output: "ee"

    Approach: The time complexity can be reduced by storing results of sub-problems.
              The idea is similar to this post.

    Maintain a boolean table[n][n] that is filled in bottom up manner.

    The value of table[i][j] is true, if the substring is palindrome, otherwise false.

    To calculate table[i][j], check the value of table[i+1][j-1],
    if the value is true and str[i] is same as str[j], then we make table[i][j] true.
    Otherwise, the value of table[i][j] is made false.

    We have to fill table previously for substring of length = 1 and length =2 because
    as we are finding , if table[i+1][j-1] is true or false , so in case of
    (i) length == 1 , lets say i=2 , j=2 and i+1,j-1 doesn’t lies between [i , j]
    (ii) length == 2 ,lets say i=2 , j=3 and i+1,j-1 again doesn’t lies between [i , j].
*/
// Function to print a substring
// str[low..high]
void printSubStr(char str[], int low, int high) {
    for (int i = low; i <= high; ++i)
        printf("%c",str[i]);
}

// This function prints the
// longest palindrome substring
// It also returns the length of
// the longest palindrome
int longestPalSubstr(char str[])
{
    // get length of input string
    int n = strlen(str);

    // table[i][j] will be false if substring
    // str[i..j] is not palindrome.
    // Else table[i][j] will be true
    bool table[n][n];

    memset(table, 0, sizeof(table));

    // All substrings of length 1
    // are palindromes
    int maxLength = 1;

    for (int i = 0; i < n; ++i)
        table[i][i] = true;

    // check for sub-string of length 2.
    int start = 0;
    for (int i = 0; i < n - 1; ++i) {
        if (str[i] == str[i + 1]) {
            table[i][i + 1] = true;
            start = i;
            maxLength = 2;
        }
    }

    // Check for lengths greater than 2.
    // k is length of substring
    for (int k = 3; k <= n; ++k) {
        // Fix the starting index
        for (int i = 0; i < n - k + 1; ++i) {
            // Get the ending index of substring from
            // starting index i and length k
            int j = i + k - 1;

            // checking for sub-string from ith index to
            // jth index iff str[i+1] to str[j-1] is a
            // palindrome
            if (table[i + 1][j - 1] && str[i] == str[j]) {
                table[i][j] = true;

                if (k > maxLength) {
                    start = i;
                    maxLength = k;
                }
            }
        }
    }

    printf("Longest palindrome substring is: ");
    printSubStr(str, start, start + maxLength - 1);

    // return length of LPS
    return maxLength;
}

// Driver Code
int main()
{
    char str[] = "forgeeksskeegfor";
    printf("\nLength is: %d\r\n", longestPalSubstr(str));
    return 0;
}