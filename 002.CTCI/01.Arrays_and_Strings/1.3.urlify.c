#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "sys/types.h"

/*
   1.3 URLify: Write a method to replace all spaces in a string with '%20'. You may assume that the string
   has sufficient space at the end to hold the additional characters, and that you are given the "true"
   length of the string.

   EXAMPLE
Input: "Mr John Smith
Output: "Mr%20John%20Smith"

SOLUTION
A common approach in string manipulation problems is to edit the string starting from the end and working
backwards. This is useful because we have an extra buffer at the end, which allows us to change characters
without worrying about what we're overwriting. We will use this approach in this problem.
The algorithm employs a two-scan approach. In the first scan, we count the number of spaces.
By tripling this number, we can compute how many extra characters we will have in the final string.
In the second pass, which is done in reverse order, we actually edit the string. When we see a space,
we replace it with %20. If there is no space, then we copy the original character.
*/

void replaceSpaces(char *str, int trueLength) {

    int16_t spaceCount = 0, index, i = 0;

    for (i = 0; i < trueLength; i++) {
        if (str[i] == ' ') {
            spaceCount++;
        }
    }

    printf ("Space Count : %d\r\n", spaceCount);

    index = trueLength + spaceCount * 2;
    if (trueLength < strlen(str)) {
        str[trueLength] = '\0';
    }

    for (i = trueLength - 1; i >= 0; i-- ) {
        if (str[i] == ' ') {
            str[index - 1] = '0';
            str[index - 2] = '2';
            str[index - 3] = '%';
            index = index - 3;
        } else {
            str[index - 1] = str[i];
            index--;
        }
    }
}

int main () {

    u_int16_t i,spaceCnt = 0;
    char inPutStr[100] = {'\0'};

    printf ("Enter Sentance to Be Test :\r\n");
    scanf ("%[^\n]s", inPutStr);

    for (i = 0; i < strlen(inPutStr); i++) {
        if (inPutStr[i] == ' ') {
            spaceCnt++;
        }
    }

    printf("%s:Len %ld: Spaces: %d\r\n", inPutStr, strlen(inPutStr), spaceCnt);
    replaceSpaces(inPutStr, strlen(inPutStr)+spaceCnt);

    printf("%s\r\n",inPutStr);

    return 0;
}
