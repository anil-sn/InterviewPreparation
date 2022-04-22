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
    Find minimum number of coins that make a given value

        Given a value V, if we want to make a change for V cents,
        and we have an infinite supply of each of C = { C1, C2, .., Cm} valued coins,
        what is the minimum number of coins to make the change?
        If it’s not possible to make a change, print -1.

    Examples:

        Input: coins[] = {25, 10, 5}, V = 30
        Output: Minimum 2 coins required
        We can use one coin of 25 cents and one of 5 cents

        Input: coins[] = {9, 6, 5, 1}, V = 11
        Output: Minimum 2 coins required
        We can use one coin of 6 cents and 1 coin of 5 cents
*/

// m is size of coins array (number of different coins)
int minCoins(int coins[], int numberOfCoinsGiven, int targetValue)
{
   // base case
   if (targetValue == 0) {
       return 0;
   }

   // Initialize result
   int res = INT_MAX;

   // Try every coin that has smaller value than V
   for (int i=0; i<numberOfCoinsGiven; i++) {
     if (coins[i] <= targetValue) {

         int sub_res = minCoins(coins, numberOfCoinsGiven, targetValue-coins[i]);

         /* we can use this coin */
         if (sub_res == INT_MAX) {
            continue;
         }

         // see if result can minimized
         // i.e, if we use this coin it is better than previous result
         if (sub_res + 1 < res)
            res = sub_res + 1;
     }
   }
   return res;
}

// Driver program to test above function
int main()
{
    int coins[] =  {9, 3, 3, 2, 1};
    int m = sizeof(coins)/sizeof(coins[0]);
    int V = 11;
    printf("Minimum coins required is %d \r\n", minCoins(coins, m, V));
    return 0;
}