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
    int val;
    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;

// For every node, length of longest path which pass it = MaxDepth of its left subtree + MaxDepth of its right subtree.
// https://www.youtube.com/watch?v=bkxqA8Rfv04&list=PLot-Xpze53ldg4pN6PfzoJY7KsKcxF1jg&index=12&ab_channel=NeetCode
int
maxDepth (TreeNode *root)
{
    if (root == NULL) {
        return 0;
    }

    int left = maxDepth (root->left);
    int right = maxDepth (root->right);
    max = MAX (max, left + right);
    return MAX (left, right) + 1;
}

int max = 0;
int
diameterOfBinaryTree (TreeNode *root)
{
    maxDepth (root);
    return max;
}