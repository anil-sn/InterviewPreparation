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
*/


// C program for Pascal’s Triangle
// A O(n^2) time and O(n^2) extra space
// method for Pascal's Triangle
void printPascal(int n) {
    // An auxiliary array to store
    // generated pascal triangle values
    int arr[n][n];

    // Iterate through every line and print integer(s) in it
    for (int line = 0; line < n; line++) {

        // Every line has number of integers
        // equal to line number
        for (int i = 0; i <= line; i++) {

            // First and last values in every row are 1
            if (line == i || i == 0) {
                arr[line][i] = 1;
            } else {
                // Other values are sum of values just
                // above and left of above
                arr[line][i] = arr[line-1][i-1] + arr[line-1][i];
            }
            printf("%d ", arr[line][i]);
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