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
    There is some frustration when people publish their perfect fine-grained algorithms without
    sharing any information abut how they were derived. This is an attempt to change the situation.
    There is not much more explanation but it's rather an example of higher level improvements.
    Converting a solution to the next step shouldn't be as hard as attempting to come up with perfect algorithm at first attempt.

    This particular problem and most of others can be approached using the following sequence:

    Find recursive relation
    Recursive (top-down)
    Recursive + memo (top-down)
    Iterative + memo (bottom-up)
    Iterative + N variables (bottom-up)
*/

/*
    Step 1. Figure out recursive relation.
    A robber has 2 options:
        a) rob current house i;
        b) don't rob current house.
    If an option "a" is selected it means she can't rob previous i-1 house but can safely proceed to the one before previous i-2
        and gets all cumulative loot that follows.
    If an option "b" is selected the robber gets all the possible loot from robbery of i-1 and all the following buildings.
    So it boils down to calculating what is more profitable:

    robbery of current house + loot from houses before the previous
    loot from the previous house robbery and any loot captured before that

    rob(i) = MAX( rob(i - 2) + currentHouseValue, rob(i - 1) )
*/

/*
    Step 2. Recursive (top-down)
    Converting the recurrent relation from Step 1 shound't be very hard.
*/
int
rob1 (int k, int houses[])
{
    if (k < 0) {
        return 0;
    }

    return MAX (rob1 (k - 2, houses) + houses[k], rob1 (k - 1, houses));
}

int
robHouses1 (int nums[], int n)
{
    return rob1 (nums, n - 1);
}

/*
    This algorithm will process the same i multiple times and it needs improvement. Time complexity: [to fill]
    Step 3. Recursive + memo (top-down).
*/

int
rob2 (int houses[], int i, int memo[])
{
    if (i < 0) {
        return 0;
    }

    if (memo[i] != -1) {
        return memo[i];
    }

    int result = MAX (rob2 (houses, i - 2, memo) + houses[i], rob2 (houses, i - 1, memo));
    memo[i] = result;
    return result;
}

int
robHouses2 (int nums[], int n)
{
    int memo[n + 1];
    memset (memo, -1, sizeof (memo));
    return rob2 (nums, n - 1, memo);
}

/*
    Much better, this should run in O(n) time.
    Space complexity is O(n) as well, because of the recursion stack, let's try to get rid of it.
    Step 4. Iterative + memo (bottom-up)
*/
int
rob3 (int houses[], int n)
{
    if (n == 0) {
        return 0;
    }

    int memo[n + 1];
    memset (memo, 0, sizeof (memo));
    memo[0] = 0;
    memo[1] = houses[0];

    for (int i = 1; i < n; i++) {
        int val = houses[i];
        memo[i + 1] = MAX (memo[i], memo[i - 1] + val);
    }

    return memo[n];
}

/*
    Step 5. Iterative + 2 variables (bottom-up)
    We can notice that in the previous step we use only memo[i] and memo[i-1], so going just 2 steps back.
    We can hold them in 2 variables instead. This optimization is met in Fibonacci sequence creation and some other problems [to paste links].
*/
/* the order is: prev2, prev1, num  */
int
rob4 (int houses[], int n)
{
    if (n == 0) {
        return 0;
    }

    int prev1 = 0;
    int prev2 = 0;

    for (int i = 0; i < n; i++) {
        int tmp = prev1;
        prev1 = MAX (prev2 + houses[i], prev1);
        prev2 = tmp;
    }

    return prev1;
}