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
    Subset – II | Print all the Unique Subsets
    Problem Statement: Given an array of integers that may contain duplicates the task is to
        return all possible subsets. Return only unique subsets and they can be in any order.

    Examples:

        Example 1:
        Input: array[] = [1,2,2]
        Output: [ [ ],[1],[1,2],[1,2,2],[2],[2,2] ]
        Explanation: We can have subsets ranging from  length 0 to 3. which are listed above.
        Also the subset [1,2] appears twice but is printed only once as we require only unique subsets.

        Example 2:
        Input: array[] = [1]
        Output: [ [ ], [1] ]
        Explanation: Only two unique subsets are available
*/

int cmp (const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    return (y - x);
}

void solve(int idx, int arr[], int n, int sum, int comb[])
{
    if (idx == n) {
        for (int i = 0; i < n; i++) {
            printf("%4d", comb[i]);
        }
        printf("\t:\tSum %4d\r\n", sum);
        return;
    }

    comb[idx] = -1;
    solve(idx + 1, arr, n, sum + 0       , comb);
    comb[idx] = arr[idx];
    solve(idx + 1, arr, n, sum + arr[idx], comb);
    comb[idx] = -1;

    return;
}

void printSubSetSum(int arr[], int len)
{
    int comb[len];
    memset(comb, -1, sizeof(comb));

    qsort(arr, len, sizeof(int), cmp);

    printf("\r\nThe Input : ");
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n\r\n");

    solve(0, arr, len, 0, comb);
    return;
}

int main ()
{
    setvbuf (stdout, NULL, _IONBF, BUFSIZ);
    int arr[] = {1,2,2};
    int len = sizeof(arr)/sizeof(arr[0]);

    printSubSetSum(arr, len);

    return;
}