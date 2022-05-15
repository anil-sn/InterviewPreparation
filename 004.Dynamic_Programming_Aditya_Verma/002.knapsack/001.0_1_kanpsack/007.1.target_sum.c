#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

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

int numOfSubsetWithDiff(int array[], int n, int diff) {

    // Compute total sum of elements
    int sumTotal = 0;
    for (int i = 0; i < n; i++)
        sumTotal += array[i];

    /* S1 - S2 = diff                Ex: [1,1,2,3]
     + S1 + S2 = totalSum            [1,3] , [1,2]
       ------------------            [1,3] , [1,2]
       2S1 = diff  +  totalSum       [1,1,2], [3]
        S1 = diff  +  totalSum
             ---------------         formula Validated:
                    2                4 = (7 + 1)
                                         -------
                                            2
    */

    int count = subsetSumCount(array, n, (diff + sumTotal)/2);
    return count;
}
/*
    You are given an integer array nums and an integer target.

    You want to build an expression out of nums by adding one of the symbols '+' and '-' before each integer in nums and then concatenate all the integers.

    For example, if nums = [2, 1], you can add a '+' before 2 and a '-' before 1 and concatenate them to build the expression "+2-1".
    Return the number of different expressions that you can build, which evaluates to target.

    Example 1:

    Input: nums = [1,1,1,1,1], target = 3
    Output: 5
    Explanation: There are 5 ways to assign symbols to make the sum of nums be target 3.
    -1 + 1 + 1 + 1 + 1 = 3
    +1 - 1 + 1 + 1 + 1 = 3
    +1 + 1 - 1 + 1 + 1 = 3
    +1 + 1 + 1 - 1 + 1 = 3
    +1 + 1 + 1 + 1 - 1 = 3

    Example 2:
    Input: nums = [1], target = 1
    Output: 1
*/
int main (){
    int array[] = {1, 1, 2, 3};
    int eval = 1;
    int len = sizeof(array)/sizeof(array[0]);

    /*  This is same as count number of subset with given difference
        Lets see how ?

        Consider Example :1, 1, 2, 3
        -1 +1 -2 +3 == -1 -2 +1 +3 == (1 + 3) - (1 + 2) = (4) - (3) = 1
        +1 -1 -2 +3 == -1 -2 +1 +3 == (1 + 3) - (1 + 2) = (4) - (3) = 1
        +1 +1 +2 -3 == (1 + 1 + 2) - (3) = (4) - (3) = 1
    */

    printf("The number of different expressions in array [1, 1, 2, 3] which evaluates to %d is %d\r\n",
            eval,
            numOfSubsetWithDiff(array,len,eval));

    return;
}