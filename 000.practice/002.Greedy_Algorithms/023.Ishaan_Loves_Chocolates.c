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
    As we know, Ishaan has a love for chocolates. He has bought a huge chocolate bar that
    contains N chocolate squares. Each of the squares has a tastiness level which is denoted
    by an array A[].
    Ishaan can eat the first or the last square of the chocolate at once.
    Ishaan has a sister who loves chocolates too and she demands the last chocolate square.
    Now, Ishaan being greedy eats the more tasty square first.

    Determine the tastiness level of the square which his sister gets.

    Example 1:

    Input : arr[ ] = {5, 3, 1, 6, 9}
    Output : 1

    Explanation:
    Initially: 5 3 1 6 9
    In first step: 5 3 1 6
    In Second step: 5 3 1
    In Third step: 3 1
    In Fourth step: 1

    Return 1


    Example 2:

    Input : arr[ ] = {5, 9, 2, 6}
    Output :  2

*/

int chocolates(int arr[], int n)
{
   // Complete the function
   int small=arr[0];
   for(int i=1;i<n;i++)
   {
       if(small>arr[i]){
           small=arr[i];
       }
   }
   return small;

}

int main()
{
    int arr[] = { 2, 3, 6, 8, 9, 5, 1, 3, 6, 3 };
    int n = sizeof(arr) / sizeof(arr[0]);

    printf( "Ishaan's sister gets to eat %d \r\n", chocolates(arr,n));
    return 0;
}