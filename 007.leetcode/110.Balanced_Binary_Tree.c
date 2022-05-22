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
    int value;
} TreeNode;

// Method 1:  O(N^2). This is a top down approach.
int
depth (TreeNode *root)
{
    if (root == NULL) {
        return 0;
    }

    return max (depth (root -> left), depth (root -> right)) + 1;
}

bool
isBalanced (TreeNode *root)
{
    if (root == NULL) {
        return true;
    }

    int left = depth (root->left);
    int right = depth (root->right);
    return abs (left - right) <= 1 && isBalanced (root->left) && isBalanced (root->right);
}

// Method 2:  O(N) based on DFS
int
dfsHeight (TreeNode *root)
{
    if (root == NULL) {
        return 0;
    }

    int leftHeight = dfsHeight (root -> left);

    if (leftHeight == -1) {
        return -1;
    }

    int rightHeight = dfsHeight (root -> right);

    if (rightHeight == -1) {
        return -1;
    }

    if (abs (leftHeight - rightHeight) > 1) {
        return -1;
    }

    return max (leftHeight, rightHeight) + 1;
}

bool
isBalanced (TreeNode *root)
{
    return dfsHeight (root) != -1;
}