#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
int countMaxOnes(int arr[], int size)
{
    int count = 0;
    int current = 0;
    if (0 == size) {
        return count;
    }

    for(int i = 0; i < size; i++) {
        if (arr[i] == 1) {
            current++;
            count = MAX(current, count);
        } else {
            current = 0;
        }
    }


    return count;
}

int main() {
    int arr[] = {1, 1, 0, 1, 1, 1};
    int size = sizeof(arr)/sizeof(arr[0]);

    int k = countMaxOnes(arr, size);
    printf("The array after removing duplicate elements is : %d\r\n", k);
}