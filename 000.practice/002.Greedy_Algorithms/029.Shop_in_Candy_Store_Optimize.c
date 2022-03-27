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
    In a candy store, there are N different types of candies available and the prices of all the N
    different types of candies are provided to you.You are now provided with an attractive offer.
    You can buy a single candy from the store and get at most K other candies
    ( all are different types ) for free.
    Now you have to answer two questions. Firstly, you have to find what is the minimum amount
    of money you have to spend to buy all the N different candies. Secondly, you have to find
    what is the maximum amount of money you have to spend to buy all the N different candies.
    In both the cases you must utilize the offer i.e. you buy one candy and get K other candies
    for free.

    Example 1:
        Input:
            N = 4
            K = 2
            candies[] = {3 2 1 4}
        Output:
        3 7

        Explanation:
            As according to the offer if you buy one candy you can take at most two
            more for free. So in the first case, you buy the candy which costs 1 and
            takes candies worth 3 and 4 for free, also you buy candy worth 2 as well.
            So min cost : 1+2 =3.
            In the second case, you can buy the candy which costs 4 and takes candies
            worth 1 and 2 for free, also you need to buy candy worth 3 as well.
            So max cost : 3+4 =7.

    Example 2:
        Input:
            N = 5
            K = 4
            candies[] = {3 2 1 4 5}

        Output:
            1 5

        Explanation:
            For minimimum cost buy the candy with the cost 1 and get all the other candies
            for free. For maximum cost buy the candy with the cost 5 and get all other candies
            for free.

    One important thing to note is, we must use the offer and get maximum candies back for every
    candy purchase. So if we want to minimize the money, we must buy candies at minimum cost and
    get candies of maximum costs for free. To maximize the money, we must do the reverse.
    Below is an algorithm based on this.

    First Sort the price array.

    For finding minimum amount :
    Start purchasing candies from starting
    and reduce k free candies from last with
    every single purchase.

    For finding maximum amount :
    Start purchasing candies from the end
    and reduce k free candies from starting
    in every single purchase.
*/

int cmp (const void *x, const void *y) {
    int *a = (int *)x;
    int *b = (int *)y;

    return *a > *b;
}

// function to find the maximum
// and the minimum cost required
void find(int arr[], int n, int k)
{
    // Sort the array
    qsort(arr, n, sizeof(arr[0]), cmp);

    int b = ceil(n / k * 1.0);
    int min_sum = 0, max_sum = 0;

    for(int i = 0; i < b; i++)
      min_sum += arr[i];

    for(int i = 2; i < n; i++)
      max_sum += arr[i];

    // print the minimum cost & maximum cost
    printf("Minimum cost is %d, Maximum cost is %d\r\n", min_sum, max_sum);
    return;
}

// Driver code
int main()
{
  int arr[] = {3, 2, 1, 4};
  int n = sizeof(arr) / sizeof(arr[0]);
  int k = 2;

  // Function call
  find(arr,n,k);
}