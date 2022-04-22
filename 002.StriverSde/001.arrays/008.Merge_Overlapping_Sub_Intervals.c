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
    Merge Overlapping Sub-intervals
    Problem Statement: Given an array of intervals, merge all the overlapping intervals
    and return an array of non-overlapping intervals.

    Examples

    Example 1:
        Input: intervals=[[1,3],[2,6],[8,10],[15,18]]
        Output: [[1,6],[8,10],[15,18]]
        Explanation: Since intervals [1,3] and [2,6] are overlapping we can merge
            them to form [1,6] intervals.

    Example 2:
        Input: [[1,3],[2,6]]
        Output: [[1,6]]
        Explanation: Since intervals [1,3] and [2,6] are overlapping we can merge
            them to form [1,6] intervals.
*/

typedef struct interval_ {
    int start;
    int end;
} int_s;

int cmp(void const *a, void const *b) {
    return (((const int_s *)a)->start < ((const int_s *)b)->start ? -1 : 1);
}

int max(int a, int b) {
    return (a > b) ? (a) : (b);
}

void merge(int_s arr[], int count, int_s merged[], int *intervalPos)
{
    qsort(arr, count, sizeof(int_s), cmp);
    for (int i = 0; i < count; i++) {
        printf("%d %d \r\n", arr[i].start, arr[i].end);
    }

    for (int i = 0; i < count; i++) {
        if (0 == *intervalPos ||
            merged[(*intervalPos) - 1].end < arr[i].start) {

            merged[(*intervalPos)].start = arr[i].start;
            merged[(*intervalPos)].end = arr[i].end;
            (*intervalPos)++;
        } else {
            merged[(*intervalPos) - 1].end = max(merged[(*intervalPos) - 1].end, arr[i].end);
        }
    }

    return;
}

int main() {

    int_s arr[] = {{1, 3}, {2, 4}, {2, 6}, {8, 9}, {8, 10}, {9, 11}, {15, 18}, {16, 17}};
    int_s interval[100] = {{0,0}};
    int intervalPos = 0;
    int count = sizeof(arr)/sizeof(arr[0]);

    merge(arr, count, interval, &intervalPos);
    printf("Merged Overlapping Intervals are \r\n");

    for (int i = 0; i < intervalPos; i++) {
        printf("%d %d \r\n", interval[i].start, interval[i].end);
    }
}