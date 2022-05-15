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
    Length of Longest Substring without any Repeating Character
    Problem Statement: Given a String, find the length of longest substring
                       without any repeating character.

    Examples:
        Example 1:
            Input: s = ”abcabcbb”
            Output: 3
            Explanation: The answer is abc with length of 3.

        Example 2:
            Input: s = ”bbbbb”
            Output: 1
            Explanation: The answer is b with length of 1 units.
*/

#define MAX(_A, _B) (_A > _B ? _A : _B)

int lengthofLongestSubstring(char s[], int size)
{
    int mpp[256] = {-1};
    int left = 0;
    int right = 0;
    int n = size;
    int len = 0;

    while (right < n) {
        if (mpp[s[right]- 'a'] != -1) {
            left = MAX(mpp[s[right] - 'a'] + 1, left);
        }

        mpp[s[right]- 'a'] = right;

        len = MAX(len, right - left + 1);
        right++;
    }

    return len;
}

int main() {
    char str[] = "abcabcbb";
    int len = sizeof(str)/sizeof(str[0]);
    printf("The length of the longest substring without repeating characters is %d \r\n", lengthofLongestSubstring(str, len));
    return;
}