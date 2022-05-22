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
    Example 1:
        Input: candies = 7, num_people = 4
        Output: [1,2,3,1]
        Explanation:
        On the first turn, ans[0] += 1, and the array is [1,0,0,0].
        On the second turn, ans[1] += 2, and the array is [1,2,0,0].
        On the third turn, ans[2] += 3, and the array is [1,2,3,0].
        On the fourth turn, ans[3] += 1 (because there is only one candy left), and the final array is [1,2,3,1].

    Example 2:
        Input: candies = 10, num_people = 3
        Output: [5,2,3]
        Explanation:
        On the first turn, ans[0] += 1, and the array is [1,0,0].
        On the second turn, ans[1] += 2, and the array is [1,2,0].
        On the third turn, ans[2] += 3, and the array is [1,2,3].
        On the fourth turn, ans[0] += 4, and the final array is [5,2,3].
*/

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

void
distributeCandies (int candies, int num_people, int people[num_people])
{
    int give = 0;

    for (give = 0; candies > 0; candies -= give) {
        people[give % num_people] +=  min (candies, (give + 1));
        give += 1;
    }

    return;
}

int
main (void)
{
    int candies = 7;
    int num_people = 4;
    int people[num_people];
    memset (people, 0, sizeof (people));
    distributeCandies (candies, num_people, people);
    printf ("[ ");

    for (int i = 0; i < num_people; i++) {
        printf ("%d ", people[i]);
    }

    printf ("]\r\n");
}