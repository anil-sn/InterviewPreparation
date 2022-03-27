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
    Given a word and a text, return the count of the occurrences of anagrams of the
    word in the text(For eg: anagrams of word for are for, ofr, rof etc.))

    Examples:

        Input : forxxorfxdofr
                for
        Output : 3
            Explanation : Anagrams of the word for - for, orf,
            ofr appear in the text and hence the count is 3.

        Input : aabaabaa
                aaba
        Output : 4
            Explanation : Anagrams of the word aaba - aaba,
            abaa each appear twice in the text and hence the
            count is 4.
*/
bool isAnagram(char s1[], char s2[]){
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

int findAnagrams (char test[], char pat[]){
    int len = strlen(test);
    int k = strlen(pat);
}

int main()
{
    char text[] = "forxxorfxdofr";
    char pat[] = "for";

    printf ("Number of anagrams found in string %s, for pattren %s is %d\r\n",
            test, pattren, findAnagrams(text, pat));

    return 0;
}