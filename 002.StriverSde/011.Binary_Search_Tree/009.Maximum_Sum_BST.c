#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Given a binary tree root, return the maximum sum of all keys of any sub-tree which is also a Binary Search Tree (BST).

    Assume a BST is defined as follows:

    The left subtree of a node contains only nodes with keys less than the node's key.
    The right subtree of a node contains only nodes with keys greater than the node's key.
    Both the left and right subtrees must also be binary search trees.
*/

typedef struct node {
    int data;
    struct node * left;
    struct node * right;
}NODE;

NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

typedef struct values {
    int min;
    int max;
    int sum;
}VAL;

VAL * NodeValue(int min, int max, int sum)
{
    VAL * val = (VAL * ) malloc(sizeof(VAL));

    val->min = min;
    val->max = max;
    val->sum = sum;
    return (val);
}

#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)

VAL* largestBSTSubtreeHelper(NODE *root)
{
    if (root == NULL) {
        return (NodeValue(INT_MAX, INT_MIN, 0)); // {min, max, sum}, initialize min=MAX_VALUE, max=MIN_VALUE
    }

    VAL *left = largestBSTSubtreeHelper(root->left);
    VAL *right = largestBSTSubtreeHelper(root->right);

    // Current node is greater than max in left AND smaller than min in right, it is a BST.
    if ((left->max < root->data) && (root->data < right->min)) {

        // It is a BST.
        return NodeValue(MIN(root->data, left->min),
                         MAX(root->data, right->max),
                         left->sum + right->sum + 1);
    }

    // Otherwise, return [-inf, inf] so that parent can't be valid BST
    return NodeValue(INT_MIN, INT_MAX, MAX(left->sum, right->sum));
}

int maxSumBST(NODE *root) {
    VAL *ret = largestBSTSubtreeHelper(root);
    return ret->sum;
}

int main ()
{
    NODE *root = newNode(20);

    root->left = newNode(15);
    root->left->left = newNode(14);
    root->left->left->right = newNode(17);

    root->left->right = newNode(18);
    root->left->right->left = newNode(16);
    root->left->right->right = newNode(19);

    root->right = newNode(40);
    root->right->left = newNode(30);
    root->right->right = newNode(60);
    root->right->right->left = newNode(50);

    printf("Maximum sum BST is : %d \r\n", maxSumBST(root));

    return;
}