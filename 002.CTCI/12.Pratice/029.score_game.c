#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int count2(int n) {
    if (n < 0) {
        return 0;
    }
    if (n == 0) {
        return 1;
    }

    return (count2(n-10) + count2(n-5) + count2(n-3));
}

// Returns number of ways to reach score n
int count(int n) {
    // table[i] will store count of solutions for
    // value i.
    int table[n+1], i;
    memset(table, 0, sizeof(int)*(n+1));

    // Initialize all table values as 0
    memset(table, 0, sizeof(table));

    // Base case (If given value is 0)
    table[0] = 1;

    // One by one consider given 3 moves and update the table[]
    // values after the index greater than or equal to the
    // value of the picked move
    for (i=1; i<=n; i++) {
        if (i-3 >= 0)
            table[i] += table[i-3];
        if (i-5 >= 0)
            table[i] += table[i-5];
        if (i-10 >= 0)
            table[i] += table[i-10];
    }

    return table[n];
}


// Driver program
int main(void) {
    int n = 20;
    printf("Count for %d is %d:%d\r\n", n, count(n), count2(n));

    n = 13;
    printf("Count for %d is %d:%d\r\n", n, count(n), count2(n));
    return 0;
}
