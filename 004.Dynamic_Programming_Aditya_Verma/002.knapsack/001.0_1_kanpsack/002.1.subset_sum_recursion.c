#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

bool
isSubSetSum(int array[], int len, int sum) {
    if((0 == sum) && (0 == len)) {
        return true;
    }

    if(0 == sum) {
        return true;
    }

    if(0 == len) {
        return false;
    }

    if (array[len - 1] > sum) {
        return (isSubSetSum(array, len - 1, sum));
    } else {
        return ((isSubSetSum(array, len - 1, sum)) ||
                (isSubSetSum(array, len - 1, sum - array[len - 1])));
    }

    return false;
}

int main (){
    int array[] = {2,3,7,8,10};
    int sum = 11;
    int len = sizeof(array)/sizeof(array[0]);

    printf("Is subset present in array [2,3,7,8,10] with sum 11 : %s\r\n",
        (isSubSetSum(array,len,sum) ? "TRUE" : "FALSE"));
}