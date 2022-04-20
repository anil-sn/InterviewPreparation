#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
One Away: There are three types of edits that can be performed on strings: insert a character,
          remove a character, or replace a character. Given two strings, write a function to check if they are
          one edit (or zero edits) away.
EXAMPLE
    pale, ple -> true
    pales, pale -> true
    pale, bale -> true
    pale, bae -> false
*/

bool
oneEditAway (char* first, char* second) {

    /* Length checks. */
    if (abs(strlen(first) - strlen(second)) > 1) {
        return false;
    }

    /* Get shorter and longer string.*/
    char * s1 = strlen(first) < strlen(second) ? first : second;
    char * s2 = strlen(first) < strlen(second) ? second : first;

    int index1 = 0;
    int index2 = 0;

    bool foundDifference = false;
    while (index2 < strlen(s2) && index1 < strlen(s1)) {
        if (s1[index1] != s2[index2]) {

            /* Ensure that this is the first difference found.*/
            if (foundDifference) {
                return false;
            }

            foundDifference = true;
            if (strlen(s1) == strlen(s2)) { //On replace, move shorter pointer
                index1++;
            }
        } else {
            index1++; // If matching, move shorter pointer
        }
        index2++; // Always move pointer for longer string
    }
    return true;
}

int main() {

    u_int16_t i = 0;
    char s1[100] = {'\0'};
    char s2[100] = {'\0'};
    char * cur = NULL;
    u_int16_t charCount[26] = {0};

    printf("Enter First Word to test :\r\n");
    scanf("%s", s1);
    printf("Enter Second Word to test :\r\n");
    scanf("%s", s2);

    if (oneEditAway(s1, s2)) {
        printf("\r\n One Way edit is possible \r\n");
    } else {
        printf("\r\n One Way edit is not possible \r\n");
    }

    return 0;
}
