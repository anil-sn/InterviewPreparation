#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns true if there is
// a subset of arr[] with sum equal to given sum
bool isSubsetSum(int array[], int len, int sum)
{

    bool MEM[len+1][sum+1];
    memset(MEM, 0, sizeof(MEM));

    // If sum is 0 and array is 0
    MEM[0][0] = true;

    // If sum is 0, then answer is true
    for (int i = 0; i <= len; i++)
        MEM[i][0] = true;

    // If sum is not 0 and set is empty,
    // then answer is false
    for (int i = 1; i <= sum; i++)
        MEM[0][i] = false;

    for (int i = 1; i <= len; i++) {
        for (int j = 1; j <= sum; j++) {
            if (j < array[i-1]) {
                MEM[i][j] = MEM[i-1][j];
            }
            if (j >= array[i-1]) {
                MEM[i][j] = MEM[i-1][j] || MEM[i-1][j - array[i-1]];
            }
        }
    }

    for (int i = 0; i < len+1; i++) {
        for (int j = 0; j < sum+1; j++) {
            printf("[%d][%d] = %d ",i, j, MEM[i][j]);
        }
        printf ("\r\n");
    }
    printf ("\r\n");

    return MEM[len][sum];
}

// Returns true if arr[] can be partitioned in two
//  subsets of equal sum, otherwise false
bool findPartiion(int arr[], int n)
{
    // Calculate sum of the elements in array
    int sum = 0;

    for (int i = 0; i < n; i++)
        sum += arr[i];

    // If sum is odd, there cannot be two subsets with equal sum
    if (sum % 2 != 0)
        return false;

    // Find if there is subset with sum equal to half of total sum
    return isSubsetSum(arr, n, sum / 2);
}

// Driver code
int main()
{
    int arr[] = { 3, 1, 5, 9, 12 };
    int n = sizeof(arr) / sizeof(arr[0]);

    // Function call
    if (findPartiion(arr, n) == true)
        printf("Can be divided into two subsets of equal sum \r\n");
    else
        printf("Can not be divided into two subsets of equal sum \r\n");
    return 0;
}