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

TreeNode *
mergeTrees (TreeNode *t1, TreeNode *t2)
{
    if (t1 == NULL && t2 == NULL) {
        return NULL;
    }

    int val = (t1 == NULL ? 0 : t1->val) + (t2 == NULL ? 0 : t2->val);
    TreeNode *newNode = getTreeNode (val);
    newNode->left = mergeTrees (t1 == NULL ? NULL : t1->left, t2 == NULL ? NULL : t2->left);
    newNode->right = mergeTrees (t1 == NULL ? NULL : t1->right, t2 == NULL ? NULL : t2->right);
    return newNode;
}