#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int missingNumber(int input[], int n) {

    int x1 = input[0];
    int x2 = 1;

    for (int i = 1; i < n; i++) {
        x1 = x1 ^ input[i];
    }
    
    for (int i = 2; i <= n + 1; i++) {
        x2 = x2 ^ i;
    }
    return x1 ^ x2;

}

int main(void) {

    int a[] = {1, 2, 3, 4, 6, 7, 8};
    int n = sizeof(a)/sizeof(a[0]);

    int missing = missingNumber(a, n);
    printf(" Missing Number is ::: %d\r\n", missing);

    return 0;
}

