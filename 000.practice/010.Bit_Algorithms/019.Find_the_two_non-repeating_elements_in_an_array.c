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
    Find the two non-repeating elements in an array of repeating elements/ Unique Numbers 2

    Given an array in which all numbers except two are repeated once. (i.e. we have 2n+2 numbers and n
    numbers are occurring twice and remaining two have occurred once). Find those two numbers in the
    most efficient way.

   Method (Use XOR)
    Let x and y be the non-repeating elements we are looking for and arr[] be the input array.
    First, calculate the XOR of all the array elements.

    xor = arr[0]^arr[1]^arr[2].....arr[n-1]
    All the bits that are set in xor will be set in one non-repeating element (x or y) and not in others.
    So if we take any set bit of xor and divide the elements of the array in two sets –
    one set of elements with same bit set and another set with same bit not set. By doing so,
    we will get x in one set and y in another set. Now if we do XOR of all the elements in the first set,
    we will get the first non-repeating element, and by doing same in other sets we will get the second
    non-repeating element.

    Let us see an example.
    arr[] = {2, 4, 7, 9, 2, 4}
    1) Get the XOR of all the elements. xor = 2^4^7^9^2^4 = 14 (1110)
    2) Get a number which has only one set bit of the xor.
       Since we can easily get the rightmost set bit, let us use it.
        set_bit_no = xor & ~(xor-1) = (1110) & ~(1101) = 0010
        Now set_bit_no will have only set as rightmost set bit of xor.
    3) Now divide the elements in two sets and do xor of
        elements in each set and we get the non-repeating
        elements 7 and 9. Please see the implementation for this step.

    Approach :
    Step 1: Xor all the elements of the array into a variable sum thus all the elements present twice
            in an array will get removed as for example, 4 = “100” and if 4 xor 4 => “100” xor “100”
            thus answer will be “000”.
    Step 2: Thus in the sum the final answer will be 3 xor 5 as both 2 and 4 are xor with itself giving 0,
            therefore sum = “011” xor “101” i.e sum = “110” = 6.
    Step 3: Now we will take 2’s Complement of sum i.e (-sum) = “010”.
    Step 4: Now bitwise And the 2’s of sum with the sum i.e “110” & “010” gives the answer “010”
            (Aim for bitwise & is that we want to get a number that contains only the rightmost
            set bit of the sum).
    Step 5: bitwise & all the elements of the array with this obtained sum,
            2 = “010” & “010” = 2, 3 = “011” & “010” = “010” , 4 = “100” & “010” = “000”,
            5 = “101” & “010” = “000”.
    Step 6: As we can see that the bitwise & of 2,3 > 0 thus they will be xor with sum1 and
            bitwise & of 4,5 is resulting into 0 thus they will be xor with sum2.
    Step 7: As 2 is present two times so getting xor with sum1 two times only the result 3 is being
            stored in it and As 4 is also present two times thus getting xor with sum2 will cancel it’s value
            and thus only 5 will remain there.
*/

/* This function sets the values of *x and *y to non-repeating elements in an array arr[] of size n*/
void get2NonRepeatingNos(int arr[], int n, int* x, int* y)
{
    /* Will hold Xor of all elements */
    int Xor = arr[0];

    /* Will have only single set bit of Xor */
    int set_bit_no;
    int i;
    *x = 0;
    *y = 0;

    /* Get the Xor of all elements */
    for (i = 1; i < n; i++)
        Xor ^= arr[i];

    /* Get the rightmost set bit in set_bit_no */
    set_bit_no = Xor & ~(Xor - 1);

    /* Now divide elements in two sets by comparing rightmost set bit
       of Xor with bit at same position in each element. */
    for (i = 0; i < n; i++) {
        if (arr[i] & set_bit_no) { /*Xor of first set */
            *x = *x ^ arr[i];
        }  else {  /*Xor of second set*/
            *y = *y ^ arr[i];
        }
    }
}

/* Driver program to test above function */
int main()
{
    int arr[] = { 2, 3, 7, 9, 11, 2, 3, 11 };
    int* x = (int*)malloc(sizeof(int));
    int* y = (int*)malloc(sizeof(int));
    get2NonRepeatingNos(arr, 8, x, y);
    printf("The non-repeating elements are %d and %d", *x, *y);
    getchar();
}