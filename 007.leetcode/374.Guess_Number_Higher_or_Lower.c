#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/**
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return       -1 if num is higher than the picked number
 *                1 if num is lower than the picked number
 *               otherwise return 0
 * int guess(int num);
 */
int pick = 6;
int
guess (int num)
{
    if (num > pick) {
        return -1;
    } else if (num < pick) {
        return 1;
    } else {
        return 0;
    }
}

int
guessNumber (int n)
{
    int low = 1;
    int high = n;
    int cmp = 0;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        cmp = guess (mid);

        if (0 == cmp) {
            return mid;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return -1;
}

int
main (void)
{
    int n = 10;
    printf ("The number that I picked : %d \r\n", guessNumber (n));
    return 0;
}