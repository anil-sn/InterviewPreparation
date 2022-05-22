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
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *
lowestCommonAncestor (TreeNode *root, TreeNode *p, TreeNode *q)
{
    if ((root->val > p->val) && (root->val > q->val)) {
        return lowestCommonAncestor (root->left, p, q);
    }

    if ((root->val < p->val) && (root->val < q->val)) {
        return lowestCommonAncestor (root->right, p, q);
    }

    return root;
}

TreeNode *
lowestCommonAncestor (TreeNode *root, TreeNode *p, TreeNode *q)
{
    TreeNode *cur = root;

    while (true) {
        if (p->val < cur->val && q->val < cur->val)  {
            cur = cur->left;
        } else if (p->val > cur->val && q->val > cur->val)  {
            cur = cur->right;
        } else  {
            break;
        }
    }

    return cur;
}