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
    Problem
        Given a binary tree in which each node element contains a number.
        Find the maximum possible sum from one leaf node to another.

    The maximum sum path may or may not go through root.

    A simple solution is to traverse the tree and
    do following for every traversed node X.

    1) Find maximum sum from leaf to root in left subtree of X
    2) Find maximum sum from leaf to root in right subtree of X.
    3) Add the above two calculated values and X->data and
       compare the sum with the maximum value obtained so far
       and update the maximum value.

    4) Return the maximum value.

    The time complexity of above solution is O(n2)
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
int maxPathSumUtil(NODE* root, int *res)
{
    // Base cases
    if (root == NULL) {
        return 0;
    }

    // Base case is return data when your are leaf node
    if ((NULL == root->left) && (NULL == root->right)) {
        return root->data;
    }

    // Find maximum sum in left and right subtree.
    // Find maximum root to leaf sums in left and right subtrees
    int ls = maxPathSumUtil(root->left, res);
    int rs = maxPathSumUtil(root->right, res);

    // If both left and right children exist
    if (root->left && root->right)
    {
        // Update result if needed
        *res = max(*res, ls + rs + root->data);

        // Return maximum possible value for root being on one side
        return max(ls, rs) + root->data;
    }

    // If any of the two children is empty, return
    // root sum for root being on one side
    return (NULL == root->left)? rs + root->data: ls + root->data;
}

// The main function which returns sum of the maximum
// sum path between two leaves. This function mainly
// uses maxPathSumUtil()
int maxPathSum(NODE *root)
{
    // Initialize result
    int res = INT_MIN;
    int val;

    // Compute and return result
    val = maxPathSumUtil(root, &res);

    //--- for test case ---
    //           7
    //         /   \
    //      Null   -3
    //  value of res will be INT_MIN but the answer is 4 ,
    //  which is returned by the function maxPathSumUtil().

    if(res == INT_MIN) {
        return val;
    } else {
        return res;
    }
}

// Driver program
int main(void)
{
    NODE *root = newNode(-15);
    root->left = newNode(5);
    root->right = newNode(6);
    root->left->left = newNode(-8);
    root->left->right = newNode(1);
    root->left->left->left = newNode(2);
    root->left->left->right = newNode(6);
    root->right->left = newNode(3);
    root->right->right = newNode(9);
    root->right->right->right= newNode(0);
    root->right->right->right->left= newNode(4);
    root->right->right->right->right= newNode(-1);
    root->right->right->right->right->left= newNode(10);

    printf("Max path sum is %d\r\n",maxPathSum(root));
    return 0;
}