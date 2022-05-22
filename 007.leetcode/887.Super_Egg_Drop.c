#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

int
find (int k, int n)
{
    if ( n == 0 || n == 1 )  {
        return n;   //if no. of floor 0 , 1 return n:
    }

    if ( k == 1 ) {
        return n;   // if 1 egg return number of floor
    }

    int ans = INT_MAX;

    for (int i = 1; i <= n; i++) { // try from 1 to n floor , drop every floor and find minimum
        int temp = 1 + max (find (k - 1, i - 1), find (k, n - i)) ; //maximum for worst case
        ans = min (ans, temp);                                   //minimum attempts from maximum temp
    }

    /*
    Here we have k eggs and n floor
    if we drop from i  (i=1 to n):
        i) egg break , now we remain k-1 eggs and i-1 floor beacase after i(included) floor all the eggs will also break
       ii) egg not break , now we remain k eggs and n-i floor because before i (included) all eggs will be remain
    */
    return ans;
}

int
superEggDrop (int K, int N)
{
    return find (K, N);
}

int
findMem (int k, int n, int row, int col, int memo[row][col])
{
    if ( n == 0 || n == 1 )  {
        return n;   //if no. of floor 0 , 1 return n:
    }

    if ( k == 1 ) {
        return n;   // if 1 egg return number of floor
    }

    if (-1 != memo[k][n]) {
        return memo[k][n];
    }

    int ans = INT_MAX;

    for (int i = 1; i <= n; i++) { // try from 1 to n floor , drop every floor and find minimum
        int temp = 1 + max (findMem (k - 1, i - 1, row, col, memo), findMem (k, n - i, row, col, memo)) ; //maximum for worst case
        ans = min (ans, temp);                                   //minimum attempts from maximum temp
    }

    /*
    Here we have k eggs and n floor
    if we drop from i  (i=1 to n):
        i) egg break , now we remain k-1 eggs and i-1 floor beacase after i(included) floor all the eggs will also break
       ii) egg not break , now we remain k eggs and n-i floor because before i (included) all eggs will be remain
    */
    memo[k][n] = ans;
    return memo[k][n];
}

int
superEggDropMem (int K, int N)
{
    int memo[K + 1][N + 1];
    memset (memo, -1, sizeof (memo));
    return findMem (K, N, K + 1, N + 1, memo);
}

int
findBinary (int k, int n, int row, int col, int memo[row][col])
{
    if ( n == 0 || n == 1 )  {
        return n;   //if no. of floor 0 , 1 return n:
    }

    if ( k == 1 ) {
        return n;   // if 1 egg return number of floor
    }

    if (-1 != memo[k][n]) {
        return memo[k][n];
    }

    int ans = INT_MAX;
    int l = 1;
    int h = n;
    int temp = 0;

    while (l <= h) {
        int mid = (l + h) / 2;
        int left = findBinary (k - 1, mid - 1, row, col, memo); //egg broken check for down floors of mid
        int right = findBinary (k, n - mid, row, col, memo);   // not broken check for up floors of mid
        temp = 1 + max (left, right);                          //store max of both

        if (left < right) {                                    //since right is more than left and we need more in worst case
            l = mid + 1;                                       // so l=mid+1 to gain more temp for worst case : upward
        } else {                                               //left > right so we will go downward
            h = mid - 1;
        }

        ans = min (ans, temp);                                  //store minimum attempts
    }

    /*
    Here we have k eggs and n floor
    if we drop from i  (i=1 to n):
        i) egg break , now we remain k-1 eggs and i-1 floor beacase after i(included) floor all the eggs will also break
       ii) egg not break , now we remain k eggs and n-i floor because before i (included) all eggs will be remain
    */
    memo[k][n] = ans;
    return memo[k][n];
}

int
superEggDropBinary (int K, int N)
{
    int memo[K + 1][N + 1];
    memset (memo, -1, sizeof (memo));
    return findBinary (K, N, K + 1, N + 1, memo);
}

int
superEggDropTab (int K, int N)
{
    if ( N == 0 || N == 1 )  {
        return N;   //if no. of floor 0 , 1 return n:
    }

    if ( K == 1 ) {
        return N;   // if 1 egg return number of floor
    }

    int dp[K + 1][N + 1];
    memset (dp, 0, sizeof (dp));

    for (int i = 0; i <= K; i++) {
        dp[i][0] = 0;
        dp[i][1] = 1;   //Filling from base case as if N==0 ans=0 , N==1 ans=1
    }

    for (int i = 0; i <= N; i++) {
        dp[0][i] = 0;
        dp[1][i] = i;   //Filling from base case as if K==0 ans=0 , K==1 ans=N, number of current floor (i)
    }

    for (int i = 2; i < K + 1; i++) {
        for (int j = 2; j < N + 1; j++) {
            int l = 1;
            int h = j;
            int temp = 0;
            int ans = INT_MAX;

            while (l <= h) {
                int mid = (l + h) / 2;
                int left = dp[i - 1][mid - 1]; //egg broken check for down floors of mid
                int right = dp[i][j - mid]; // not broken check for up floors of mid
                temp = 1 + max (left, right);        //store max of both

                if (left < right) {                //since right is more than left and we need more in worst case
                    l = mid + 1;                     // so l=mid+1 to gain more temp for worst case : upward
                } else {                         //left > right so we will go downward
                    h = mid - 1;
                }

                ans = min (ans, temp);             //store minimum attempts
            }

            dp[i][j] = ans;
        }
    }

    return dp[K][N];
}

int
main (void)
{
    //K -> egg , N -> floor
    int K = 0;
    int N = 0;
    K = 1;
    N = 2;
    printf ("[recursive] we need at minimum %d moves to determine with certainty \r\n", superEggDrop (K, N));
    printf ("[memoization] we need at minimum %d moves to determine with certainty \r\n", superEggDropMem (K, N));
    printf ("[memoization + binarySearch] we need at minimum %d moves to determine with certainty \r\n", superEggDropBinary (K, N));
    printf ("[tabulation + binarySearch] we need at minimum %d moves to determine with certainty \r\n", superEggDropTab (K, N));
    return 0;
}