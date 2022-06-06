#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

//https://www.baeldung.com/java-kth-largest-element


void swap(int arr[], int n1, int n2) 
{
    int temp = arr[n2];
    arr[n2] = arr[n1];
    arr[n1] = temp;
}

int partition(int arr[], int left, int right) {
    int pivot = arr[right];
    int i = left;
    for (int j = left; j <= right - 1; j++) {
        if (arr[j] <= pivot) {
            swap(arr, i, j);
            i++;
        }
    }

    swap(arr, i, right);
    return i;
}

int findKthElementByQuickSelect(int arr[], int left, int right, int k) 
{
    if (k >= 0 && k <= right - left + 1) 
    {
        int pos = partition(arr, left, right);
        if (pos - left == k) {
            return arr[pos];
        }

        if (pos - left > k) {
            return findKthElementByQuickSelect(arr, left, pos - 1, k);
        }

        return findKthElementByQuickSelect(arr, pos + 1, right, k - pos + left - 1);
    }

    return 0;
}

int main(void) {

    int a[] = {13, 3, 5, 7, 9, 11, 0};
    int alen = sizeof(a)/sizeof(a[0]);
    int k = 4;

    printf("The kth largest Element : %d", findKthElementByQuickSelect(a, 0, alen -1, k));

    return 0;
}

