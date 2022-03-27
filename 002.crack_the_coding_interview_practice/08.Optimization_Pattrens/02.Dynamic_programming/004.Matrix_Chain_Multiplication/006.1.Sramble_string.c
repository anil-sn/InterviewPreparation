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
    Given a string s1, we may represent it as a binary tree by partitioning
    it to two non-empty substrings recursively.

    Below is one possible representation of s1 = "great":

        great
       /    \
      gr    eat
      / \    /  \
      g   r  e   at
              / \
              a   t
    To scramble the string, we may choose any non-leaf node and swap its
    two children.

    For example, if we choose the node "gr" and swap its two children,
    it produces a scrambled string "rgeat".

        rgeat
       /    \
       rg    eat
       / \    /  \
       r   g  e   at
               / \
               a   t
    We say that "rgeat" is a scrambled string of "great".

    Similarly, if we continue to swap the children of nodes "eat" and "at", it produces a scrambled string "rgtae".

        rgtae
    /    \
    rg    tae
    / \    /  \
    r   g  ta  e
        / \
        t   a
    We say that "rgtae" is a scrambled string of "great".

    Given two strings s1 and s2 of the same length, determine if s2 is a scrambled string of s1. Return 0/1 for this problem.

    Hint 1
    Hint : Divide and Conquer + Dynamic Programming

    On every step, you need to figure out if there exists one of the positions at root, using which the 2 parts of strings are scrambled strings of each other.

    Try writing a bruteforce solution and see which steps can be memoized.
*/
void sort (char str[])
{
    int chTemp;
    int i, j, len;
    len = strlen(str);
    for(i=0; i<len; i++)
    {
        for(j=0; j<(len-1); j++)
        {
            if(str[j]>str[j+1])
            {
                chTemp = str[j];
                str[j] = str[j+1];
                str[j+1] = chTemp;
            }
        }
    }
}

bool isAnagram( char s1[], char s2[]){
    char temp_s1[100] = {'\0'};
    char temp_s2[100] = {'\0'};

    strcpy(temp_s1, s1);
    strcpy(temp_s2, s2);
    sort(temp_s1);
    sort(temp_s2);
    if(strcmp(temp_s1, temp_s2))
        return false;
    else
        return true;
}

// Following function extracts characters present in `src`
// between `m` and `n` (excluding `n`)
char* substr(const char *src, int m, int n)
{
    char *dest = (char*)malloc(sizeof(char) * (n + 1));
    strncpy(dest, src+m, n);

    // return the destination string
    return dest;
}

bool isScramble(char s1[], char s2[])
{
    int l1 = strlen(s1);
    int l2 = strlen(s2);

    printf ("\t%s, %s\r\n", s1, s2);

    // Strings of non-equal length
    // cant' be scramble strings
    if(l1 != l2) {
        return false;
    }

    // Empty strings are scramble strings
    if(0 == l1) {
        return true;
    }



    // Equal strings are scramble string
    if(0 == strcmp(s1, s2)) {
        return true;
    }

    // Check for the condition of anagram
    if(true != isAnagram(s1, s2)) {
        return false;
    }

    for (int i = 1; i < l1; i++) {

        bool A_FirstPart_B_SecondPart = isScramble(substr(s1, 0, i), substr(s2, l1-i, i));
        bool A_SecondPart_B_FirstPart = isScramble(substr(s1, i, l1-i), substr(s2, 0, l1-i));
        if (A_FirstPart_B_SecondPart && A_SecondPart_B_FirstPart) {
            return true;
        }

        bool A_FirstPart_B_FirstPart   = isScramble(substr(s1, 0, i), substr(s2, 0, i));
        bool A_SecondPart_B_SecondPart = isScramble(substr(s1, i, l1-i), substr(s2, i, l1-i));
        if (A_FirstPart_B_FirstPart &&  A_SecondPart_B_SecondPart) {
            return true;
        }
    }

    return false;
}


// Driver code
int main()
{
    char s1[] = "coder";
    char s2[] = "ocred";
    bool result = isScramble(s1, s2);

    printf ("Is Sting %s is Scrambled form of string %s : %s\r\n", s1, s2, result ? "TRUE" : "FALSE");

    return 0;
}