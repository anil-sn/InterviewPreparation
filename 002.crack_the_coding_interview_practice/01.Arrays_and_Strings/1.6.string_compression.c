#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

int32_t getCompressedLength(char *s1){

    int32_t compressedlength = 0;
    int32_t i;

    for (i = 0; i < strlen(s1); i++) {

        /* If next character is different than current, increase the length.*/
        if ((i == strlen(s1)) || (s1[i] != s1[i + 1])) {
            compressedlength += 1;
        }
    }

    return compressedlength*2;
}

/* abbccccdeeeeddffffff -> a1b2c4d1e4d2f5 */
char * compressStr(char *s1) {

    int32_t i,x;
    int32_t duplicate;
    char currentChar;
    int32_t compressedlength = getCompressedLength(s1);
    char *compressedString = malloc(compressedlength + 1);

    if (compressedlength >= strlen(s1)) {
        goto EXIT;
    }

    /* Assume every char is duplicate */
    currentChar = s1[0];
    duplicate = 1;
    x = 0;
    for (i = 0; i < strlen(s1); i++) {
        if ((i+1 > strlen(s1)) || (s1[i] != s1[i+1])) {
            compressedString[x++] = currentChar;
            compressedString[x++] = duplicate +'0';
            currentChar = s1[i+1];
            duplicate = 1;
            continue;
        }

         duplicate++;
    }

EXIT:
    printf ("Current Lenth : %ld, Compressed Length: %d\r\n", strlen(s1), compressedlength);
    return compressedString;
}

/*
1.6
    String Compression: Implement a method to perform basic string compression using the counts
    of repeated characters. For example, the string aabcccccaaa would become a2blc5a3. If the
    "compressed" string would not become smaller than the original string, your method should return
    the original string. You can assume the string has only uppercase and lowercase letters (a - z).

    we can check in advance. This will be more optimal in cases where we don't have a large number of
    repeating characters. It will avoid us having to create a string that we never use. The downside of this is that
    it causes a second loop through the characters and also adds nearly duplicated code.

    One other benefit of this approach is that we can initialize StringBuilder to its necessary capacity
    up-front. Without this, StringBuilder will (behind the scenes) need to double its capacity every time it
    hits capacity. The capacity could be double what we ultimately need.
*/

int main() {

    u_int16_t i = 0;
    char s1[100] = {'\0'};
    char * cur = NULL;
    char * cmpStr = NULL;

    printf("Enter the string for compression :\r\n");
    scanf("%s", s1);

    cmpStr = compressStr(s1);
    printf ("%s\r\n", cmpStr);
    free(cmpStr);

    return 0;
}

