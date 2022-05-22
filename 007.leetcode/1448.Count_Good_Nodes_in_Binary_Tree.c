#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

//Definition for a binary tree node.
typedef struct TreeNode_ {
    int val;
    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;

int
goodNodes (TreeNode *root, int ma)
{
    if (root == NULL) {
        return 0;
    }

    int res = root->val >= ma ? 1 : 0;
    res += goodNodes (root->left, max (ma, root->val));
    res += goodNodes (root->right, max (ma, root->val));
    return res;
}

int
goodNodes (TreeNode *root)
{
    return goodNodes (root, -10000);
}