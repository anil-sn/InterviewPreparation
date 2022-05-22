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

/*
    The basic idea is here:
    Say we have 2 arrays, PRE and IN.
    Preorder traversing implies that PRE[0] is the root node.
    Then we can find this PRE[0] in IN, say it's IN[5].
    Now we know that IN[5] is root, so we know that IN[0] - IN[4] is on the left side, IN[6] to the end is on the right side.
    Recursively doing this on subarrays, we can build a tree out of it :)
*/

/*
        3
      /   \
    9     20
        /    \
       15     7

    Input: preorder = [3,9,20,15,7], inorder = [9,3,15,20, 7]
    Output: [3,9,20,null,null,15,7]
*/

TreeNode *
helper (int preStart, int inStart, int inEnd, int preorder[], int inorder[], int len)
{
    if (preStart > len - 1 || inStart > inEnd) {
        return NULL;
    }

    TreeNode *root = getTreeNode (preorder[preStart]);
    // Index of current root in inorder
    int inIndex = 0;

    for (int i = inStart; i <= inEnd; i++) {
        if (inorder[i] == root->val) {
            inIndex = i;
        }
    }

    root->left = helper (preStart + 1, inStart, inIndex - 1, preorder, inorder, len);
    root->right = helper (preStart + inIndex - inStart + 1, inIndex + 1, inEnd, preorder, inorder, len);
    return root;
}

TreeNode *
buildTree (int preorder[], int inorder[],  int len)
{
    return helper (0, 0, len - 1, preorder, inorder, len);
}
