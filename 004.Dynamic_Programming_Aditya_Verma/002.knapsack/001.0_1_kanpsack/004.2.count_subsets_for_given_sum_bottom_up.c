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
    {1, 2, 3, 3, 6}
    1,2,3
    1,2,3
    3,3
    6
*/
// Recursive function to return the count
// of subsets with sum equal to the given value
int subsetSumCount(int array[], int n, int sum)
{
    int MEM[n+1][sum+1];
    memset(MEM, 0, sizeof(MEM));

    // If sum is 0 and array is 0
    MEM[0][0] = 1;

    // If sum is 0, then answer is true
    for (int i = 0; i <= n; i++)
        MEM[i][0] = 1;

    // If sum is not 0 and set is empty,
    // then answer is false
    for (int i = 1; i <= sum; i++)
        MEM[0][i] = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= sum; j++) {
            if (j < array[i-1]) {
                MEM[i][j] = MEM[i-1][j];
            }
            if (j >= array[i-1]) {
                MEM[i][j] = MEM[i-1][j] + MEM[i-1][j - array[i-1]];
            }
        }
    }

    for (int i = 0; i < n+1; i++) {
        for (int j = 0; j < sum+1; j++) {
            printf("[%d][%d] = %d ",i, j, MEM[i][j]);
        }
        printf ("\r\n");
    }
    printf ("\r\n");

    return MEM[n][sum];
}

int main (){
    int array[] = {1, 2, 3, 3, 6};
    int sum = 6;
    int len = sizeof(array)/sizeof(array[0]);

    printf("Is subset present in array [1, 2, 3, 3] with sum %d : %d\r\n",
            sum,
            subsetSumCount(array,len,sum));
}