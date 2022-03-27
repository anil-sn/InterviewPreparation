#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

void KswapPermutation(
    int arr[], int n, int k)
{
    for(int i=0;i<n-1;i++)
    {
        if( k>0)
        {
            int max = i;
            for(int j=i+1;j<n;j++)
            if(arr[j]>arr[max])
            max = j;

          // this condition checks whether the max value has changed since when
          // we started , or is it the same.
          // We need to ignore the swap if the value is same.
          // It means that the number ought to be present at the ith position , is already
          // there.
            if(max!=i)
            {
            int temp = arr[max];
            arr[max] = arr[i];
            arr[i] = temp;
            k--;
            }
        }
        else
        break;
    }
}

// Driver code
int main()
{
    int arr[] = { 4, 5, 2, 1, 3, 8, 9 };
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 3;

    for (int i = 0; i < n; ++i)
        printf ("%d ", arr[i]);

    KswapPermutation(arr, n, k);

    printf("\r\nLargest permutation after %d swaps\r\n", k);
    for (int i = 0; i < n; ++i)
        printf ("%d ", arr[i]);

    printf ("\r\n");
    return 0;
}