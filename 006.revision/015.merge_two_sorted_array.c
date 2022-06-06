#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void mergedSorted(int alen, int blen, int a[], int b[])
{
    int i = 0;
    int j = 0;
    int final[alen+blen];
    int pos = 0;

    while (i < alen && j < blen) {
        if (a[i] < b[j]) {
            final[pos++] = a[i++];
        } else {
            final[pos++] = b[j++];
        }
    }

    while (i < alen) {
        final[pos++] = a[i++];
    }

    while (j < blen) {
        final[pos++] = b[j++];
    }

    for (int x = 0; x < pos; x++) {
        printf("%d, ", final[x]);
    }

    printf("\r\n");
    return;
}

int main(void) {

    int a[] = {0, 3, 5, 7, 9, 11, 13};
    int b[] = {1, 2, 4, 6, 8, 10, 12};
    int alen = sizeof(a)/sizeof(a[0]);
    int blen = sizeof(b)/sizeof(b[0]);

    mergedSorted(alen, blen, a, b);

    return 0;
}

