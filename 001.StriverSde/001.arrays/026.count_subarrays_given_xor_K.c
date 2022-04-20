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
    Count the number of subarrays with given xor K
    Problem Statement: Given an array of integers A and an integer B.
    Find the total number of subarrays having bitwise XOR of all elements equal to B.

    Examples:
    Input Format:  A = [4, 2, 2, 6, 4] , B = 6
    Result: 4
    Explanation: The subarrays having XOR of their elements as 6 are
    [4, 2], [4, 2, 2, 6, 4], [2, 2, 6], [6]

    Input Format: A = [5, 6, 7, 8, 9], B = 5
    Result: 2
    Explanation:The subarrays having XOR of their elements as 2 are
    [5] and [5, 6, 7, 8, 9]
*/

int solve(int A[], int len, int key) {

    int count = 0;
    for(int i = 0; i < len; i++) {

        int current_xor = 0;
        for(int j=i; j<len; j++) {

            current_xor = current_xor ^ A[j];
            if(current_xor == key) {

                count++;
            }
        }
    }
    return count;
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

int solveHash(int A[], int len, int key) {

    int c = 0;
    int cpx = 0;
    int sum = 0;
    OBJ in;

    for(int i = 0; i < len; i++) {

        cpx = cpx ^ A[i];
        sum = cpx ^ key;

        if(hashGetIndexForSum(sum) != -1) {
            c += hashGetIndexForSum(sum);
        }

        if(cpx == key) {
            c++;
        }

        if(hashGetIndexForSum(cpx) != -1) {
            in.sum = cpx;
            in.index = hashGetIndexForSum(cpx) + 1;
            hashPutIndexForSum(&in);
        } else {
            in.sum = cpx;
            in.index = 1;
            hashPutIndexForSum(&in);
        }
    }

    return c;
}

int main()
{
    int A[] = { 4,2,2,6,4 };
    int len = sizeof(A)/sizeof(int);
    int key = 6;

    int totalCount= solve(A, len, key);
    printf("The total number of subarrays having a given XOR k is %d\r\n", totalCount);
    totalCount= solveHash(A, len, key);
    printf("Hash Table The total number of subarrays having a given XOR k is %d\r\n", totalCount);
}