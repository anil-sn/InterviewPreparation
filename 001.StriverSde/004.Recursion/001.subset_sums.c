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

#define BACK_TRACE false

#if (BACK_TRACE == true)
void print_trace() {
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)]=0;
    prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY, 0, 0, 0);
    int child_pid = fork();
    if (!child_pid) {
        dup2(2,0); // redirect output to stderr - edit: unnecessary?
        execl("/usr/bin/gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", name_buf, pid_buf, NULL);
        abort(); /* If gdb failed to start */
    } else {
        waitpid(child_pid, NULL, 0);
    }
}
#else
void print_trace() {
    return;
}
#endif
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
#if (BACK_TRACE == true)
    printf("\tTrace: idx : %d, sum: %d \r\n", idx, sum);
    print_trace();
#endif
    if (idx == n) {
#if (BACK_TRACE == true)
        printf("\t\t Trace: Result Sum : %d, sum: %d \r\n\r\n\r\n", pos, sum);
#endif
        sums[pos++] = sum;
        return;
    }

#if (BACK_TRACE == true)
    printf("Trace: solve(idx(%d)+1, arr, n, sum(%d) + 0); \r\n", idx, sum);
#endif
    solve(idx+1, arr, n, sum + 0);
#if (BACK_TRACE == true)
    printf("Trace: solve(idx(%d)+1, arr, n, sum(%d) + arr[idx](%d)); \r\n", idx, sum, arr[idx]);
#endif
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

#if (BACK_TRACE == true)
    printf("Trace: solve(0, arr, len, 0); \r\n");
#endif
    solve(0, arr, len, 0);
    return;
}

void main ()
{
    setvbuf (stdout, NULL, _IONBF, BUFSIZ);
    int arr[] = {5,2,1};
    int len = sizeof(arr)/sizeof(arr[0]);
    printSubSetSum(arr, len);

    for (int i = 0; i < pos; i++){
        printf("%d ", sums[i]);
    }
    printf("\r\n");

    return;
}