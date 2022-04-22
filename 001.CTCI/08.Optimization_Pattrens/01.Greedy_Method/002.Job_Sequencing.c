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

/*
    Given an array of jobs where every job has a deadline and associated profit
    if the job is finished before the deadline. It is also given that every job takes
    a single unit of time, so the minimum possible deadline for any job is 1.
    How to maximize total profit if only one job can be scheduled at a time.

    Examples:

    Input: Four Jobs with following
    deadlines and profits
    JobID  Deadline  Profit
    a      4        20
    b      1        10
    c      1        40
    d      1        30
    Output: Following is maximum profit sequence of jobs
            c, a

    Input:  Five Jobs with following
    deadlines and profits
    JobID   Deadline  Profit
    a       2        100
    b       1        19
    c       2        27
    d       1        25
    e       3        15
    Output: Following is maximum
    profit sequence of jobs
            c, a, e

    A Simple Solution is to generate all subsets of a given set of jobs and
    check individual subsets for the feasibility of jobs in that subset.
    Keep track of maximum profit among all feasible subsets.
    The time complexity of this solution is exponential.

    This is a standard Greedy Algorithm problem.

    Following is the algorithm.

    1) Sort all jobs in decreasing order of profit.
    2) Iterate on jobs in decreasing order of profit.For each job , do the following :
        a) Find a time slot i, such that slot is empty and i < deadline and i is greatest.
           Put the job in this slot and mark this slot filled.
        b) If no such i exists, then ignore the job.
*/

// A structure to represent a job
typedef struct Job
{
   char id;     // Job Id
   int dead;    // Deadline of job
   int profit;  // Profit if job is over before or on deadline
} JOB;

// Comparison function to sort Item according to val/weight ratio
int cmp(const void *x, const void *y)
{
    JOB *a = (JOB *)x;
    JOB *b = (JOB *)y;

    return a->profit > b->profit;
}

void
printJobs(JOB arr[], int n)
{
    for (int i = 0; i <n; i++){
        printf("ID : %d, Deadline : %d, Profit : %d\r\n", arr[i].id, arr[i].dead, arr[i].profit);
    }
}

// Returns minimum number of platforms required
void printJobScheduling(JOB arr[], int n)
{
    printf("Before Sorting :\r\n");
    printJobs(arr, n);

    // sorting Item on basis of ratio
    qsort(arr, n, sizeof(arr[0]), cmp);

    printf("After Sorting :\r\n");
    printJobs(arr, n);

    int result[n]; // To store result (Sequence of jobs)
    bool slot[n];  // To keep track of free time slots

    // Initialize all slots to be free
    for (int i=0; i<n; i++) {
        slot[i] = false;
        result[i] = -1;
    }

    // Iterate through all given jobs
    for (int i=0; i<n; i++)
    {
       // Find a free slot for this job (Note that we start
       // from the last possible slot)
       for (int j=min(n, arr[i].dead)-1; j>=0; j--)
       {
          // Free slot found
          if (slot[j]==false)
          {
             result[j] = i;  // Add this job to result
             slot[j] = true; // Make this slot occupied
             break;
          }
       }
    }

    printf ("\r\nResult & Slot Index : \r\n");
    for (int i =0; i <n; i++){
        printf("%d[%s] ", result[i], slot[i] ? "TRUE" : "FALSE");
    }

    printf("\r\n\r\nFollowing is maximum profit sequence of jobs : ");

    // Print the result
    for (int i=0; i<n; i++)
       if (slot[i])
         printf("ID -> %d, ", arr[result[i]].id);

    printf ("\r\n");
}

// Driver code
int main()
{
    JOB arr[] = { {'a', 2, 100},
                  {'b', 1, 19},
                  {'c', 2, 27},
                  {'d', 1, 25},
                  {'e', 3, 15}};

    int n = sizeof(arr)/sizeof(arr[0]);


    // Function call
    printJobScheduling(arr, n);
    return 0;
}