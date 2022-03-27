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
    Find smallest number with given number of digits and sum of digits

    How to find the smallest number with given digit sum s and number of digits d?
    Examples :
        Input  : s = 9, d = 2
        Output : 18
        There are many other possible numbers
        like 45, 54, 90, etc with sum of digits
        as 9 and number of digits as 2. The
        smallest of them is 18.

    Input  : s = 20, d = 3
    Output : 299
*/

// Prints the smallest possible number with digit sum 's'
// and 'm' number of digits.
void findSmallest(int m, int s)
{
    int sum = s;
    // If sum of digits is 0, then a number is possible
    // only if number of digits is 1.
    if (s == 0)  {
        if (m == 1) {
            printf ("Smallest number is 0 \r\n");
        } else {
            printf ("Smallest number not possible \r\n");
        }
        return ;
    }

    // Sum greater than the maximum possible sum.
    if (s > 9*m)
    {
        printf ("Smallest number not possible \r\n");
        return ;
    }

    // Create an array to store digits of result
    int res[m];

    // deduct sum by one to account for cases later
    // (There must be 1 left for the most significant digit)
    s -= 1;

    // Fill last m-1 digits (from right to left)
    for (int i=m-1; i>0; i--)
    {
        // If sum is still greater than 9,
        // digit must be 9.
        if (s > 9)
        {
            res[i] = 9;
            s -= 9;
        }
        else
        {
            res[i] = s;
            s = 0;
        }
    }

    // Whatever is left should be the most significant
    // digit.
    res[0] = s + 1;  // The initially subtracted 1 is incorporated here.

    printf ("Smallest number for Sum : %d, Digits: %d is : ", sum, m);
    for (int i=0; i<m; i++)
        printf ("%d" ,res[i]);

    printf ("\r\n");
}

// Driver code
int main()
{
    int s = 9, m = 2;
    findSmallest(m, s);
    return 0;
}