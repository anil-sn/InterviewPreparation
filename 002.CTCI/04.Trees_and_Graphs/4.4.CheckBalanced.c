#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "limits.h"
#include "math.h"
#include "sys/types.h"
#include "assert.h"

/*
    4.4 Check Balanced: Implement a function to check if a binary tree is balanced. For the purposes of
                        this question, a balanced tree is defined to be a tree such that the heights of
                        the two subtrees of any node never differ by more than one.

    SOLUTION
        In this question, we've been fortunate enough to be told exactly what balanced means: that for each node,
        the two subtrees differ in height by no more than one. We can implement a solution based on this definition.
        We can simply recurse through the entire tree, and for each node, compute the heights of each subtree.

        checking the height of each subtree as we recurse down from the root.
        On each node, we recursively get the heights of the left and right subtrees through the checkHeight
        method. If the subtree is balanced, then checkHeight will return the actual height of the subtree. If the
        subtree is not balanced, then checkHeight will return an error code. We will immediately break and
        return an error code from the current call.
*/

/* A binary tree node has data, pointer to left child
   and a pointer to right child */
typedef struct node {
    int data;
    struct node* left;
    struct node* right;
}NODE;

/* Helper function that allocates a new node with the given data and NULL left and right pointers. */
NODE*
newNode(int data) {
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return (node);
}

/* UTILITY FUNCTIONS TO TEST isBalanced() FUNCTION */

/* returns maximum of two integers */
int
max(int a, int b) {
    return (a >= b) ? a : b;
}

/*  The function Compute the "height" of a tree.
    Height is the number of nodes along the longest path from the root node down to the farthest leaf node.*/
int
height(NODE* node) {

    /* base case tree is empty */
    if (node == NULL) {
        return 0;
    }

    /* If tree is not empty then height = 1 + max of left height and right heights */
    return 1 + max(height(node->left), height(node->right));
}

/* Returns true if binary tree with root as root is height-balanced */
bool
isBalancedUnOptmized(NODE* root) {

    int lh; /* for height of left subtree */
    int rh; /* for height of right subtree */

    /* If tree is empty then return true */
    if (root == NULL) {
        return 1;
    }

    /* Get the height of left and right sub trees */
    lh = height(root->left);
    rh = height(root->right);

    if (abs(lh - rh) <= 1 && isBalancedUnOptmized(root->left) && isBalancedUnOptmized(root->right)) {
        return 1;
    }

    /* If we reach here then tree is not height-balanced */
    return 0;
}

bool
isBalanced(NODE* root, int* height)
{
    int lh = 0; /* for height of left subtree */
    int rh = 0; /* for height of right subtree */
    int l = 0; /* l will be true if left subtree is balanced */
    int r = 0; /* r will be true if right subtree is balanced */

    if (root == NULL) {
        *height = 0;
        return 1;
    }

    /* Get the heights of left and right subtrees in lh and rh
    And store the returned values in l and r */
    l = isBalanced(root->left, &lh);
    r = isBalanced(root->right, &rh);

    /* Height of current node is max of heights of left and right subtrees plus 1*/
    *height = max(lh, rh) + 1;

    /* If difference between heights of left and right subtrees is more than 2 then this node is not balanced
        so return 0 */
    if (abs(lh - rh) >= 2) {
        return 0;
    } else {
        return l && r; /* If this node is balanced and left and right subtrees are balanced then return true */
    }
}

int
main() {
    int height = 0;

    /* Constructed binary tree is
             1
           /   \
         2      3
       /  \    /
     4     5  6
    /
   7
  */
    NODE* root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);
    root->right->left = newNode(6);
    root->left->left->left = newNode(7);

    if (isBalancedUnOptmized(root))
        printf("[UnOptimized] Tree is balanced \r\n");
    else
        printf("[UnOptimized] Tree is not balanced \r\n");

    if (isBalanced(root, &height))
        printf("Tree is balanced \r\n");
    else
        printf("Tree is not balanced \r\n");

    return 0;
}

