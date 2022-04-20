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

int dp[100][100];

/*
    Given a string, a partitioning of the string is a palindrome partitioning if
    every substring of the partition is a palindrome. For example, “aba|b|bbabb|a|b|aba”
    is a palindrome partitioning of “ababbbabbababa”. Determine the fewest cuts needed for
    a palindrome partitioning of a given string. For example, minimum of 3 cuts are needed
    for “ababbbabbababa”. The three cuts are “a|babbbab|b|ababa”. If a string is a palindrome,
    then minimum 0 cuts are needed. If a string of length n containing all different characters,
    then minimum n-1 cuts are needed.

    Examples :

        Input : str = “geek”
        Output : 2
            We need to make minimum 2 cuts, i.e., “g ee k”

        Input : str = “aaaa”
        Output : 0
            The string is already a palindrome.

        Input : str = “abcde”
        Output : 4

        Input : str = “abbac”
        Output : 1

    If the string is a palindrome, then we simply return 0.
    Else, like the Matrix Chain Multiplication problem,
    we try making cuts at all possible places, recursively calculate the cost
    for each cut and return the minimum value.
*/

bool
isPalindrome(char p[], int i, int j)
{
    while (i < j) {
        if (p[i] != p[j]) {
            return false;
        }
        i++;
        j--;
    }
    return true;
}

// Matrix Ai has dimension p[i-1] x p[i] for i = 1..n
int palindrome_partitioning(char p[], int i, int j)
{
    /* Zero Elememts, no matrix can be formed */
    if (i > j) {
        return 0;
    }

    /* One Elememts, no matrix can be formed */
    if (i == j) {
        return 0;
    }

    /* if string is already a palindrome then no more cuts needed */
    if (isPalindrome(p, i, j)) {
        return 0;
    }

    if (0 != dp[i][j]) {
        return dp[i][j];
    }

    int k;
    int result = INT_MAX;
    int count;
    int left;
    int right;
    int calculation;

    // place parenthesis at different places between first and last matrix,
    // recursively calculate count of multiplications for each parenthesis placement
    // and return the minimum count
    for (k = i; k < j; k++)
    {
        left = palindrome_partitioning(p, i, k);
        right = palindrome_partitioning(p, k + 1, j);
        calculation = 1;

        count =  left + right + calculation;

        if (count < result) {
            result = count;
            dp[i][j] = result;
        }
    }

    // Return minimum count
    return result;
}

// Driver code
int main()
{
    char arr[] = "ababbbabbababa";
    int n = strlen(arr);
    printf ("Input %s is %s palindrome\r\n", arr, isPalindrome(arr, 0, (n-1)) ? "a" : "not a");
    printf("Palindrome partitioning is %d \r\n", palindrome_partitioning(arr, 0, (n -1)));

    return 0;
}