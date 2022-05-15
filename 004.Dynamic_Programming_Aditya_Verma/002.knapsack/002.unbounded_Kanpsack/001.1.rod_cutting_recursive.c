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
    Given a rod of length n inches and an array of prices that includes prices of all pieces of size
    smaller than n. Determine the maximum value obtainable by cutting up the rod and selling the pieces.
    For example, if the length of the rod is 8 and the values of different pieces are given as the following,
    then the maximum obtainable value is 22 (by cutting in two pieces of lengths 2 and 6)

    length   | 1   2   3   4   5   6   7   8
    --------------------------------------------
    price    | 1   5   8   9  10  17  17  20

    And if the prices are as following, then the maximum obtainable value is 24
    (by cutting in eight pieces of length 1)

    length   | 1   2   3   4   5   6   7   8
    --------------------------------------------
    price    | 3   5   8   9  10  17  17  20
*/

// A utility function to get the maximum of two integers
int max(int a, int b) { return (a > b)? a : b;}

/* Returns the best obtainable price for a rod of length n and
   price[] as prices of different pieces */
int cutRod_loop(int price[], int n)
{
    if(n <= 0){
        return 0;
    }

    int q = INT_MIN;
    for(int i = 1; i<=n; i++){
        q = max(q,price[i-1]+cutRod_loop(price,n-i));
    }

    return q;
}

// Recursive solution w/o for loop
//p is the price array, n is the size of the array/rod, i is index initial called with 0
int cutRod_without_loop(int price[],int n, int i){

  if(n<=0 || i>=n){
    return 0;
  }

  return max(price[i]+cutRod_without_loop(price,n-(i+1),0), cutRod_without_loop(price,n,i+1));
}
/*
    Input:

        length[] = [1, 2, 3, 4,  5,  6,  7,  8]
        price[] =  [1, 5, 8, 9, 10, 17, 17, 20]

    Rod length: 4


    Best: Cut the rod into two pieces of length 2 each to gain revenue of 5 + 5 = 10

        Cut           Profit
        4                9
        1, 3            (1 + 8) = 9
        2, 2            (5 + 5) = 10
        3, 1            (8 + 1) = 9
        1, 1, 2         (1 + 1 + 5) = 7
        1, 2, 1         (1 + 5 + 1) = 7
        2, 1, 1         (5 + 1 + 1) = 7
        1, 1, 1, 1      (1 + 1 + 1 + 1) = 4
*/
/* Driver program to test above functions */
int main()
{
    int price[] = { 1, 5, 8, 9, 10, 17, 17, 20 };

    // rod length
    int n = 4;

    printf("Loop: Maximum Obtainable Value is %d\n", cutRod_loop(price, n));
    printf("With Out Loop: Maximum Obtainable Value is %d\n", cutRod_without_loop(price, n, 0));
    return 0;
}