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
    In the original Naive String matching algorithm , we always slide the pattern by 1.
    When all characters of pattern are different, we can slide the pattern by more than 1.
    Let us see how can we do this. When a mismatch occurs after j matches,
    we know that the first character of pattern will not match the j matched characters
    because all characters of pattern are different. So we can always slide the pattern by j
    without missing any valid shifts. Following is the modified code that is optimized for the
    special patterns.
*/

/* A modified Naive Pattern Searching algorithm that is optimized
   for the cases when all characters of pattern are different */
void search(char pat[], char txt[])
{
    int M = strlen(pat);
    int N = strlen(txt);
    int i = 0;
    int j = 0;

    while (i <= N - M)
    {
        /* For current index i, check for pattern match */
        for (j = 0; j < M; j++)
            if (txt[i+j] != pat[j])
                break;

        if (j == M)  // if pat[0...M-1] = txt[i, i+1, ...i+M-1]
        {
           printf("Pattern found at index %d \n", i);
           i = i + M;
        }
        else if (j == 0)
           i = i + 1;
        else
           i = i + j; // slide the pattern by j
    }
}

/* Driver program to test above function */
int main()
{
   char txt[] = "ABCEABCDABCEABCD";
   char pat[] = "ABCD";
   search(pat, txt);
   return 0;
}