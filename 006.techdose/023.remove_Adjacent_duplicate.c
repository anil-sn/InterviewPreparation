#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void
cleanup (int n, char s[])
{
    int i = 0;
    char res[100] = {'\0'};
    int pos = 0;

    while (i < n) {
        if (s[i] != s[i + 1]) {
            res[pos++] = s[i];
            i++;
        }

        if (('\0' != s[i + 1]) && (s[i] == s[i + 1])) {
            while (('\0' != s[i + 1]) && (s[i] == s[i + 1])) { 
                i++; 
            }
        }
    }
    
    for (int j = 0; j < pos; j++) {
        printf("%c", res[j]);
    }
    printf("\r\n");
    return;
}


int
main (void)
{
    char input[] = {"aaaabbbbbccccdddeeefff"};
    int n = strlen (input);
    printf ("%s \r\n", input);
    cleanup (n, input);
    return 0;
}