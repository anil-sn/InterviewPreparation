#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"
//#include "color.h"

// row,col
// row array length
// coloumn is sum value


#define TRUE  1
#define FALSE 0

#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define CRESET "\e[0m"

bool
isSubSetSum(int array[], int len, int sum) {

    bool MEM[len+1][sum+1];
    memset(MEM, 0, sizeof(MEM));

    // If sum is 0 and array is 0
    MEM[0][0] = true;

    // If sum is 0, then answer is true
    for (int i = 0; i <= len; i++)
        MEM[i][0] = true;

    // If sum is not 0 and set is empty, then answer is false
    for (int i = 1; i <= sum; i++)
        MEM[0][i] = false;

    for (int i = 1; i <= len; i++) {
        for (int j = 1; j <= sum; j++) {
            if (j < array[ i - 1]) {
                MEM[i][j] = MEM[i - 1][j];
            }
            if (j >= array[i - 1]) {
                MEM[i][j] = MEM[i-1][j] || MEM[i - 1][j - array[i - 1]];
            }
        }
    }

    for (int i = 0; i < len+1; i++) {
        for (int j = 0; j < sum+1; j++) {
            printf("[%d][%d] = %d \t",i, j, MEM[i][j]);
        }
        printf ("\r\n");
    }
    printf ("\r\n");

    return MEM[len][sum];
}

int main (){
    int array[] = {2,3,7,8,10};
    int sum = 8;
    int len = sizeof(array)/sizeof(array[0]);

    printf("Is subset present in array [2,3,7,8,10] with sum %d : %s\r\n", sum,
        (isSubSetSum(array,len,sum) ? "TRUE" : "FALSE"));
}