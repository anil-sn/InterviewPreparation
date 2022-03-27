#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "math.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }
// Swap two integers
void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

/*
    Ternary search is a decrease(by constant) and conquer algorithm that can be used to
    find an element in an array. It is similar to binary search where we divide the array
    into two parts but in this algorithm, we divide the given array into three parts and
    determine which has the key (searched element). We can divide the array into three parts
    by taking mid1 and mid2 which can be calculated as shown below. Initially, l and r will
    be equal to 0 and n-1 respectively, where n is the length of the array.

    mid1 = l + (r-l)/3
    mid2 = r – (r-l)/3

    Note: Array needs to be sorted to perform ternary search on it.

    Steps to perform Ternary Search:

    First, we compare the key with the element at mid1. If found equal, we return mid1.
        If not, then we compare the key with the element at mid2. If found equal, we return mid2.
        If not, then we check whether the key is less than the element at mid1. If yes, then recur to the first part.
        If not, then we check whether the key is greater than the element at mid2. If yes, then recur to the third part.
        If not, then we recur to the second (middle) part.
*/

// Function to perform Ternary Search
int ternarySearch(int l, int r, int key, int ar[])
{
    if (r >= l) {

        // Find the mid1 and mid2
        int mid1 = l + (r - l) / 3;
        int mid2 = r - (r - l) / 3;

        // Check if key is present at any mid
        if (ar[mid1] == key) {
            return mid1;
        }
        if (ar[mid2] == key) {
            return mid2;
        }

        // Since key is not present at mid, check in which region it is present
        // then repeat the Search operation in that region
        if (key < ar[mid1]) {

            // The key lies in between l and mid1
            return ternarySearch(l, mid1 - 1, key, ar);
        }
        else if (key > ar[mid2]) {

            // The key lies in between mid2 and r
            return ternarySearch(mid2 + 1, r, key, ar);
        }
        else {

            // The key lies in between mid1 and mid2
            return ternarySearch(mid1 + 1, mid2 - 1, key, ar);
        }
    }

    // Key not found
    return -1;
}

// Driver code
int main()
{
    int l, r, p, key;
    int ar[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    l = 0;
    r = 9;
    key = 5;

    p = ternarySearch(l, r, key, ar);
    printf("Index of %d is %d\r\n", key, p);

    key = 50;
    p = ternarySearch(l, r, key, ar);
    printf("Index of %d is %d\r\n", key, p);
}