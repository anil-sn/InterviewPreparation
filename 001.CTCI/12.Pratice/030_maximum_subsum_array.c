#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


/* Function to calculate maximum subsum array */
int
maxsum(int array[], int length) {
    int i,j;
    int maxsum = 0;
    int tempsum = 0;

    for (i=0; i<length; i++) {
        tempsum = array[i];
        for (j=i+1; j<length; j++){

            //tempsum hold sum of all elements from i to j index
            tempsum += array[j];

            if (tempsum > maxsum) {
                maxsum = tempsum;
            }
        }
    }

    return maxsum;
}

/* Kadane's Algorithm */
int
KadaneMaxSum(int array[], int length) {
    int i,j;
    int maxSumSoFar = 0;
    int maxSumEndsHere = 0;

    for (i=0; i<length; i++) {

        maxSumEndsHere += array[i];
        if (maxSumEndsHere < 0 ) {
            maxSumEndsHere = 0;
        }

        if (maxSumSoFar < maxSumEndsHere) {
            maxSumSoFar = maxSumEndsHere;
        }
    }

    return maxSumSoFar;
}
/* main Function */
int main ()
{
    int array[] = {-1,2,-3,4,-5,6,-5,2,3,4,-2};

    printf ("Maximum Subsum array value is : %d\r\n", maxsum(array, (sizeof(array)/sizeof(array[0]))));
    printf ("Kadane's Maximum Subsum array value is : %d\r\n", KadaneMaxSum(array, (sizeof(array)/sizeof(array[0]))));
}
