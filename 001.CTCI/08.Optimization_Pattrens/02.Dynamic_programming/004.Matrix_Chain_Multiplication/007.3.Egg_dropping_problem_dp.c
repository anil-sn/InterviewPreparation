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
    k ==> Number of floors
    n ==> Number of Eggs

    eggDrop(n, k) ==> Minimum number of trials needed to find the critical floor in worst case.
    eggDrop(n, k) = 1 + min{max(eggDrop(n – 1, x – 1), eggDrop(n, k – x)),
    where x is in {1, 2, …, k}}

    Concept of worst case:
    For example :
        Let there be ‘2’ eggs and ‘2’ floors then-:
        If we try throwing from ‘1st’ floor:

        Number of tries in worst case= 1+max(0, 1)

        0=>If the egg breaks from first floor then it is threshold floor (best case possibility).
        1=>If the egg does not break from first floor we will now have ‘2’ eggs and 1 floor
        to test which will give answer as ‘1’.(worst case possibility)

        We take the worst case possibility in account, so 1+max(0, 1)=2

        If we try throwing from ‘2nd’ floor:
            Number of tries in worst case= 1+max(1, 0)
            1=>If the egg breaks from second floor then we will have 1 egg and 1 floor to find threshold floor.(Worst Case)
            0=>If egg does not break from second floor then it is threshold floor.(Best Case)

        We take worst case possibility for surety, so 1+max(1, 0)=2.
        The final answer is min(1st, 2nd, 3rd….., kth floor)

        So answer here is ‘2’.
*/
// A utility function to get
// maximum of two integers
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/* Function to get minimum
number of trials needed in worst
case with n eggs and k floors */
int eggDrop(int n, int k)
{
    /* A 2D table where entry eggFloor[i][j] will represent
       minimum number of trials needed for i eggs and j floors. */
    int eggFloor[n + 1][k + 1];
    int res;
    int i, j, x;
    int low, high;
    memset(eggFloor, 0, sizeof(eggFloor));

    // We need one trial for one floor and 0
    // trials for 0 floors
    for (i = 1; i <= n; i++) {
        eggFloor[i][1] = 1;
        eggFloor[i][0] = 0;
    }

    // We always need j trials for one egg
    // and j floors.
    for (j = 1; j <= k; j++)
        eggFloor[1][j] = j;

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= k; j++) {
            printf("%5d", eggFloor[i][j]);
        }
        printf ("\r\n");
    }
    printf ("\r\n");

    // Fill rest of the entries in table using
    // optimal substructure property
    for (i = 2; i <= n; i++) {
        for (j = 2; j <= k; j++) {
            eggFloor[i][j] = INT_MAX;
            for (x = 1; x <= j; x++) {
                low = eggFloor[i - 1][x - 1];
                high = eggFloor[i][j - x];
                res = 1 + max(low, high);
                if (res < eggFloor[i][j]) {
                    eggFloor[i][j] = res;
                }
            }
        }
    }

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= k; j++) {
            printf("%5d", eggFloor[i][j]);
        }
        printf ("\r\n");
    }
    printf ("\r\n");

    // eggFloor[n][k] holds the result
    return eggFloor[n][k];
}

/* Driver program to test to printDups*/
int main()
{
    int n = 2, k = 10;
    printf("\nMinimum number of trials in worst case with %d eggs and %d floors is %d \n",
           n, k, eggDrop(n, k));
    return 0;
}