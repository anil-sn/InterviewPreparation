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
    5.2 Binary to String: Given a real number between 0 and 1 (e.g., 0.72) that is passed in as a double,
                        print the binary representation. If the number cannot be represented accurately in binary with at
                        most 32 characters, print"ERROR."
    Examples:

        Input :  (0.625)10
        Output : (0.101)2

        Input : (0.72)10
        Output : ERROR

    Solution: First, let’s start off by asking ourselves what a non-integer number in binary looks like.
              By analogy to a decimal number, the binary number 0 .1012 would look like:

        0.1012 = 1 * 1/21 + 0 *1/22 + 1 * 1/23 .

    Method 1: Multiply the decimal part by 2

    To print the decimal part, we can multiply by 2 and check if 2*n is greater than or equal to 1. This is essentially “shifting” the fractional sum. That is:

    r = 210 * n;
      = 210 * 0.1012;
      = 1 * 1/20 + 0 *1/21 + 1 * 1/22;
      = 1.012;
    If r >= 1, then we know that n had a 1 right after the decimal point.
    By doing this continuously, we can check every digit.

*/

/* Function to convert Binary real number to String */
void binaryToString(double n, char *string)
{
    char pos = 0;
    // Check if the number is Between 0 to 1 or Not
    if (n >= 1 || n <= 0) {
        sprintf(string, "%s", "ERROR");
        return;
    }

    double frac = 0.5;
    string[pos++] = '.';

    // Setting a limit on length: 32 characters.
    while (n > 0)
    {
        //Setting a limit on length: 32 characters
        if (strlen(string) >= 32) {
            sprintf(string, "%s", "ERROR");
            return;
        }

        // Multiply n by 2 to check it 1 or 0
        double b = n * 2;
        if (b >= 1)
        {
            string[pos++] = '1';
            n = b - 1;
        }
        else
        {
            string[pos++] = '0';
            n = b;
        }
    }

    return;
}


void
main () {

    double fraction = 0.625;
    char string[100] = {'\0'};

    binaryToString(fraction, string);
    printf("%s\r\n", string);

    fraction = 0.72;
    binaryToString(fraction, string);
    printf("%s\r\n", string);

    return;
}

