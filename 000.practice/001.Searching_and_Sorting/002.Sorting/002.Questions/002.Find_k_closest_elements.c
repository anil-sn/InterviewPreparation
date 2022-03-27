#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Swap two integers
void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

// A utility function to print a given array of given size
void printArray(int *arr, int size) {
    int i;

    for (i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }

    printf("\r\n");
}

void findKthClosest(int arr[], int size, int n, int k)
{
    int i = 0;
    int index = 0;
    for(i = 0; arr[i] == k || i < n; i++);
    printf("i is %d k is %d\r\n", i, k);

    index = i-1;
    int left = -1;
    int right = -1;
    int rvalue = INT_MAX;
    int lvalue = INT_MAX;

    for (int count = 1; count <= n;){
        if (-1 == left){
            left = index-1;
        }
        if (-1 == right){
            right = index+1;
        }

        if(left >= 0) {
            lvalue = k - arr[left];
        }

        if (right < size){
            rvalue = arr[right] - k;
        }

        if ((lvalue <= rvalue) && (left >= 0)){
            printf ("%d ", arr[left]);
            left--;
            count++;
        } else if (right < size){
            printf ("%d ", arr[right]);
            right++;
            count++;
        }
    }

    printf("\r\n");
    return;
}

int main() {
    int arr[] ={12, 16, 22, 30, 35, 39, 42, 45, 48, 50, 53, 55, 56};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    int n = 4;
    int k = 35;

    printf("\nGiven Array is \n");
    printArray(arr, arr_size);
    findKthClosest(arr, arr_size, n, k);

    return 0;
}

