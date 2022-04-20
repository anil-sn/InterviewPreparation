#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "sys/types.h"

#define maxASCII 256
#define maxCount 100

/*
   1.1 Is Unique: Implement an algorithm to determine if a string has all unique characters. What if you
   cannot use additional data structures?

   SOLUTION
   You should first ask your interviewer if the string is an ASCII string or a Unicode string. Asking this question
   will show an eye for detail and a solid foundation in computer science. We'll assume for simplicity the char­
   acter set is ASCII. If this assumption is not valid, we would need to increase the storage size.
   One solution is to create an array of boolean values, where the flag at index i indicates whether character
   i in the alphabet is contained in the string. The second time you see this character you can immediately
   return false.
   We can also immediately return false if the string length exceeds the number of unique characters in the
   alphabet. After all, you can't form a string of 280 unique characters out of a 128-character alphabet.

   It's also okay to assume 256 characters. This would be the case in extended ASCII. You should
   clarify your assumptions with your interviewer.
   */

int main () {

    u_int16_t i = 0;
    bool bitmap[maxASCII] = {false};
    char inPutStr[maxCount] = {'\0'};

    printf ("Enter Sentance to Be Test :\r\n");
    scanf ("%[^\n]s", inPutStr);
    printf ("%s\r\n", inPutStr);

    for (i = 0; i < strlen(inPutStr); i++) {
        if (true == bitmap[inPutStr[i]]) {
            printf("Found Duplicate @ %d[%c]\r\n", i, inPutStr[i]);
        }

        bitmap[inPutStr[i]] = true;
    }

    printf("No Duplicates Found \r\n");

    return 0;
}
