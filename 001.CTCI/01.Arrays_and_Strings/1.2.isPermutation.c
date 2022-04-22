#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "sys/types.h"

/*

   1.2 Check Permutation: Given two strings, write a method to decide if one is a permutation of the other.

   SOLUTION
   Like in many questions, we should confirm some details with our interviewer. We should understand if the
   permutation comparison is case sensitive. That is: is God a permutation of dog? Additionally, we should
   ask if whitespace is significant. We will assume for this problem that the comparison is case sensitive and
   whitespace is significant. So, "god " is different from "dog".
   Observe first that strings of different lengths cannot be permutations of each other. There are two easy
   ways to solve this problem, both of which use this optimization.

Solution: Check if the two strings have identical character counts.
We can also use the definition of a permutation-two words with the same character counts-to implement this algorithm.
We simply iterate through this code, counting how many times each character appears.
Then, afterwards, we compare the two arrays.

*/
int main () {

    u_int16_t i = 0;
    char s1[100] = {'\0'};
    char s2[100] = {'\0'};
    char *cur = NULL;
    u_int16_t charCount[26] = {0};

    printf ("Enter First Word to test :\r\n");
    scanf ("%s", s1);
    printf ("Enter Second Word to test :\r\n");
    scanf ("%s", s2);

    cur = s1;
    for ( ; *cur; ++cur) {
        *cur = tolower(*cur);
        printf ("%d,", *cur-'a');
        charCount[(*cur - 'a')] += 1;
    }

    printf("\r\n");

    cur = s2;
    for ( ; *cur; ++cur) {
        *cur = tolower(*cur);
        printf ("%d,", *cur-'a');
        charCount[(*cur - 'a')] += 1;
    }

    printf("\r\n");

    for (i = 0; i < 26; i++) {
        printf("%d,", charCount[i]);
        if ((charCount[i] != 0) && (charCount[i] < 2)) {
            printf ("\r\nDue to '%c' both strings are not palindrome\r\n", i+'a');
        }
    }

    printf("\r\n other string is permuation of first one \r\n");
    return 0;
}
