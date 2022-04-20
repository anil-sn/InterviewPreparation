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
    Allocate Minimum Number of Pages

    Problem Statement: Given an array of integers A of size N and an integer B.
    The College library has N bags, the ith book has A[i] number of pages.

    You have to allocate books to B number of students
    so that the maximum number of pages allocated to a student is minimum.

    Conditions given :
    A book will be allocated to exactly one student.
    Each student has to be allocated at least one book.
    Allotment should be in contiguous order,
    for example, A student cannot be allocated book 1 and book 3, skipping book 2.
    Calculate and return the minimum possible number.
    Return -1 if a valid assignment is not possible.

    Examples:

        Example 1:
            Input: A = [12, 34, 67, 90]
                B = 2

            Output: 113

                Student 1    |   Student 2
                        12   |  34, 67, 90 (191)
                    12  34   |  67, 90 (157)
        (113)   12  34  67   |  90

    So, the maximum number of pages allocated in each case is [191,157,113].
    So, the minimum number among them is 113. Hence, our result is 113.
*/

#define MAX(a_, b_) (a_ > b_ ? a_ : b_)
#define MIN(a_, b_) (a_ < b_ ? a_ : b_)

int isPossible(int A[], int len, int pages, int students)
{
    int cnt = 0;
    int sumAllocated = 0;

    for (int i = 0; i < len; i++) {
        if (sumAllocated + A[i] > pages) {

            cnt++;
            sumAllocated = A[i];

            if (sumAllocated > pages) {
                return false;
            }

        } else {
            sumAllocated += A[i];
        }
    }

    if (cnt < students) {
        return true;
    }

    return false;
}

int books(int A[], int len, int B)
{
    if (B > len) {
        return -1;
    }

    int low = A[0];
    int high = 0;

    //to find minimum value and sum of all pages
    for (int i = 0; i < len; i++) {
        high = high + A[i];
        low = MIN(low, A[i]);
    }

    //binary search
    while (low <= high) {

        int mid = (low + high) >> 1;
        if (isPossible(A, len, mid, B)) {

            high = mid - 1;
        } else {

            low = mid + 1;
        }
    }

    return low;
}

int main() {
    int A[] = {12,34,67,90};
    int len = sizeof(A)/sizeof(A[0]);
    int B = 2;

    printf("Minimum Possible Number is %d \r\n", books(A, len, B));
    return 0;
}