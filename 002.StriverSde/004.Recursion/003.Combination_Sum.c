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
    Combination Sum – 1

    Problem Statement:
        Given an array of distinct integers and a target, you have to return the list of all
        unique combinations where the chosen numbers sum to target.
        You may return the combinations in any order.

        The same number may be chosen from the given array an unlimited number of times.
        Two combinations are unique if the frequency of at least one of the chosen numbers is different.

        It is guaranteed that the number of unique combinations that sum up to target
        is less than 150 combinations for the given input.

    Examples:

        Example 1:
            Input: array = [2,3,6,7], target = 7
            Output: [[2,2,3],[7]]
            Explanation: 2 and 3 are candidates, and 2 + 2 + 3 = 7. Note that 2 can be used multiple times.
                        7 is a candidate, and 7 = 7.
                        These are the only two combinations.
        Example 2:
            Input: array = [2], target = 1
            Output: []
            Explaination: No combination is possible.
*/

#define CMT //

int combination[100];
int pos;

void findCombinations(int ind, int arr[], int len, int target) {
    // printf("\tTrace: Considering arr[%d] <<<< %d >>>>, idx : %d, Target: %d Choice : ", ind, arr[ind], ind, target);
    // for(int i = 0; i < pos; i++) {
            // printf("%d ", combination[i]);
    // }
    // printf("\r\n");

    if (ind == len) {
        // printf("\t\tTrace: idx : %d, Target: %d \r\n", ind, target);
        if (target == 0) {
            // printf("\t\t\tTrace: idx : %d, Target: %d \r\n", ind, target);
            printf("Combination : ");
            for(int i = 0; i < pos; i++) {
                printf("%d ", combination[i]);
            }
            printf("\r\n");
        }
        return;
    }

    if (arr[ind] <= target) {
        combination[pos++] = arr[ind];
        findCombinations(ind, arr, len, target - arr[ind]);
        combination[pos--] = 0;
    }

    findCombinations(ind + 1, arr, len, target);
    // printf("Unsuccessful return from recursion \r\n");
    return;
}

void combinationSum(int candidates[], int len, int target) {

    /* First Index */
    findCombinations(0, candidates, len, target);
    return;
}

int main() {
    int arr[] = {2,3,6,7};
    int len = sizeof(arr)/sizeof(arr[0]);
    int target = 7;

    printf("Input : 2, 3, 6, 7 \r\nTarget Sum : %d \r\n\r\n", target);
    combinationSum(arr, len, target);
}
