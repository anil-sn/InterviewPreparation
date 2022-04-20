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
    Find K-th Permutation Sequence
    Problem Statement: Given N and K, where N is the sequence of numbers from 1 to N([1,2,3….. N])
                       find the Kth permutation sequence.

    For N = 3  the 3!  Permutation sequences in order would look like this:-
    K = 1, "123"
    K = 2, "132"
    K = 3, "213"
    K = 4, "231"
    K = 5, "312"
    K = 6, "321"

    Note: 1<=K<=N! Hence for a given input its Kth permutation always exists

    Examples:

    Example 1:
        Input: N = 3, K = 3
        Output: “213”
        Explanation: The sequence has 3! permutations as illustrated in the figure above. K = 3 corresponds to the third sequence.

    Example 2:
        Input: N = 3, K = 5
        Result: “312”
        Explanation: The sequence has 3! permutations as illustrated in the figure above. K = 5 corresponds to the fifth sequence.
*/

char permute[100][100];
int row;

#define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)

//function to generate all possible permutations of a string
void solve(char s[], int n, int index) {

    if (index == n) {
        sprintf(permute[row++], "%s\r\n", s);
        return;
    }

    for (int i = index; i < n; i++) {
        SWAP(s[i], s[index], char);
        solve(s, n, index + 1);
        SWAP(s[i], s[index], char);
    }
}

void getPermutation(int n, int k) {

    char s[n+1];
    int pos = 0;

    //create string
    for (int i = 1; i <= n; i++) {
        s[pos++] = (i + '0');
    }
    s[pos] = '\0';

    solve(s, pos, 0);

    for (int i = 0; i < row; i++) {
        printf("%s", permute[i]);
    }
    printf("\r\n");

    return;
}

int main() {
  int n = 3;
  int k = 3;

  getPermutation(n, k);

  return 0;
}