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
    Job Sequencing Problem
    Problem Statement: You are given a set of N jobs where each job comes with a deadline and profit.
                       The profit can only be earned upon completing the job within its deadline.
                       Find the number of jobs done and the maximum profit that can be obtained.
                       Each job takes a single unit of time and only one job can be performed at a time.
    Examples
        Example 1:
            Input : N = 4, Jobs = {(1,4,20),(2,1,10),(3,1,40),(4,1,30)}
            Output: 2 60
            Explanation: The 3rd job with a deadline 1 is performed during the first unit of time .
                         The 1st job is performed during the second unit of time as its deadline is 4.
            Profit = 40 + 20 = 60

        Example 2:
            Input : N = 5, Jobs = {(1,2,100),(2,1,19),(3,2,27),(4,1,25),(5,1,15)}
            Output: 2 127
            Explanation: The  first and third job both having a deadline 2 give the highest profit.
            Profit = 100 + 27 = 127
*/

#define MAX(a_,b_) (a_ > b_ ? a_ : b_)

typedef struct job_ {
    int slno;
    int deadline;
    int profit;
} JOB;

int comparison(const void *a, const void *b)
{
    JOB *x = (JOB *)a;
    JOB *y = (JOB *)b;
    return (y->profit - x->profit);
}

//Function to find the maximum profit and the number of jobs done
void JobScheduling(JOB arr[], int n) {

    qsort(arr, n, sizeof(JOB), comparison);

    /* Input Job schedule */
    for (int i = 1; i < n; i++) {
        printf("%d.  Deadline:%d  Profit:%d \r\n", arr[i].slno, arr[i].deadline, arr[i].profit);
    }

    int maxi = arr[0].deadline;
    for (int i = 1; i < n; i++) {
        maxi = MAX(maxi, arr[i].deadline);
    }

    int slot[maxi + 1];
    for (int i = 0; i <= maxi; i++) {
        slot[i] = -1;
    }

    int countJobs = 0;
    int jobProfit = 0;
    for (int i = 0; i < n; i++) {
        for (int j = arr[i].deadline; j > 0; j--) {
            if (slot[j] == -1) {
                slot[j] = i;
                countJobs++;
                jobProfit += arr[i].profit;
                break;
            }
        }
    }

    printf("\r\nMax number of jobs %d, Max number of jobs %d \r\n", countJobs, jobProfit);
    return;
}

int main ()
{
    JOB arr[] = {{1,4,20},{2,1,10},{3,2,40},{4,2,30}};
    int n = sizeof(arr)/sizeof(arr[0]);

    JobScheduling(arr, n);

    return;
}