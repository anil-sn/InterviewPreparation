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
    Palindrome Partitioning

    Problem Statement: You are given a string s, partition it in such a way that every substring
        is a palindrome. Return all such palindromic partitions of s.
    Note: A palindrome string is a string that reads the same backward as forward.

    Examples:

        Example 1:
        Input: s = “aab”
        Output: [ ["a","a","b"], ["aa","b"] ]

        Example 2:
        Input: s = “aabb”
        Output: [ [“a”,”a”,”b”,”b”], [“aa”,”bb”], [“a”,”a”,”bb”], [“aa”,”b”,”b”] ]

*/

char combination[100][100];
char string[100];
int row;
int pos;

bool isPalindrome(char s[], int start, int end)
{
    while (start <= end) {
        if (s[start++] != s[end--]) {
            return false;
        }
    }

    return true;
}

void printPalindrome(char s[], int start, int end)
{
    printf("Palindrome substring : ");

    for (int i = start; i <= end; i++){
        printf("%c", s[i]);
        string[pos++] = s[i];
    }
    string[pos++] = ',';
    printf("\r\n");
    return;
}

void func(int index, char s[], int len) {
    if (index == len) {
        sprintf(combination[row++], "%s", string);
        return;
    }

    for (int i = index; i < len; i++) {
        if (isPalindrome(s, index, i)) {
            printPalindrome(s, index, i);
            func(i + 1, s, len);
            for (int j = index; j <= i+1; j++) {
                string[pos--] = '\0';
            }
        }
    }
    return;
}

void partition(char s[], int n)
{
    row = 0;
    pos = 0;
    memset(combination, '\0', sizeof(combination));
    memset(string, '\0', sizeof(string));
    func(0, s, n);
    return;
}

int main() {
    char s[] = "aabb";
    int n = strlen(s);
    partition(s, n);

    for (int i = 0; i < row; i++) {
        printf("%s \r\n", combination[i]);
    }
    printf("\r\n");
    return;
}