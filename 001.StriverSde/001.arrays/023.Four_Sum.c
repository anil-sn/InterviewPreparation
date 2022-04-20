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
    4 Sum | Find Quads that add up to a target value
    Problem Statement: Given an array of N integers, your task is to find unique quads
    that add up to give a target value. In short, you need to return an array of all
    the unique quadruplets [arr[a], arr[b], arr[c], arr[d]] such that their sum is
    equal to a given target.

    Pre-req: Binary Search and 2-sum problem

    Note:

        0 <= a, b, c, d < n
        a, b, c, and d are distinct.
        arr[a] + arr[b] + arr[c] + arr[d] == target

    Example 1:
        Input Format: arr[] = [1,0,-1,0,-2,2], target = 0
        Result: [[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]

        Explanation: We have to find unique quadruplets from
        the array such that the sum of those elements is
        equal to the target sum given that is 0.

        The result obtained is such that the sum of the
        quadruplets yields 0.

    Example 2:
        Input Format: arr[] = [4,3,3,4,4,2,1,2,1,1], target = 9
        Result: [[1,1,3,4],[1,2,2,4],[1,2,3,3]]
*/
//void *bsearch(const void *key, const void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *))

int cmp (const void *a, const void *b) {
    int x = *((int *)a);
    int y = *((int *)b);
    return (x - y);
}

void fourSum(int v[], int len, int target) {

    int *result = NULL;
    int index = 0;

    qsort(v, len, sizeof(int), cmp);
    for(int i = 0; i < len; i++) {
        for(int j = i+1; j < len; j++) {
            for(int k = j+1; k < len; k++) {
                int x = (long long)target - (long long)v[i] - (long long)v[j] - (long long)v[k];
                for (int l = k+1; l < len; l++) {
                    if (v[l] == x) {
                        printf("v[%d]=%d, v[%d]=%d, v[%d]=%d, v[%d]=%d \r\n", i, v[i],
                                                                              j, v[j],
                                                                              k, v[k],
                                                                              l, v[l]);
                    }
                }
            }
        }
    }

    return;
}

void fourSumPointer(int v[], int len, int target) {

    if (len == 0) {
        return;
    }

    int *result = NULL;
    int index = 0;

    qsort(v, len, sizeof(int), cmp);
    for (int i = 0; i < len; i++) {
        printf("%d ", v[i]);
    }
    printf("\r\n");

    for(int i = 0; i < len; i++) {

        int target_3 = target - v[i];

        for(int j = i+1; j < len; j++) {

            int target_2 = target_3 - v[j];

            int front = j+1;
            int back = len-1;

            while (front < back){
                int two_sum = v[front]+v[back];
                if (two_sum < target_2) {
                    front++;
                } else if (two_sum > target_2) {
                    back--;
                } else {
                    printf("v[%d]=%d, v[%d]=%d, v[%d]=%d, v[%d]=%d \r\n", i, v[i],
                                                                          j, v[j],
                                                                          front, v[front],
                                                                          back, v[back]);
                    front++;
                    back--;
                }
            }
        }
    }

    return;
}

int main()
{
    int v[] = {1, 0, -1, 0, -2, 2};
    int len = sizeof(v)/sizeof(v[0]);
    int target = 0;

    //fourSum(v,len, target);
    fourSumPointer(v, len, target);
}