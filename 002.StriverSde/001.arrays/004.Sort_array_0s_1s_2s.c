#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void swap(int nums[], int i, int j) {
    int temp = 0;
    temp = nums[i];
    nums[i] = nums[j];
    nums[j] = temp;
}

//Dutch National flag algorithm
void sortColors(int nums[], int n)
{
    int lo = 0;
    int hi = n - 1;
    int mid = 0;
    while (mid <= hi) {
        switch (nums[mid]) {
        case 0: // If the element is 0
            swap(nums, lo, mid);
            lo++; mid++;
            break;
        case 1: // If the element is 1 .
            mid++;
            break;
        case 2: // If the element is 2
            swap(nums, mid, hi);
            hi--;
            break;
        }
    }
}

int main ()
{
    int array[] = {1,2,1,2,0,0,1,2,0,1};
    int n = sizeof(array)/sizeof(array[0]);

    sortColors(array, n);
    for (int i = 0; i < n; i++) {
        printf("%d", array[i]);
    }
    printf("\r\n");
}