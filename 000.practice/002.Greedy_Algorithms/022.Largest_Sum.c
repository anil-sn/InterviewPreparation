#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    Largest number with given sum

    Geek lost the password of his super locker.
    He remembers the number of digits N as well as the sum S of all the digits of his password.
    He know that his password is the largest number of N digits that can be made with given sum S. As he is busy doing his homework, help him retrieving his password.

    Example 1:
    Input:
        N = 5, S = 12
    Output:
        93000
    Explanation:
        Sum of elements is 12. Largest possible
        5 digit number is 93000 with sum 12.

    Example 2:
    Input:
        N = 3, S = 29
    Output:
        -1
    Explanation:
        There is no such three digit number whose sum is 29.

    A Simple Solution is to consider all m digit numbers and keep track of maximum number
    with digit sum as s. A close upper bound on time complexity of this solution is O(10m).
    There is a Greedy approach to solve the problem. The idea is to one by one fill all
    digits from leftmost to rightmost (or from most significant digit to least significant).
    We compare the remaining sum with 9 if the remaining sum is more than 9,
    we put 9 at the current position, else we put the remaining sum. Since we fill digits
    from left to right, we put the highest digits on the left side, hence get the largest number.
*/

void findLargest(int m, int s)
{
    // If sum of digits is 0, then a number is possible
    // only if number of digits is 1.
    if (s == 0)
    {
        (m == 1) ? printf("Largest number is %d\r\n",0): printf("Not possible\r\n");
        return ;
    }

    // Sum greater than the maximum possible sum.
    if (s > 9*m) {
        printf("Not possible\r\n");
        return ;
    }

    // Create an array to store digits of result
    int res[m];

    // Fill from most significant digit to least significant digit.
    for (int i=0; i<m; i++) {

        // Fill 9 first to make the number largest
        if (s >= 9) {
            res[i] = 9;
            s -= 9;
        } else { // If remaining sum becomes less than 9, then fill the remaining sum
            res[i] = s;
            s = 0;
        }
    }

    printf("Largest number is :");
    for (int i=0; i<m; i++) {
        printf("%d", res[i]);
    }
    printf("\r\n");
}

// Driver code
int main()
{
    int s = 9, m = 2;
    findLargest(m, s);
    return 0;
}