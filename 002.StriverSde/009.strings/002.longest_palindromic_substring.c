#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


int main(void)
{
    char s[100] = "MALAYALAM";
    int n = strlen(s);
    int longest_palindrome_start = 0;
    int longest_palindrome_len = 1;
    int palindrome_len = 0;

    for (int i = 0; i < n; i++) 
    { 
        int right = i;
        // s[i, right - 1] inclusive are equal characters e.g. "aaa"
        while (right < n && s[i] == s[right]) {
            right += 1;
        }
        
        // while s[left] == s[right], s[left, right] inclusive is palindrome e.g. "baaab"
        int left = i - 1;
        while (left >= 0 && right < n && s[left] == s[right]){
            left -= 1;
            right += 1;
        }
        
        // s[left + 1, right - 1] inclusive is palindromic
        palindrome_len = right - left - 1;
        if (palindrome_len > longest_palindrome_len) {
            longest_palindrome_len = palindrome_len;
            longest_palindrome_start = left + 1;
        }
    }

    printf("Palindrome %d <-> %d", longest_palindrome_start, longest_palindrome_start+longest_palindrome_len);
    return 0;
}