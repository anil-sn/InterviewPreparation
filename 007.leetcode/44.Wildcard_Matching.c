#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


bool
isMatch (char *s, char *p)
{
    const char *star = NULL;
    const char *ss = s;

    while (*s) {
        //advancing both pointers when (both characters match) or ('?' found in pattern)
        //note that *p will not advance beyond its length
        if ((*p == '?') || (*p == *s)) {
            s++;
            p++;
            continue;
        }

        // * found in pattern, track index of *, only advancing pattern pointer
        if (*p == '*') {
            star = p++;
            ss = s;
            continue;
        }

        //current characters didn't match, last pattern pointer was *, current pattern pointer is not *
        //only advancing pattern pointer
        if (star) {
            p = star + 1;
            s = ++ss;
            continue;
        }

        //current pattern pointer is not star, last patter pointer was not *
        //characters do not match
        return false;
    }

    //check for remaining characters in pattern
    while (*p == '*') {
        p++;
    }

    return !*p;
}