#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// row,col
// row array length
// coloumn is sum value
int MEM[100][100];

#define TRUE  1
#define FALSE 0
#define UNKNOWN -1

bool
isSubSetSum(int array[], int len, int sum) 
{

    if((0 == sum) && (0 == len)) {
        return true;
    }

    if(0 == sum) {
        return true;
    }

    if(0 == len) {
        return false;
    }

    if (UNKNOWN != MEM[len][sum]) {
        return MEM[len][sum];
    }

    if (array[len-1] > sum) {
        MEM[len][sum] = (isSubSetSum(array, len - 1, sum));
    } else {
        MEM[len][sum] = ((isSubSetSum(array, len - 1, sum)) ||
                         (isSubSetSum(array, len - 1, sum - array[len - 1])));
    }

    return MEM[len][sum];
}

int main ()
{
    int array[] = {2,3,7,8,10};
    int sum = 11;
    int len = sizeof(array)/sizeof(array[0]);

    for (int i = 0; i < len + 1; i++) {
        for (int j = 0; j < sum + 1; j++) {
            if ((0 == i) && (0 == j)) {
                MEM[i][j] = TRUE;
            } else if (i == 0) {
                MEM[i][j] = FALSE;
            } else if (j == 0) {
                MEM[i][j] = TRUE;
            } else {
                MEM[i][j] = UNKNOWN;
            }
        }
    }

    printf("Is subset present in array [2,3,7,8,10] with sum 11 : %s\r\n", (isSubSetSum(array,len,sum) ? "TRUE" : "FALSE"));
}