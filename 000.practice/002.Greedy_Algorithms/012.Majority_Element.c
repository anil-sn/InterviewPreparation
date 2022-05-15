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

/*
    Write a function which takes an array and prints the majority element (if it exists),
    otherwise prints “No Majority Element”. A majority element in an array A[] of size n is
    an element that appears more than n/2 times (and hence there is at most one such element).

    Examples :

        Input : {3, 3, 4, 2, 4, 4, 2, 4, 4}
        Output : 4
        Explanation: The frequency of 4 is 5 which is greater than the half of the size of the array size.

        Input : {3, 3, 4, 2, 4, 4, 2, 4}
        Output : No Majority Element
        Explanation: There is no element whose frequency is greater than the half of the size of the array size.
*/

int majorityElement(int a[], int size)
{

    int ansIndex = a[0], count = 1;

    for (int i = 1; i < size; i++)
    {
        if (a[i] == ansIndex)
            count++;
        else
            count--;

        if (count == 0)
        {
            ansIndex = a[i];
            count = 1;
        }
    }

    count = 0;
    for (int i = 0; i < size; i++)
        if (a[i] == ansIndex)
            count++;
    if (count > size / 2)
        return ansIndex;
    return -1;
}

int main (){
    int N = 5;
    int A[] = {3,1,3,3,2};
    printf ("The majority element count is : %d\r\n", majorityElement(A, N));

    return 0;
}