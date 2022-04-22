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
    K-th Element of two sorted arrays
    Problem Statement: Given two sorted arrays of size m and n respectively,
    you are tasked with finding the element that would be at the kth position of the final sorted array.

    Examples :

    Input: m = 5, n = 4
        array1 = [2,3,6,7,9]
        array2 = [1,4,8,10]
        k = 5

    Output: 6

    Explanation: Merging both arrays and sorted. Final array will be - [1,2,3,4,6,7,8,9,10]
    We can see at k = 5 in the final array has 6.

    Input: m = 1 n = 4
        array1 = [0]
        array2 = [1,4,8,10]
        k = 2

    Output: 4

    Explanation:
    Merging both arrays and sorted. Final array will be - [1,4,8,10]
    We can see at k = 2 in the final array has 4
*/

int kthelement(int array1[], int array2[], int m, int n, int k)
{
    int p1 = 0;
    int p2 = 0;
    int counter = 0;
    int answer = 0;

    while(p1 < m && p2 < n) {
        if (counter == k) {
            break;
        } else if (array1[p1] < array2[p2]) {
            answer = array1[p1];
            ++p1;
        } else {
            answer = array2[p2];
            ++p2;
        }

        ++counter;
    }

    if(counter != k) {
        if(p1 != m-1)
            answer = array1[k-counter];
        else
            answer = array2[k-counter];
    }

    return answer;
}

#define MAX(a_, b_) (a_ > b_ ? a_ : b_)
#define MIN(a_, b_) (a_ < b_ ? a_ : b_)

int kthelementBinary(int arr1[], int arr2[], int m, int n, int k)
{
    if(m > n) {
        return kthelement(arr2, arr1, n, m, k);
    }

    int low = MAX(0,k-m);
    int high = MIN(k,n);

    while(low <= high) {
        int cut1 = (low + high) >> 1;
        int cut2 = k - cut1;
        int l1 = cut1 == 0 ? INT_MIN : arr1[cut1 - 1];
        int l2 = cut2 == 0 ? INT_MIN : arr2[cut2 - 1];
        int r1 = cut1 == n ? INT_MAX : arr1[cut1];
        int r2 = cut2 == m ? INT_MAX : arr2[cut2];

        if(l1 <= r2 && l2 <= r1) {
            return MAX(l1, l2);
        }
        else if (l1 > r2) {
            high = cut1 - 1;
        }
        else {
            low = cut1 + 1;
        }
    }
    return 1;
}

int main() {
    int array1[] = {2,3,6,7,9};
    int array2[] = {1,4,8,10};
    int m = sizeof(array1)/sizeof(array1[0]);
    int n = sizeof(array2)/sizeof(array2[0]);
    int k = 5;

    printf("The element at the kth position in the final sorted array is %d \r\n" , kthelement(array1,array2,m,n,k));
    printf("The element at the kth position in the final sorted array is %d \r\n" , kthelementBinary(array1,array2,m,n,k));

    return 0;
}