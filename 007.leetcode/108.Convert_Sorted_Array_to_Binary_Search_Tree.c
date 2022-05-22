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
helper (int num[], int low, int high)
{
    if (low > high) {
        return NULL;
    }

    int mid = (low) + (high - low) / 2;
    TreeNode *node = getTreeNode (num[mid]);
    node->left = helper (num, low, mid - 1);
    node->right = helper (num, mid + 1, high);
    return node;
}


TreeNode *
sortedArrayToBST (int num[], int n)
{
    if (n == 0) {
        return NULL;
    }

    TreeNode *head = helper (num, 0, n - 1);
    return head;
}