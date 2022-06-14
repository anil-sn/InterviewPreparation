#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void swap(int arr[], int a, int b)
{
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

void sort(int len, int arr[])
{
    int l = 0;
    int m = 0;
    int h = len - 1;

    while (m <= h) {
        if (arr[m] == 0) {
            swap(arr, l, m);
            l++;
            m++;
        } else if (arr[m] == 1) {
            m++;
        } else if (arr[m] == 2) {
            swap(arr, m, h);
            h--;
        }
    }
}

/* Driver code*/
int main(void)
{
    int arr[] = { 0, 1, 2, 0, 1, 2 };
    int len = sizeof(arr)/sizeof(arr[0]);

    printf("Orginal : ");
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");

    sort(len, arr);

    printf("Sorted : ");
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");

    return 0;
}