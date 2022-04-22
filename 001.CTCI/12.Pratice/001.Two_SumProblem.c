// C program to check if given array
// has 2 elements whose sum is equal
// to the given value

/*
Sorting and Two-Pointers technique.

Approach: A tricky approach to solve this problem can be to use the two-pointer technique. But for using two pointer technique, the array must be sorted. Once the array is sorted the two pointers can be taken which mark the beginning and end of the array respectively. If the sum is greater than the sum of those two elements, shift the right pointer to decrease the value of required sum and if the sum is lesser than the required value, shift the left pointer to increase the value of the required sum. Let’s understand this using an example.

Let an array be {1, 4, 45, 6, 10, -8} and sum to find be 16
After sorting the array
A = {-8, 1, 4, 6, 10, 45}
Now, increment ‘l’ when the sum of the pair is less than the required sum and decrement ‘r’ when the sum of the pair is more than the required sum.
This is because when the sum is less than the required sum then to get the number which could increase the sum of pair, start moving from left to right(also sort the array) thus “l++” and vice versa.
Initialize l = 0, r = 5
A[l] + A[r] ( -8 + 45) > 16 => decrement r. Now r = 4
A[l] + A[r] ( -8 + 10) increment l. Now l = 1
A[l] + A[r] ( 1 + 10) increment l. Now l = 2
A[l] + A[r] ( 4 + 10) increment l. Now l = 3
A[l] + A[r] ( 6 + 10) == 16 => Found candidates (return 1)

Note: If there is more than one pair having the given sum then this algorithm reports only one. Can be easily extended for this though.

Algorithm:

hasArrayTwoCandidates (A[], ar_size, sum)
Sort the array in non-decreasing order.
Initialize two index variables to find the candidate
elements in the sorted array.
Initialize first to the leftmost index: l = 0
Initialize second the rightmost index: r = ar_size-1
Loop while l < r.
If (A[l] + A[r] == sum) then return 1
Else if( A[l] + A[r] < sum ) then l++
Else r–
No candidates in the whole array – return 0
*/

#include <stdio.h>
#define bool int

/*
    Dont practice until you get it right
    practice until you can't get it worng
*/

void quickSort(int*, int, int);

bool hasArrayTwoCandidates(
    int A[], int arr_size, int sum) {
    int l, r;

    /* Sort the elements */
    quickSort(A, 0, arr_size - 1);

    /* Now look for the two candidates in the sorted
       array*/
    l = 0;
    r = arr_size - 1;
    while (l < r) {
        if (A[l] + A[r] == sum)
            return 1;
        else if (A[l] + A[r] < sum)
            l++;
        else // A[i] + A[j] > sum
            r--;
    }
    return 0;
}

/* FOLLOWING FUNCTIONS ARE ONLY FOR SORTING
    PURPOSE */
void exchange(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int A[], int si, int ei) {
    int x = A[ei];
    int i = (si - 1);
    int j;

    for (j = si; j <= ei - 1; j++) {
        if (A[j] <= x) {
            i++;
            exchange(&A[i], &A[j]);
        }
    }
    exchange(&A[i + 1], &A[ei]);
    return (i + 1);
}

/* Implementation of Quick Sort
A[] --> Array to be sorted
si  --> Starting index
ei  --> Ending index
*/
void quickSort(int A[], int si, int ei) {
    int pi; /* Partitioning index */
    if (si < ei) {
        pi = partition(A, si, ei);
        quickSort(A, si, pi - 1);
        quickSort(A, pi + 1, ei);
    }
}

/* Driver program to test above function */
int main() {
    int A[] = { 1, 4, 45, 6, 10, -8 };
    int n = 16;
    int arr_size = 6;

    if (hasArrayTwoCandidates(A, arr_size, n))
        printf("Array has two elements with given sum");
    else
        printf("Array doesn't have two elements with given sum");

    getchar();
    return 0;
}
