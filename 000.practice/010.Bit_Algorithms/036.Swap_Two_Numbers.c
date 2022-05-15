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

void swap1(int *x, int *y)
{
    *x = *x + *y;
    *y = *x - *y;
    *x = *x - *y;
}

void swap2(int *x, int *y)
{
    *x = *x * *y;
    *y = *x / *y;
    *x = *x / *y;
}

void swap3(int *x, int *y)
{
    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
}

void swap4(int *a, int *b)
{
    // same as a = a + b
    *a = (*a & *b) + (*a | *b);

    // same as b = a - b
    *b = *a + (~(*b)) + 1;

    // same as a = a - b
    *a = *a + (~(*b)) + 1;
}

int main(void)
{
    uint32 a = 100;
    uint32 b = 200;

    printf("Before : %d, %d \r\n", a, b);
    swap1(&a, &b);
    printf("After  : %d, %d \r\n\r\n", a, b);

    printf("Before : %d, %d \r\n", a, b);
    swap2(&a, &b);
    printf("After  : %d, %d \r\n\r\n", a, b);

    printf("Before : %d, %d \r\n", a, b);
    swap3(&a, &b);
    printf("After  : %d, %d \r\n\r\n", a, b);

    printf("Before : %d, %d \r\n", a, b);
    swap4(&a, &b);
    printf("After  : %d, %d \r\n\r\n", a, b);

    return;
}