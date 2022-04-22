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
    Count Reverse Pairs
    Problem Statement: Given an array of numbers, you need to return
    the count of reverse pairs. Reverse Pairs are those pairs
    where i<j and arr[i] > 2*arr[j].

    Examples:

        Example 1:
        Input: N = 5, array[] = {1,3,2,3,1)
        Output: 2

        Explanation: The pairs are (3, 1) and (3, 1) as from both
                    the pairs the condition arr[i] > 2*arr[j] is satisfied.

    Example 2:

        Input: N = 4, array[] = {3,2,1,4}
        Output: 1
        Explaination: There is only 1 pair  ( 3 , 1 ) that satisfy
                    the condition arr[i] > 2*arr[j]
*/

int reversePairsBrute(int arr[], int length) {

  int Pairs = 0;

  for (int i = 0; i < length; i++) {
    for (int j = i + 1; j < length; j++) {
      if (arr[i] > (2 * arr[j])) {
        Pairs++;
      }
    }
  }

  return Pairs;
}

int merge(int arr[], int low, int mid, int high)
{
    int j = mid + 1;
    int count = 0;

    for (int i = low; i <= mid; i++) {
        while (j <= high && arr[i] > (2 * (long) arr[j])) {
            j++;
        }
        count += (j - (mid+1));
    }

    int temp[high - low + 1];
    int pos = 0;
    memset(temp, 0, sizeof(temp));
    int left = low;
    int right = mid + 1;

    while(left <= mid && right <= high) {
        if(arr[left] <= arr[right]) {
            temp[pos++] = arr[left++];
        }
        else {
            temp[pos++] = arr[right++];
        }
    }

    while (left<=mid) {
        temp[pos++] = arr[left++];
    }

    while (right<=high) {
        temp[pos++] = arr[right++];
    }

    for(int i = low; i <= high; i++) {
        arr[i] = temp[i - low];
    }

    return count;
}

int mergesort(int arr[], int low, int high)  {

    if (low >= high) {
        return 0;
    }

    int pairs = 0;
    int mid = (low + high)/2;

    pairs += mergesort(arr, low, mid);
    pairs += mergesort(arr, mid + 1, high);
    pairs += merge(arr, low, mid, high);

    return pairs;
}


int reversePairsMergeSort(int arr[], int length) {

  int pairs = mergesort(arr, 0, length - 1);
  return pairs;
}

int main()
{
    int arr[] = {1,3,2,3,1};
    int length = sizeof(arr)/sizeof(arr[0]);
    printf("The Total Brute Force Reverse Pairs are %d \r\n", reversePairsBrute(arr, length));
    printf("The Total Merge Sort Reverse Pairs are %d \r\n", reversePairsMergeSort(arr, length));
}