#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// solution to a problem from hackerrank

int main() {

    char str[1200];
    scanf("%s", str);
    const char* ptr = str;

    int arr[10] = { 0 };

    while (*ptr) {
        if (isdigit(*ptr)) {
            int d = *ptr - '0';
            arr[d]++;
        }
        ptr++;
    }

    for (size_t i = 0; i < 10; i++)
        printf("%d ", arr[i]);

    return 0;
}
