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
Given three strings A, B and C. Write a function that checks whether C is an interleaving of A and B. C is said to be interleaving A and B, if it contains all and only characters of A and B and order of all characters in individual strings is preserved.

Example:

    Input: strings: "XXXXZY", "XXY", "XXZ"
    Output: XXXXZY is interleaved of XXY and XXZ
            The string XXXXZY can be made by
            interleaving XXY and XXZ
            String:    XXXXZY
            String 1:    XX Y
            String 2:  XX  Z

    Input: strings: "XXY", "YX", "X"
    Output: XXY is not interleaved of YX and X
            XXY cannot be formed by interleaving YX and X.
            The strings that can be formed are YXX and XYX

    Solution: Dynamic Programming.
    Dynamic Programming solution starts solving the problem bottom-up.
    At each stage we are computing if a substring C is interleaving of substring of A and B.
    if i (i<=m, length of A) and j (j <=n, length of B) are variables that iterate over
    sting A and B the for all possible values of i and j we see if first i characters of A
    and first j characters of B interleave to form first i+j characters of C.

    Matrix seems to be the obvious choice for storing all such values (because of there are
    to parameters i and j) with on string on horizontal axis and one on vertical axis.

    The value in the cell (i,j) is true if first i characters of A and first j characters of B
    interleave to form first (i+j) characters of C. while filling the matrix, if we are at cell
    (i,j), we check the (i+j-1)th character in C.

    Cell (0,0) is true. it means that zero chat of A and zero char of B interleave to
    for string that is same as first zero chars of string C.

    First row means that string A is empty. it will just check if substring B is same
    as that of substring of C:
    if (B[i-1] != C[i-1])
        MAT[0][i] = FALSE
    else
        MAT[0][i] = MAT[0][i-1]

    Similarly first coloumn will be polulated as
    if (A[j-1] != C[j-1])
        MAT[j][0] = FALSE
    else
        MAT[j][0] = MAT[j-1][0]

    other cells are populated string from top-left, moving in row-wise order.
    At each cell, we compare the current characters of A and B with the curre char of C.
    if we are at cell (i,j) the current characters of A, B and C are at
    the i-1th, j-1th and (i+j-1)th character in A, B and C respectively.
    At each cell, there or four possibilities.

    1. Cur char of C is neither equal to cur char of A nor cur char of B
        Value of cell is false
    2. Cur char of C is equal to cur char of A but not cur char of B
        Value of cell is same as the cell above it
    3. Cur chat of C is equal to cur char of B but no cur char of A
        Value of cell is same of the cell on its left
    4. Cur chat of C is equal to cur char of both A & B (All three are same).
        Value of the cell is ture
            if either of the cell above it or left is true
            otherwise its false
*/
bool isInterleaved(char* A, char* B, char* C)
{
    int M = strlen(A);
    int N = strlen(B);

    /* C should have exactly M+N char */
    if (strlen(C) != M+N) {
        return false;
    }

    //2D Array Matrix all set to 0
    bool MAT[M+1][N+1];
    MAT[0][0] = true;

    // populating first coloumn
    for (int i = 1; i <= M; i++) {
        if (A[i-1] != C[i-1])
            MAT[i][0] = false;
        else
            MAT[i][0] = MAT[i-1][0];
    }

    // populating first row
    for (int j = 1; j <= N; j++) {
        if (B[j-1] != C[j-1])
            MAT[0][j] = false;
        else
            MAT[0][j] = MAT[0][j-1];
    }

    for (int i=1; i <= M; i++){
        for (int j=1; j<=N; j++) {
            //current char of c same as A but not B
            if(A[i-1] == C[i+j-1] && B[j-1] != C[i+j-1]) {
                MAT[i][j] = MAT[i-1][j];
            } else if (A[i-1] != C[i+j-1] && B[j-1] == C[i+j-1]) {
                // current char of c same as B but not A
                MAT[i][j] = MAT[i][j-1];
            } else if (A[i-1] == C[i+j-1] && B[j-1] == C[i+j-1]) {
                // current char of c same as B but not A
                MAT[i][j] = (MAT[i-1][j] || MAT[i][j-1]);
            } else {
                MAT[i][j] = false;
            }
        }
    }

    return MAT[M][N];
}
// A function to run test cases
void test(char* A, char* B, char* C)
{
    if (isInterleaved(A, B, C))
        printf("%s is interleaved of %s and %s \r\n", C, A, B);
    else
        printf("%s is not interleaved of %s and %s \r\n", C, A, B);
}

// Driver program to test above functions
int main()
{
    test("XXY", "XXZ", "XXZXXXY");
    test("XY", "WZ", "WZXY");
    test("XY", "X", "XXY");
    test("YX", "X", "XXY");
    test("XXY", "XXZ", "XXXXZY");
    test("xyz", "abcd", "zabyczd");
    return 0;
}