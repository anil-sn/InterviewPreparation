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

#define MAX 1000
typedef struct stack_ {
    TreeNode *stack[MAX];
    int top;
} STACK;

// DFS in-order recursive: time complexity: O(N)
// better keep these two variables in a wrapper class
int number = 0;
int count = 0;

void
helper (TreeNode *n)
{
    if (n->left != NULL) {
        helper (n->left);
    }

    count--;

    if (count == 0) {
        number = n->val;
        return;
    }

    if (n->right != NULL) {
        helper (n->right);
    }
}

int
kthSmallest (TreeNode *root, int k)
{
    count = k;
    helper (root);
    return number;
}

// DFS in-order iterative: time complexity: O(N) best
int
kthSmallest (TreeNode *root, int k)
{
    STACK s;

    while (root != NULL) {
        push (&s, root);
        root = root->left;
    }

    while (k != 0) {
        TreeNode *n = pop (&s);
        k--;

        if (k == 0) {
            return n->val;
        }

        TreeNode *right = n->right;

        while (right != NULL) {
            push (&s, right);
            right = right->left;
        }
    }

    return -1; // never hit if k is valid
}

/* Optimized */
int
kthSmallest (TreeNode *root, int k)
{
    STACK s;
    TreeNode *p = root;
    int count = 0;

    while (false == isStackEmpty (&s) || p != NULL) {
        if (p != NULL) {
            push (&s, p);   // Just like recursion
            p = p->left;
        } else {
            TreeNode *node = pop (&s);

            if (++count == k) {
                return node->val;
            }

            p = node->right;
        }
    }

    return INT_MIN;
}