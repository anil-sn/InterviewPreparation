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

int
sum (TreeNode *root, int s)
{
    if (root == NULL) {
        return 0;
    }

    if (root->right == NULL && root->left == NULL) {
        return s * 10 + root->val;
    }

    return sum (root->left, s * 10 + root->val) + sum (root->right, s * 10 + root->val);
}

int
sumNumbers (TreeNode *root)
{
    return sum (root, 0);
}
