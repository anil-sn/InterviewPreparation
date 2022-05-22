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
    https://www.youtube.com/watch?v=Ox0TenN3Zpg&list=PLot-Xpze53ldg4pN6PfzoJY7KsKcxF1jg&index=9

    First note that dp[k] represents the number of BST trees built from 1....k;

    Then assume we have the number of the first 4 trees: dp[1] = 1 ,dp[2] =2 ,dp[3] = 5, dp[4] =14 ,
    how do we get dp[5] based on these four numbers is the core problem here.
    The essential process is: to build a tree, we need to pick a root node,
    then we need to know how many possible left sub trees and right sub trees can be held under that node, finally multiply them.
    To build a tree contains {1,2,3,4,5}. First we pick 1 as root, for the left sub tree, there are none;
    for the right sub tree, we need count how many possible trees are there constructed from {2,3,4,5},
    apparently it's the same number as {1,2,3,4}. So the total number of trees under "1" picked as root is dp[0] * dp[4] = 14.
    (assume dp[0] =1). Similarly, root 2 has dp[1]*dp[3] = 5 trees.
    root 3 has dp[2]*dp[2] = 4, root 4 has dp[3]*dp[1]= 5 and root 5 has dp[0]*dp[4] = 14.
    Finally sum the up and it's done.
    Now, we may have a better understanding of the dp[k], which essentially represents the number of BST trees with k consecutive nodes.
    It is used as database when we need to know how many left sub trees are possible for k nodes when picking (k+1) as root.
*/

int
numTrees (int n)
{
    int dp[n + 1];
    memset (dp, 0, sizeof (dp));
    /* base condition */
    dp[0] = 1;
    dp[1] = 1;

    for (int nodes = 2; nodes <= n; nodes++) {
        for (int root = 1; root <= nodes; root++) {
            int left = root - 1;
            int right = nodes - root;
            dp[nodes] += dp[left] * dp[right];
        }
    }

    return dp[n];
}