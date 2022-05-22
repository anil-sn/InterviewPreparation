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
 Two null trees are flip equivalent
 A non-null and null tree are NOT flip equivalent
 Two non-null trees with different root values are NOT flip equivalent
 Two non-null trees are flip equivalent if
      The left subtree of tree1 is flip equivalent with the left subtree of tree2 and the right subtree of tree1 is
      flipequivalent with the right subtree of tree2 (no flip case)
      OR
      The right subtree of tree1 is flip equivalent with the left subtree of tree2 and the left subtree of tree1 is
      flipequivalent with the right subtree of tree2 (flip case)
*/

bool
flipEquiv (TreeNode *root1, TreeNode *root2)
{
    if (NULL == root1 && NULL == root2) {
        return true;
    }

    if (NULL == root1 && NULL != root2 ||
            NULL != root1 && NULL == root2 ||
            root1->val != root2->val) {
        return false;
    }

    bool a = flipEquiv (root1->left, root2->left) && flipEquiv (root1->right, root2->right);
    bool b = flipEquiv (root1->right, root2->left) && flipEquiv ( root1->left, root2->right);
    return  (a || b);
}