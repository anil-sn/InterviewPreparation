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
    Print All Permutations of a String/Array
    Problem Statement: Given an array arr of distinct integers, print all permutations of String/Array.

    Examples:
        Example 1:
            Input: arr = [1, 2, 3]
            Output:
            [
                [1, 2, 3],
                [1, 3, 2],
                [2, 1, 3],
                [2, 3, 1],
                [3, 1, 2],
                [3, 2, 1]
            ]
            Explanation: Given an array, return all the possible permutations.

        Example 2:

            Input: arr = [0, 1]
            Output:
            [
                [0, 1],
                [1, 0]
            ]
            Explanation: Given an array, return all the possible permutations.
*/
#define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)

void recurPermute(int nums[], int len, int freq[], int ans[], int pos)
{
    if (pos == len) {
        for(int i = 0; i < len; i++) {
            printf("%d ", ans[i]);
        }
        printf("\r\n");
        return;
    }

    for (int i = 0; i < len; i++) {
        if (0 == freq[i]) {
            ans[pos++] = nums[i];
            freq[i] = 1;
            recurPermute(nums, len, freq, ans, pos);
            freq[i] = 0;
            ans[pos--] = 0;
        }
    }
}

void recurPermuteBackTrack(int index, int nums[], int len) {
    if (index == len) {
        for(int i = 0; i < len; i++) {
            printf("%d ", nums[i]);
        }
        printf("\r\n");
        return;
    }

    for (int i = index; i < len; i++) {
        SWAP(nums[index], nums[i], int);
        recurPermuteBackTrack(index + 1, nums, len);
        SWAP(nums[index], nums[i], int);
    }
}

void permute(int nums[], int len)
{
    int freq[len];
    int ans[len];
    int pos = 0;
    for (int i = 0; i < len; i++) {
        freq[i] = 0;
        ans[i] = 0;
    }

    printf("\r\n Plain Recursion : \r\n");
    recurPermute(nums, len, freq, ans, pos);
    printf("\r\n Recursion with backtracking : \r\n");
    recurPermuteBackTrack(0, nums, len);
    return;
}

int main() {
    int v[] = {1,2,3};
    int len = sizeof(v)/sizeof(v[0]);
    permute(v, len);
}