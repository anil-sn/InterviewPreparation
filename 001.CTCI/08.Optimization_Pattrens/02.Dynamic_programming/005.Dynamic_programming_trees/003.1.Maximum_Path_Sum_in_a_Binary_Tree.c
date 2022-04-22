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
    Given a binary tree, find the maximum path sum. The path may start and end at any node in the tree.
    Example:

    Input: Root of below tree
        1
        / \
        2   3
    Output: 6

    See below diagram for another example.
    1+2+3

    For each node there can be four ways that the max path goes through the node:
        1. Node only
        2. Max path through Left Child
            + Node
        3. Max path through Right Child
            + Node
        4. Max path through Left Child
            + Node
            + Max path through Right Child

    The idea is to keep trace of four paths and pick up the max one in the end.
    An important thing to note is, root of every subtree need to return maximum
    path sum such that at most one child of root is involved. This is needed for
    parent function call. In below code, this sum is stored in ‘max_single’ and
    returned by the recursive function.
*/

// A binary tree node
typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
}NODE;

// A utility function to allocate a new node
NODE* newNode(int data)
{
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return (newNode);
}

// returns max of two integers
int max(int a, int b) { return (a > b) ? a : b; }

// This function returns overall maximum path sum in 'res'
// And returns max path sum going through root.
int findMaxUtil(NODE* root, int *res)
{
    //Base Case
    if (root == NULL) {
        return 0;
    }

    // l and r store maximum path sum
    // going through left and right child of root respectively
    int l = findMaxUtil(root->left,res);
    int r = findMaxUtil(root->right,res);

    // Max path for parent call of root.
    // This path must include at-most one child of root
    int max_single = max(max(l, r) + root->data, root->data);

    // Max Top represents the sum when the Node under
    // consideration is the root of the maxsum path and no
    // ancestors of root are there in max sum path
    int max_top = max(max_single, l + r + root->data);

    *res = max(*res, max_top); // Store the Maximum Result.

    return max_single;
}

// Returns maximum path sum in tree with given root
int findMaxSum(NODE *root)
{
    // Initialize result
    int res = INT_MIN;

    // Compute and return result
    findMaxUtil(root, &res);
    return res;
}

// Driver program
int main(void)
{
    NODE *root = newNode(10);

    root->left        = newNode(2);
    root->right       = newNode(10);
    root->left->left  = newNode(20);
    root->left->right = newNode(1);
    root->right->right = newNode(-25);
    root->right->right->left   = newNode(3);
    root->right->right->right  = newNode(4);

    printf("Max path sum is %d\r\n",findMaxSum(root));
    return 0;
}