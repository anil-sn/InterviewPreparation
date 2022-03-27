#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Dont practice until you get it right
    practice until you can't get it worng
*/

void
print2DArray(int array[][10], int n, int m){
    int i,j;
    for (i = 0; i < n+1; i++) {
        for (j = 0; j < m; j++) {
            printf("%2d ", array[i][j]);
        }
        printf("\r\n");
    }
}

void
printArray(int array[], int n){
    int i,j;
    for (i = 0; i < n+1; i++) {
        printf("%2d ", array[i]);
    }
    printf("]\r\n");
}

// C program for coin change problem.
int count( int S[], int m, int n ) {
    int i, j, x, y;

    // We need n+1 rows as the table is constructed
    // in bottom up manner using the base case 0
    // value case (n = 0)
    int table[n+1][m];
    memset(table, 0, sizeof(int)*(n+1)*m);

    // Fill the entries for 0 value case (n = 0)
    for (i=0; i<m; i++)
        table[0][i] = 1;

    print2DArray(table, n, m-1);

    // Fill rest of the table entries in bottom
    // up manner
    for (i = 1; i < n+1; i++) {
        for (j = 0; j < m; j++) {
            // Count of solutions including S[j]
            x = (i-S[j] >= 0) ? table[i - S[j]][j]: 0;
            printf("Count of solutions including S[%d]=%d is  %d\r\n", j, S[j], x);

            // Count of solutions excluding S[j]
            y = (j >= 1) ? table[i][j-1]: 0;
            printf("Count of solutions excluding S[%d]=%d is  %d\r\n", j, S[j], y);

            // total count
            table[i][j] = x + y;
            print2DArray(table, n, m-1);
        }
    }

    print2DArray(table, n, m-1);
    return table[n][m-1];
}

int count_optimized( int S[], int m, int n ) {

    // table[i] will be storing the number of solutions for
    // value i. We need n+1 rows as the table is constructed
    // in bottom up manner using the base case (n = 0)
    int table[n+1];

    // Initialize all table values as 0
    memset(table, 0, sizeof(table));

    // Base case (If given value is 0)
    table[0] = 1;

    // Pick all coins one by one and update the table[] values
    // after the index greater than or equal to the value of the
    // picked coin
    for(int i=0; i<m; i++) {
        printf("\r\n");
        for(int j=S[i]; j<=n; j++) {
            table[j] += table[j-S[i]];
            printf("S[i]= %d, table[j] = %d, i = %d, j = %d, table[%d] = %d",S[i], table[j], i, j, i, table[i]);
            printf("\r\n\t  [");
            printArray(table, n);
        }
    }

    return table[n];
}
// Driver program to test above function
int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int m = sizeof(arr)/sizeof(arr[0]);
    int n = 5;

    //printf("There are %d ways to make change \r\n", count(arr, m, n));
    printf("[1, 2, 3, 4, 5, 6, 7, 8, 9, 10] to make 4$ \r\n");
    printf("[Optimized] There are %d ways to make change \r\n", count_optimized(arr, m, n));
    return 0;
}