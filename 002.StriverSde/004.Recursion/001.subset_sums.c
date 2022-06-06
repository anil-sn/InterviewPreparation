#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#include <sys/wait.h>
#include <unistd.h>
#include <sys/prctl.h>

void print_trace() {
    return;
}

/*
    Subset Sum : Sum of all Subsets
    Problem Statement: Given an array print all the sum of the subset generated from it,
    in the increasing order.

    Examples:

        Example 1:
            Input: N = 3, arr[] = {5,2,1}
            Output: 0,1,2,3,5,6,7,8
            Explanation: We have to find all the subset’s sum and print them.
            in this case the generated subsets are [ [], [1], [2], [2,1], [5], [5,1], [5,2]. [5,2,1],
            so the sums we get will be  0,1,2,3,5,6,7,8

        Example 2:
            Input: N=3,arr[]= {3,1,2}
            Output: 0,1,2,3,3,4,5,6
            Explanation: We have to find all the subset’s sum and print them.
            in this case the generated subsets are [ [], [1], [2], [2,1], [3], [3,1], [3,2]. [3,2,1],
            so the sums we get will be  0,1,2,3,3,4,5,6
*/

int pos = 0;
int sums[100];

int cmp (const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    return (y - x);
}

void solve(int idx, int arr[], int n, int sum)
{
    if (idx == n) {
        sums[pos++] = sum;
        return;
    }

    solve(idx+1, arr, n, sum + 0);
    solve(idx+1, arr, n, sum + arr[idx]);
    return;
}

void printSubSetSum(int arr[], int len)
{
    qsort(arr, len, sizeof(int), cmp);

    printf("The Input : ");
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");

    solve(0, arr, len, 0);
    return;
}

int main ()
{
    int arr[] = {5,2,1};
    int len = sizeof(arr)/sizeof(arr[0]);
    printSubSetSum(arr, len);

    for (int i = 0; i < pos; i++){
        printf("%d ", sums[i]);
    }
    printf("\r\n");

    return;
}