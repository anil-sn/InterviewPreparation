#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int
min (int a, int b, int c) {
    if (a < b) {
        if (a < c) {
            return a;
        } else {
            return c;
        }
    } else {
        if (b < c) {
            return b;
        } else {
            return c;
        }
    }
}

int
editDistance(char *str1, char *str2) {
    int deleteCount = 0;
    int updateCount = 0;
    int insertCount = 0;

    printf ("%c, %c ", *str1, *str2);
    if ((NULL == str1) || ('\0' == *str1)) {
        return strlen(str2);
    }

    if ((NULL == str2) || ('\0' == *str2)) {
        return strlen(str1);
    }

    if (*str1 == *str2) {
        return (editDistance(str1+1, str2+1));
    }

    deleteCount = editDistance(str1+1, str2);
    updateCount = editDistance(str1+1, str2+1);
    insertCount = editDistance(str1, str2+1);

    return (min(deleteCount, updateCount, insertCount) + 1);
}

int main()
{
    char str1[] = "sunday";
    char str2[] = "saturday";

    printf ("Edit Distance of sunday & saturday is : %d\r\n", editDistance(str1, str2));
    return;
}