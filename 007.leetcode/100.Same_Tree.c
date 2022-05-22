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

bool
isSameTree (TreeNode *p, TreeNode *q)
{
    if (p == NULL && q == NULL) {
        return true;
    }

    if (p == NULL || q == NULL) {
        return false;
    }

    if (p->val == q->val) {
        return (isSameTree (p->left, q->left) && isSameTree (p->right, q->right));
    }

    return false;
}