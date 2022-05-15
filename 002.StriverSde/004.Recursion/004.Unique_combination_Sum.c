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
    Combination Sum II – Find all unique combinations
    Problem Statement: Given a collection of candidate numbers (candidates) and
        a target number (target), find all unique combinations in candidates where the
        candidate numbers sum to target. Each number in candidates may only be used once
        in the combination.
    Note: The solution set must not contain duplicate combinations.

    Examples:
    Example 1:

        Input: candidates = [10,1,2,7,6,1,5], target = 8
        Output:
                [[1,1,6],
                [1,2,5],
                [1,7],
                [2,6]]
        Explanation: These are the unique combinations whose sum is equal to target.

    Example 2:
        Input: candidates = [2,5,2,1,2], target = 5
        Output: [[1,2,2],[5]]
        Explanation: These are the unique combinations whose sum is equal to target.
*/

int combination[100];
int pos;

void findCombinations(int ind, int arr[], int len, int target)
{
    if (0 == target) {
        for (int i = 0; i < pos; i++) {
            printf("%d ", combination[i]);
        }

        printf("\r\n");
        return;
    }

    for (int i = ind; i < len; i++) {

        if (i > ind && arr[i] == arr[i - 1]) {
            continue;
        }

        if (arr[i] > target) {
                break;
        }

        combination[pos++] = arr[i];
        findCombinations(i + 1, arr, len, target - arr[i]);
        combination[pos--] = 0;
    }

    return;
}

int cmp (const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    return (x - y);
}

void combinationUniqueSum(int candidates[], int len, int target) {

    qsort(candidates, len, sizeof(int), cmp);
    /* First Index */
    findCombinations(0, candidates, len, target);
    return;
}

int main() {
    int arr[] = {10,1,2,7,6,1,5};
    int len = sizeof(arr)/sizeof(arr[0]);
    int target = 8;

    printf("Input : 10, 1, 2, 7, 6, 1, 5 \r\n Target Sum : %d \r\n\r\n", target);
    combinationUniqueSum(arr, len, target);
}
