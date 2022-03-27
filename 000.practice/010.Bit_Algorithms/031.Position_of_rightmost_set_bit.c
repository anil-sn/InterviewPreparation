#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"
#include "time.h"

/* Major data types typedef. */
typedef unsigned char           boolean;

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned int            uint32;
typedef unsigned long long      uint64;

typedef char                    int8;
typedef short                   int16;
typedef int                     int32;
typedef long long               int64;

typedef float                   float_t;
typedef double                  double_t;

#define SWAP(a_m, b_m)  (((a_m) ^= (b_m)), ((b_m) ^= (a_m)), ((a_m) ^= (b_m)))
#define MIN(a_m,b_m)    (b_m ^ ((a_m ^ b_m) & -(a_m < b_m)))
#define MAX(a_m,b_m)    (a_m ^ ((a_m ^ b_m) & -(a_m < b_m)))
#define SETBIT(flag_m, mask_m, val_m) (val_m = (val_m & ~mask_m) | (-flag_m & mask_m))
#define MERGE(a_m,b_m,r_m,mask_m) (r_m = a_m ^ ((a_m ^ b_m) & mask_m))

// Print a digit in binary
void dumpBinary(int n, char reason[]) {
    int i;
    printf("0");
    for (i = 1 << 30; i > 0; i = i / 2) {
        if((n & i) != 0) {
            printf("1");
        } else {
            printf("0");
        }
    }

    printf("[ %d ]: %s ", n, reason);
    printf("\r\n");
}

/*
    Position of rightmost set bit

    Write a one line function to return position of first 1 from right to left,
    in binary representation of an Integer.

    Input :     18,   Binary Representation 010010
    Output:     2

    Input :     19,   Binary Representation 010011
    Output:     1
*/

#define INT_SIZE 32

int Right_most_setbit(int num) {
    if(num == 0) { // for num==0 there is zero set bit
        return 0;
    } else {
        int pos = 1;

        // counting the position of first set bit
        for (int i = 0; i < INT_SIZE; i++) {
            if (!(num & (1 << i))) {
                pos++;
            } else {
                break;
            }
        }

        return pos;
    }
}
int main() {
    int num = 280;
    dumpBinary(num, "Input");
    int pos = Right_most_setbit(num);
    printf("Print right most bit possition : %d \r\n", pos);
    return 0;
}