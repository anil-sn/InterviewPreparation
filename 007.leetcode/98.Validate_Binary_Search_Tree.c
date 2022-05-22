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

TreeNode *
getTreeNode (int val)
{
    TreeNode *t = calloc (1, sizeof (TreeNode));
    t->val = val;
    return t;
}

bool
isValidBST (TreeNode *root, TreeNode *minNode, TreeNode *maxNode)
{
    if (NULL == root) {
        return true;
    }

    if ((minNode) && (root->val <= minNode->val)) {
        return false;
    }

    if ((maxNode) && (root->val >= maxNode->val)) {
        return false;
    }

    return (isValidBST (root->left, minNode, root) && isValidBST (root->right, root, maxNode));
}

bool
isValidBST (TreeNode *root)
{
    return isValidBST (root, NULL, NULL);
}