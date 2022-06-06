#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int bellNumber(int n) 
{ 
    int bell[n+1][n+1]; 
    bell[0][0] = 1; 
    for (int i=1; i<=n; i++) 
    { 
        // Explicitly fill for j = 0 
        bell[i][0] = bell[i-1][i-1]; 

        // Fill for remaining values of j 
        for (int j=1; j<=i; j++) 
            bell[i][j] = bell[i-1][j-1] + bell[i][j-1]; 
    } 
    return bell[n][0]; 
} 

// Driver program 
int main() 
{ 
    for (int n=0; n<=5; n++) 
        printf("Bell Number %d is %d \r\n", n, bellNumber(n));
    return 0; 
} 