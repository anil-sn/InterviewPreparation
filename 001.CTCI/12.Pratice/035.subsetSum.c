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
    Method 1: Recursion.
        Approach: For the recursive approach we will consider two cases.

        Consider the last element and now the required sum = target sum – value of ‘last’ element and number of elements = total elements – 1
        Leave the ‘last’ element and now the required sum = target sum and number of elements = total elements – 1
        Following is the recursive formula for isSubsetSum() problem.

        isSubsetSum(set, n, sum)
        = isSubsetSum(set, n-1, sum) ||
        isSubsetSum(set, n-1, sum-set[n-1])
        Base Cases:
        isSubsetSum(set, n, sum) = false, if sum > 0 and n == 0
        isSubsetSum(set, n, sum) = true, if sum == 0
        Let’s take a look at the simulation of above approach-:

        set[]={3, 4, 5, 2}
        sum=9
        (x, y)= 'x' is the left number of elements,
        'y' is the required sum

                    (4, 9)
                    {True}
                /        \
                (3, 6)       (3, 9)

                /    \        /   \
            (2, 2)  (2, 6)   (2, 5)  (2, 9)
            {True}
            /   \
        (1, -3) (1, 2)
        {False}  {True}
                /    \
            (0, 0)  (0, 2)
            {True} {False}
*/

// Returns true if there is a subset
// of set[] with sum equal to given sum
bool isSubsetSum_recursion(int set[], int n, int sum)
{
    // Base Cases
    if (sum == 0)
        return true;

    if (n == 0)
        return false;

    // If last element is greater than sum,
    // then ignore it
    if (set[n - 1] > sum)
        return isSubsetSum_recursion(set, n - 1, sum);

    /* else, check if sum can be obtained by any of the following:
      (a) including the last element
      (b) excluding the last element   */
    return isSubsetSum_recursion(set, n - 1, sum)
           || isSubsetSum_recursion(set, n - 1, sum - set[n - 1]);
}
// Returns true if there is a subset of set[]
// with sum equal to given sum
bool isSubsetSum_dp(int set[], int n, int sum)
{
    // The value of subset[i][j] will be true if
    // there is a subset of set[0..j-1] with sum
    // equal to i
    bool subset[n + 1][sum + 1];
    for (int i = 0; i <= n; i++){
        for (int j = 0; j <= sum; j++)
            subset[i][j] = 0;
    }

    // If sum is 0, then answer is true
    for (int i = 0; i <= n; i++)
        subset[i][0] = true;

    // If sum is not 0 and set is empty,
    // then answer is false
    for (int i = 1; i <= sum; i++)
        subset[0][i] = false;

    // uncomment this code to print table
    printf("\r\n");
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= sum; j++)
            printf ("%4d", subset[i][j]);
        printf("\r\n");
    }

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
    printf("\r\n");
     for (int i = 0; i <= n; i++)
     {
       for (int j = 0; j <= sum; j++)
          printf ("%4d", subset[i][j]);
       printf("\r\n");
     }

    return subset[n][sum];
}

// Driver code
int main()
{
    int set[] = { 3, 34, 4, 12, 5, 2 };
    int sum = 9;
    int n = sizeof(set) / sizeof(set[0]);
    if (isSubsetSum_recursion(set, n, sum) == true)
        printf("Recursion Found a subset with given sum");
    else
        printf("Recursion No subset with given sum");

    if (isSubsetSum_dp(set, n, sum) == true)
        printf("Dynamic Programming Found a subset with given sum");
    else
        printf("Dynamic Programming No subset with given sum");
    return 0;
}