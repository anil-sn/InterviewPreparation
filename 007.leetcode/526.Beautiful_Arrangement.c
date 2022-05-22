
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
    Back tracking (swap) starting from the back
    The back tracking start from the back so that each search won't go too deep before it fails because smaller numbers have higher chance to be divisible among themselves. Also I don't use "visited" boolean array but use swap of an array of 1~N to avoid duplication.
*/

void
swap (int nums[], int i, int j)
{
    int tmp = nums[i];
    nums[i] = nums[j];
    nums[j] = tmp;
}

void
helper (int nums[], int start, int *count)
{
    if (start == 0) {
        (*count)++;
        return;
    }

    for (int i = start; i > 0; i--) {
        swap (nums, start, i);

        if (nums[start] % start == 0 || start % nums[start] == 0) {
            helper (nums, start - 1, count);
        }

        swap (nums, i, start);
    }
}

int
countArrangement (int N)
{
    int count = 0;

    if (N == 0) {
        return 0;
    }

    int nums[N + 1];
    memset (nums, 0, sizeof (nums));

    for (int i = 0; i <= N; i++) {
        nums[i] = i;
    }

    helper (nums, N, &count);
    return count;
}