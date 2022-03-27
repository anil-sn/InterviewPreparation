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
    Counting sort is a sorting technique based on keys between a specific range.
    It works by counting the number of objects having distinct key values (kind of hashing).
    Then doing some arithmetic to calculate the position of each object in the output sequence.

    Let us understand it with the help of an example.

    For simplicity, consider the data in the range 0 to 9.
    Input data: 1, 4, 1, 2, 7, 5, 2
    1) Take a count array to store the count of each unique object.
    Index:     0  1  2  3  4  5  6  7  8  9
    Count:     0  2  2  0   1  1  0  1  0  0

    2) Modify the count array such that each element at each index
    stores the sum of previous counts.
    Index:     0  1  2  3  4  5  6  7  8  9
    Count:     0  2  4  4  5  6  6  7  7  7

    The modified count array indicates the position of each object in
    the output sequence.

    3) Rotate the array clockwise for one time.
    Index:     0 1 2 3 4 5 6 7 8 9
    Count:     0 0 2 4 4 5 6 6 7 7

    4) Output each object from the input sequence followed by
    increasing its count by 1.
    Process the input data: 1, 4, 1, 2, 7, 5, 2. Position of 1 is 0.
    Put data 1 at index 0 in output. Increase count by 1 to place
    next data 1 at an index 1 greater than this index.
*/

#define RANGE 255

// The main function that sort the given string arr[] in
// alphabetical order
void countSort(char arr[])
{
    // The output character array that will have sorted arr
    char output[strlen(arr)];

    // Create a count array to store count of individual
    // characters and initialize count array as 0
    int count[RANGE + 1], i;
    memset(count, 0, sizeof(count));

    // Store count of each character
    for (i = 0; arr[i]; ++i)
        ++count[arr[i]];

    // Change count[i] so that count[i] now contains actual
    // position of this character in output array
    for (i = 1; i <= RANGE; ++i)
        count[i] += count[i - 1];

    // Build the output character array
    for (i = 0; arr[i]; ++i) {
        output[count[arr[i]] - 1] = arr[i];
        --count[arr[i]];
    }

    /*
     For Stable algorithm
     for (i = sizeof(arr)-1; i>=0; --i)
    {
        output[count[arr[i]]-1] = arr[i];
        --count[arr[i]];
    }
    */

    // Copy the output array to arr, so that arr now
    // contains sorted characters
    for (i = 0; arr[i]; ++i)
        arr[i] = output[i];
}


/* Driver program to test above functions */
int main()
{
    char arr[] = "anilKumarSNAnilKumar";

    countSort(arr);

    printf("Sorted character array is %s\r\n", arr);
    return 0;
}