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
    There are N piles of coins each containing  Ai (1<=i<=N) coins.
    Find the minimum number of coins to be removed such that the absolute difference of coins in
    any two piles is at most K.

    Note: You can also remove a pile by removing all the coins of that pile.

    Example 1:
    Input:
        N = 4, K = 0
        arr[] = {2, 2, 2, 2}
    Output:
        0

    Explanation:
        For any two piles the difference in the
        number of coins is <=0. So no need to
        remove any coins.

    Example 2:
    Input:
        N = 6, K = 3
        arr[] = {1, 5, 1, 2, 5, 1}
    Output :
        2
    Explanation:
    If we remove one coin each from both
    the piles containing 5 coins , then
    for any two piles the absolute difference
    in the number of coins is <=3.

    Approach: Since we cannot increase the number of coins in a pile.
        So, the minimum number of coins in any pile will remain the same as they can’t be removed,
        and increasing them will add to the operations which we need to minimize.
        Now, find the minimum coins in a pile, and for every other pile if the difference between
        the coins in the current pile and the minimum coin pile is greater than K then remove
        the extra coins from the current pile.
*/

int arrayMin(int a[], int n){
    int min = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (min > a[i]) {
            min = a[i];
        }
    }

    return min;
}

int minimumCoins(int a[], int n, int k)
{
    // To store the coins needed to be removed
    int cnt = 0;

    // Minimum value from the array
    int minVal = arrayMin(a, n);

    // Iterate over the array
    // and remove extra coins
    for (int i = 0; i < n; i++)
    {
        int diff = a[i] - minVal;

        // If the difference between
        //  the current pile and the
        // minimum coin pile is greater than k
        if (diff > k) {

            // Count the extra coins to be removed
            cnt += (diff - k);
        }
    }

    // Return the required count
    return cnt;
}

/* Driver program to test pageFaults function*/
int main()
{
    int piles[] = { 1, 5, 1, 2, 5, 1};
    int n = 6, k = 3;

    printf("Minimum number of coins to be removed is %d\r\n",minimumCoins(piles, n, k));
    return 0;
}