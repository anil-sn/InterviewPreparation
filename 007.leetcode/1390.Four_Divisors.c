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
    Given an integer array nums, return the sum of divisors of the integers in that array that have exactly four divisors. If there is no such integer in the array, return 0.

    Example 1:

        Input: nums = [21,4,7]
        Output: 32
        Explanation:
        21 has 4 divisors: 1, 3, 7, 21
        4 has 3 divisors: 1, 2, 4
        7 has 2 divisors: 1, 7
        The answer is the sum of divisors of 21 only.
*/

/*
    Intuition
    Any number n greater than 1 already have two divisors: 1 and n. So, for a number to have exact 4 divisors, there should be only one pair of divisors d and n / d.

    Catch: if d == n / d, the number has 3 divisors, not four.
*/
int
sumFourDivisors (int *nums, int numsSize)
{
    auto sum = 0;

    for (int i = 0; i < numsSize; i++) {
        int n = nums[i];
        int last_d = 0;

        for (int d = 2; d * d <= n; ++d) {
            if (n % d == 0) {
                if (last_d == 0) {
                    last_d = d;
                } else {
                    last_d = 0;
                    break;
                }
            }
        }

        if (last_d > 0 && last_d != n / last_d) {
            sum += 1 + n + last_d + n / last_d;
        }
    }

    return sum;
}