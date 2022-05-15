#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int min (int a, int b) {
    return ((a < b) ? a : b) ;
}

bool **MEM;

void
updateSubSetSumTable(int array[], int len, int sum) {

    // If sum is 0 and array is 0
    MEM[0][0] = true;

    // If sum is 0, then answer is true
    printf (" Input Array [");
    for (int i = 0; i <= len; i++) {
        MEM[i][0] = true;
        // Use same loop to print array
        if (i != len) {
            printf(" %d ", array[i]);
        }
    }
    printf ("] Sum of all elements is : %d\r\n", sum);

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
            printf("%d,%d=%d  ",i, j, MEM[i][j]);
        }
        printf ("\r\n");
    }
    printf ("\r\n");

    return;
}


// Returns minimum possible difference between sums
// of two subsets
int findMin(int arr[], int n)
{
    // Compute total sum of elements
    int sumTotal = 0;
    for (int i = 0; i < n; i++)
        sumTotal += arr[i];

    MEM = (bool**)calloc(1, (n+1) * sizeof(bool*));
    for (int i = 0; i <= n; i++)
        MEM[i] = (bool*)calloc(1, (sumTotal+1) * sizeof(bool));

    updateSubSetSumTable(arr, n, sumTotal);

    int minimum = INT_MAX;
    for (int i=1; i <= sumTotal/2; i++){
        if (MEM[n][i]) {
            printf("%d,%d=%d  ",n, i, MEM[n][i]);
            printf("Min %d:Can %d\r\n",minimum, sumTotal - (2*i));
            minimum = min(minimum, sumTotal - (2*i));
        }
    }
    printf ("\r\n");

    return minimum;
}

// Driver program to test above function
int main()
{
    int arr1[] = { 1, 6, 11, 5 };
    int arr[] = { 1, 2, 7 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("The minimum difference between two sets is %d\r\n", findMin(arr, n));
    return 0;
}