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
    Tricky Subset Problem

    N children are made to stand in a queue. Each of them is given a number Ai.
    The teacher writes a number S on a page and passes it to the first child.
    Each child must add all the numbers they see on the page along with their own number,
    write the sum on the paper and pass it to the next person.
    In the end, the teacher must determine if X can be formed by adding some of the
    numbers from the series of numbers written on the paper.

    Example 1:
        Input:
            S = 1, N = 4, X = 7
            A = {1, 2, 4, 2}
        Output: yes
            Explaination: The final sequence of
            numbers on the paper is 1, 2, 5, 12, 22.
            Using 2 and 5 we can form 7.

    Example 2:
        Input:
            S = 100, N = 2, X = 500
            A = {51, 88}
        Output: no
            Explaination: The final sequence of
            numbers on the paper is 100, 151, 339.
            Using these numbers we cannot form 500.

    Your Task:
    Your task is to complete the function isPossible() which takes S, N, X and array A as
    input parameters and returns 1 if there is a subset which adds to this sum.
    Otherwise returns 0.
*/
bool isSubsetSum(long long set[], long long n, long long sum)
{
    // The value of subset[i][j] will be true if
    // there is a subset of set[0..j-1] with sum
    // equal to i
    bool subset[n + 1][sum + 1];

    // If sum is 0, then answer is true
    for (int i = 0; i <= n; i++)
        subset[i][0] = true;

    // If sum is not 0 and set is empty,
    // then answer is false
    for (int i = 1; i <= sum; i++)
        subset[0][i] = false;

    // Fill the subset table in bottom up manner
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= sum; j++) {
            if (j < set[i - 1])
                subset[i][j] = subset[i - 1][j];
            if (j >= set[i - 1])
                subset[i][j] = subset[i - 1][j]
                               || subset[i - 1][j - set[i - 1]];
        }
    }

    // uncomment this code to print table
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= sum; j++)
            printf ("%4d", subset[i][j]);
        printf("\n");
    }

    return subset[n][sum];
}

int isPossible(long long S, long long N, long long X, long long A[])
{
    long long paper[N+1];
    int paperPos = 0;
    int sum = 0;
    paper[paperPos++] = S;

    for (int i = 0; i < N; i++){
        sum = 0;
        for (int j = paperPos - 1;j >=0;j--){
            sum += paper[j];
        }
        paper[paperPos++] = sum + A[i];
    }

    printf("\r\n\r\n");
    for (int i = 0; i <= N; i++){
        printf("%4lld ", paper[i]);
    }
    printf("\r\n\r\n");

    return (isSubsetSum(paper, N+1, X));
}

int main ()
{
    long long S = 1;
    long long N = 4;
    long long X = 7;
    long long A[] = {1, 2, 4, 2};

    bool result = isPossible(S, N, X, A);
    printf("\r\n\r\n");
    printf ("Can Teacher make sum %lld : %s\r\n", X, result ? "TRUE" : "FALSE");
    printf("\r\n\r\n");

    return 0;
}