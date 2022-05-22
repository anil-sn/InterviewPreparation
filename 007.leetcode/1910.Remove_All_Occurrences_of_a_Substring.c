#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

char *
removeOccurrences (char *s, char *part)
{
    int part_len = strlen (part);
    char *ret;

    while ((ret = strstr (s, part)) != NULL) {
        memmove (ret, ret + part_len, strlen (ret + part_len) + 1);
    }

    return s;
}