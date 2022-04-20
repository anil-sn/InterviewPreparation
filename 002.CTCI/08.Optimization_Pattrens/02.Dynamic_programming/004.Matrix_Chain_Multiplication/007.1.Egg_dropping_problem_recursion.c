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
int max(int a, int b) {
    return (a > b) ? a : b;
}

/* Function to get minimum number of
 trials needed in worst case with n eggs
 and k floors */
int eggDrop(int egg, int floor)
{
    // If there are no floors, then no
    // trials needed. OR if there is
    // one floor, one trial needed.
    if (floor == 1 || floor == 0)
        return floor;

    // We need f trials for one egg and f floors
    if (egg == 1)
        return floor;

    int min = INT_MAX, res;

    // Consider all droppings from 1st floor to kth floor and
    // return the minimum of these values plus 1.
    for (int k = 1; k <= floor; k++) {
        res = max(eggDrop(egg - 1, k - 1), eggDrop(egg, floor - k));
        if (res < min) {
            min = res;
        }
    }

    return min + 1;
}

/* Driver program to test to  printDups*/
int main()
{
    int n = 2, f = 10;
    printf("Minimum number of trials in worst case with %d eggs and %d floors is %d \n",
           n, f, eggDrop(n, f));
    return 0;
}