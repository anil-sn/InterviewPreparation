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
    5.1 Insertion: You are given two 32-bit numbers, N and M, and two bit positions, i and j. Write a method
                    to insert Minto N such that M starts at bit j and ends at bit i. You can assume that the bits j through
                    i have enough space to fit all of M. That is, if M = 10011, you can assume that there are at least 5
                    bits between j and i. You would not, for example, have j = 3 and i = 2, because M could not fully
                    fit between bit 3 and bit 2.
    EXAMPLE :
    Input: N = 10000000000, M = 10011, i = 2, j = 6
    Output: N = 10001001100
*/
void print_binary(u_int32_t number)
{
    if (number >> 1) {
        print_binary(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}

void insert(u_int32_t N, u_int32_t M, u_int8_t i, u_int8_t j){

    printf("N : ");
    print_binary(N);
    printf("\r\n");

    printf("M : ");
    print_binary(M);
    printf("\r\n");

    printf("%d, %d \r\n", i, j);

    /* Clear the bits j through i in N */
    u_int32_t right_mask = (1 << i) - 1;
    print_binary(right_mask);
    printf("\r\n");

    u_int32_t left_mask = -1 << (j + 1);
    print_binary(left_mask);
    printf("\r\n");

    /* Shift M so that it lines up with bits J through i */
    u_int32_t M_shifted = M << i;
    print_binary(M_shifted);
    printf("\r\n");

    u_int32_t full_mask = right_mask | left_mask;
    print_binary(full_mask);
    printf("\r\n");

    u_int32_t N_cleared = N & full_mask;
    print_binary(N_cleared);
    printf("\r\n");

    printf("Expected Answer : ");
    print_binary(N_cleared | M_shifted);
    printf("\r\n");

    /* Merge M and N */
}

void
main () {

    u_int32_t N = 0b10000000000;
    u_int32_t M = 0b10011;

    insert(N, M, 2, 6);

    return;
}

