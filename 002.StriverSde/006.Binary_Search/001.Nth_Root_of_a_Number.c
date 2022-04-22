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
    Nth Root of a Number using Binary Search
    Problem Statement: Given two numbers N and M, find the Nth root of M.

    The nth root of a number M is defined as a number X when raised to the power N equals M.

    Example 1:
        Input: N=3 M=27
        Output: 3
        Explanation: The cube root of 27 is 3.
    Example 2:
        Input: N=2 M=16
        Output: 4
        Explanation: The square root of 16 is 4
    Example 3:
        Input: N=5 M=243
        Output: 3
        Explaination: The 5th root of 243 is 3

    Approach: In order to find the Nth root of any given integer we are gonna use Binary Search.

        Step 1: Take low and high. Low will be equal to 1 and high will be M.
                We will take the mid of low and high such that the searching space is
                reduced using low + high / 2.
        Step 2: Make a separate function to multiply mid N times.
        Step 3: Run the while loop till (high – low > eps).
                Take eps as 1e-6, since we have to find answers to 6 decimal places.
        Step 4:  If the mid is less than M, then we will reduce search space to low and mid.
                Else, if it is greater than M then search space will be reduced to mid and high.
        Step 5: After the loop breaks we will have our answer as low or high.

    We have to find the answer to 6 decimals.
    So, we will have a double 1e-6. We will run the while loop till (high – low > eps).
    When we will come out of the loop we will have the answer
    which will be equal to low as well as high.

     -----------------------------------------------------------------------
    |Scientific notation and metric affixes                                 |
    |-----------------------------------------------------------------------|
    |To enter           | Use this          |   Scientific  |   Use this    |
    |this number        | metric affix      |   Notation    |   E Notation  |
    |-------------------|-------------------|---------------|---------------|
    |0.1                | 1d (deci)         |   10^-1       |   1e-1        |
    |0.01               | 1c (centi)        |   10^-2       |   1e-2        |
    |0.001              | 1m (milli)        |   10^-3       |   1e-3        |
    |0.000001           | 1u (micro)        |   10^-6       |   1e-6        |
    |0.000000001        | 1n (nano)         |   10^-9       |   1e-9        |
    |0.000000000001     | 1p (pico)         |   10^-12      |   1e-12       |
    |1,000              | 1k (kilo)         |   10^3        |   1e3         |
    |1,000,000          | 1M (mega)         |   10^6        |   1e6         |
    |1,000,000,000      | 1G (giga)         |   10^9        |   1e9         |
    |1,000,000,000,000  | 1T (tera)         |   10^12       |   1e12        |
    |-----------------------------------------------------------------------|
    |Note: When you use metric affixes or e notation, make sure you do not  |
    |include spaces between the number and the metric affix or the e.       |
     -----------------------------------------------------------------------
*/

double multiply(double number, int n)
{
    double ans = 1.0;
    for(int i = 1;i<=n;i++) {
        ans = ans * number;
    }
    return ans;
}

void getNthRoot(int n, int m)
{
    double low = 1;
    double high = m;
    double eps = 1e-6;

    while((high - low) > eps) {
        double mid = (low + high) / 2.0;
        if(multiply(mid, n) < m) {
            low = mid;
        }
        else {
            high = mid;
        }
    }

    printf("Calculated : %dth root of %d is %f \r\n", n, m, low);
    printf("POW : %dth root of %d is %f \r\n", n, m, pow(m, (double)(1.0/(double)(n))));
    return;
}

int main()
{
    int n = 0;
    int m = 0;

    n = 3;
    m = 27;
    getNthRoot(n, m);

    n = 3;
    m = 26;
    getNthRoot(n, m);

    n = 4;
    m = 15;
    getNthRoot(n, m);

    return 0;
}