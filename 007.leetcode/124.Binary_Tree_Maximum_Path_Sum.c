#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

typedef struct TreeNode_ {
    struct TreeNode_ *left;
    struct TreeNode_ *right;
    int val;
} TreeNode;

/*
    A path from start to end, goes up on the tree for 0 or more steps, then goes down for 0 or more steps. Once it goes down, it can't go up. Each path has a highest node, which is also the lowest common ancestor of all other nodes on the path.
    A recursive method maxPathDown(TreeNode node) (1) computes the maximum path sum with highest node is the input node, update maximum if necessary (2) returns the maximum sum of the path that can be extended to input node's parent.
*/

int maxValue = INT_MIN;

int
maxPathDown (TreeNode *node)
{
    if (node == NULL) {
        return 0;
    }

    int left = MAX (0, maxPathDown (node->left));
    int right = MAX (0, maxPathDown (node->right));
    maxValue = MAX (maxValue, left + right + node->val);
    return MAX (left, right) + node->val;
}

int
maxPathSum (TreeNode *root)
{
    maxValue = INT_MIN;
    maxPathDown (root);
    return maxValue;
}
