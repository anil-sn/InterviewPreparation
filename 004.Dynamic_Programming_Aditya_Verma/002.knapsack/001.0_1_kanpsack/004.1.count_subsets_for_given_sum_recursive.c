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
int subsetSumCount(int arr[], int n, int i, int sum, int count)
{
    // The recursion is stopped at N-th level
    // where all the subsets of the given array
    // have been checked
    if (i == n) {

        // Incrementing the count if sum is
        // equal to 0 and returning the count
        if (sum == 0) {
            count++;
        }
        return count;
    }

    // Recursively calling the function for two cases

    // Either the element can be counted in the subset
    // If the element is counted, then the remaining sum
    // to be checked is sum - the selected element
    count = subsetSumCount(arr, n, i + 1, sum - arr[i], count);

    // If the element is not included, then the remaining sum
    // to be checked is the total sum
    count = subsetSumCount(arr, n, i + 1, sum, count);
    return count;
}

int main (){
    int array[] = {1, 2, 3, 3, 6};
    int sum = 6;
    int len = sizeof(array)/sizeof(array[0]);

    printf("Is subset present in array [1, 2, 3, 3] with sum %d : %d\r\n",
            sum,
            subsetSumCount(array,len,0,sum,0));
}