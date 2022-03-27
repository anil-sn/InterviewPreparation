#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

// Print a digit in binary
void bin(int n, char reason[]) {
    int i;
    printf("0");
    for (i = 1 << 30; i > 0; i = i / 2) {
        if((n & i) != 0) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("[ %d ]: %s ", n, reason);
    printf("\r\n");
}

bool oppositeSigns(int x, int y) {
    bin(x, "x");
    bin(y, "y");
    bin((x ^ y), "(x ^ y)");
    return ((x ^ y) < 0);
}

int main()
{
    int x = 10, y = -10;
    if (oppositeSigns(x, y) == true) {
        printf("Signs are opposite\r\n");
    } else {
        printf("Signs are not opposite\r\n");
    }
    return 0;
}