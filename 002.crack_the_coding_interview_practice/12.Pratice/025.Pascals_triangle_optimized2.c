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
    Dont practice until you get it right
    practice until you can't get it worng
*/

/*
    Pascal’s triangle is a triangular array of the binomial coefficients.
    Write a function that takes an integer value n as input and prints first n lines of the Pascal’s triangle. Following are the first 6 rows of Pascal’s Triangle.

    This method is based on method 1. We know that ith entry in a line number line is Binomial Coefficient C(line, i) and all lines start with value 1. The idea is to calculate C(line, i) using C(line, i-1). It can be calculated in O(1) time using the following.

    C(line, i)   = line! / ( (line-i)! * i! )
    C(line, i-1) = line! / ( (line - i + 1)! * (i-1)! )
    We can derive following expression from above two expressions.
    C(line, i) = C(line, i-1) * (line - i + 1) / i

    So C(line, i) can be calculated from C(line, i-1) in O(1) time
*/


// C program for Pascal’s Triangle
// A O(n^2) time and O(1) extra space
// function for Pascal's Triangle
void printPascal(int n) {
    for (int line = 1; line <= n; line++) {
        int C = 1; // used to represent C(line, i)
        for (int i = 1; i <= line; i++) {
            printf("%d ", C); // The first value in a line is always 1
            C = C * (line - i) / i;
        }
        printf("\n");
    }
}

// Driver program
int main() {
    int n = 7;
    printPascal(n);
    return 0;
}