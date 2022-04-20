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
    Implement Pow(x,n) | X raised to the power N
    Problem Statement: Given a double x and integer n, calculate x raised to power n.
                       Basically Implement pow(x, n).

    Examples:

        Example 1:
        Input: x = 2.00000, n = 10
        Output: 1024.00000
        Explanation: You need to calculate 2.00000 raised to 10
                     which gives ans 1024.00000

    Example 2:

        Input: x = 2.10000, n = 3
        Output: 9.26100
        Explanation: You need to calculate 2.10000 raised to 3
                     which gives ans 9.26100
*/

double myPow(double x, int n) {
    double ans = 1.0;

    for (int i = 0; i < n; i++) {
        ans = ans * x;
    }

    return ans;
}

// Using Binary Exponentiation
double myPowBin(double x, int n) {

    double ans = 1.0;
    long long nn = n;

    printf("Size of int : %ld, Size of long long : %ld \r\n", sizeof(int), sizeof(long long));
    if (nn < 0)
        nn = -1 * nn;
    while (nn) {
        if (nn % 2) {
            ans = ans * x;
            nn = nn - 1;
        } else {
            x = x * x;
            nn = nn / 2;
        }
    }

    if (n < 0)
        ans = (double)(1.0) / (double)(ans);

    return ans;
}

int main()
{
    printf("Bruteforce: pow(2.1, 3) is %f \r\n", myPow(2.1, 3));
    printf("Binary Exponential : pow(2.1, 3) is %f \r\n", myPowBin(2.1, 3));
}