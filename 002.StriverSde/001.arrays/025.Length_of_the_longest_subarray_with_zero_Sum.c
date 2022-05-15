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
    Length of the longest subarray with zero Sum
    Problem Statement: Given an array containing both positive and negative integers, we have to find the length of the longest subarray with the sum of all elements equal to zero.

    Example 1:

    Input Format: N = 6, array[] = {9, -3, 3, -1, 6, -5}

    Result: 5

    Explanation: The following subarrays sum to zero:
    {-3, 3} , {-1, 6, -5}, {-3, 3, -1, 6, -5}
    Since we require the length of the longest subarray, our answer is 5!
    Example 2:

    Input Format: N = 8, array[] = {6, -2, 2, -8, 1, 7, 4, -10}

    Result: 8

    Subarrays with sum 0 : {-2, 2}, {-8, 1, 7}, {-2, 2, -8, 1, 7}, {6, -2, 2, -8, 1, 7, 4, -10}
    Length of longest subarray = 8
    Example 3:

    Input Format: N = 3, array[] = {1, 0, -5}

    Result: 1

    Subarray : {0}
    Length of longest subarray = 1
    Example 4:

    Input Format: N = 5, array[] = {1, 3, -5, 6, -2}

    Result: 0

    Subarray: There is no subarray that sums to zero
*/

#define MAX(x, y) ((x) > (y) ? (x) : (y))

void solve(int a[], int len)
{
	int  max = 0;
	for(int i = 0; i < len; i++) {
		int sum = 0;
		for(int j = i; j < len; j++){
			sum += a[j];
			if(sum == 0){
				max = MAX(max, j-i+1);
			}
		}
	}

    printf ("Length of the longest subarray with zero Sum is %d \r\n", max);
	return;
}

typedef struct object_ {
    int sum;
    int index;
}OBJ;

OBJ* HashMap[4096];

void hashPutIndexForSum (OBJ *data) {
    OBJ *in = malloc (sizeof(OBJ));

    in->sum = data->sum;
    in->index = data->index;

    HashMap[data->sum] = in;
    return;
}

int hashGetIndexForSum (int index) {
    OBJ *in = HashMap[index];
    if (NULL == in) {
        return -1;
    } else {
        return in->index;
    }
    return -1;
}


void maxLen(int A[], int n)
{
    OBJ in = {0,0};
    int maxi = 0;
    int sum = 0;
    for(int i = 0;i<n;i++) {

        sum += A[i];

        if(sum == 0) {
            maxi = i + 1;
        }
        else {
            if(hashGetIndexForSum(sum) != -1) {
                maxi = MAX(maxi, i - hashGetIndexForSum(sum));
            } else {
                in.sum  = sum;
                in.index = i;
                hashPutIndexForSum(&in);
            }
        }
    }
    printf ("Length of the longest subarray with zero Sum is %d \r\n", maxi);
    return;
}

int main()
{
    int a[] = {9, -3, 3, -1, 6, -5};
    int len = sizeof(a)/sizeof(a[0]);

    solve(a, len);
    maxLen(a, len);
}