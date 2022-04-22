#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
1.9 String Rotation: Assume you have a method i 5Su b 5 tr ing which checks if one word is a substring
    of another. Given two strings, 51 and 52, write code to check if 52 is a rotation of 51 using only one
    call to i5Sub5tring (e.g., "waterbottle" is a rotation of" erbottlewat").

SOLUTION
    If we imagine that s2 is a rotation of s1, then we can ask what the rotation point is. For example, if you
    rotate waterbottle after wat. you get erbottlewat. In a rotation, we cut 51 into two parts, x and y,
    and rearrange them to get s2.
    s1 = xy = waterbottle
    x = wat
    y = erbottle
    s2 = yx = erbottlewat

    So, we need to check if there's a way to split s1 into x and y such that xy = s1 and yx = s2. Regardless of
    where the division between x and y is, we can see thatyx will always be a substring of xyxy.That is, s2 will
    always be a substring of s1s1.
    And this is precisely how we solve the problem: simply do isSubstring(slsl, s2).

*/

bool
isRotation(char *s1, char *s2) {
    char d[200] = {'\0'};
    int  len = 0;

    len = strlen(s1);
    /* Check that sl and s2 are equal length and not empty*/
    if (len == strlen(s2) && len > 0) {

        /* Concatenate s1 and s2 within new buffer */
        strcat (strcpy (d, s1), s1);
        if(strstr(d, s2) != NULL) {
            return true;
        }
    }

    return false;
}


int main () {

    u_int16_t i = 0;
    char s1[100] = {'\0'};
    char s2[100] = {'\0'};

    printf ("First String to test :\r\n");
    scanf ("%s", s1);
    printf ("Rotation String to test :\r\n");
    scanf ("%s", s2);

    if (isRotation(s1, s2)) {
        printf ("Second String is rotation of first string \r\n");
    } else {
        printf ("Second String is not rotation of first string \r\n");
    }

    return 0;
}
