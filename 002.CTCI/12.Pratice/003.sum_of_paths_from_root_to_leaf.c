// C program to find sum of all paths from root to leaves

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
    Dont practice until you get it right
    practice until you can't get it worng
*/

/*
Given a binary tree, where every node value is a Digit from 1-9 .Find the sum of all the numbers which are formed from root to leaf paths.
For example consider the following Binary Tree.


           6
       /      \
     3          5
   /   \          \
  2     5          4
      /   \
     7     4
  There are 4 leaves, hence 4 root to leaf paths:
   Path                    Number
  6->3->2                   632
  6->3->5->7               6357
  6->3->5->4               6354
  6->5>4                    654
Answer = 632 + 6357 + 6354 + 654 = 13997

The idea is to do a preorder traversal of the tree. In the preorder traversal, keep track of the value calculated till the current node, let this value be val. For every node, we update the val as val*10 plus node’s data.
*/

struct node {
    int data;
    struct node *left, *right;
};

// function to allocate new node with given data
struct node* newNode(int data) {
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->data = data;
    node->left = node->right = NULL;
    return (node);
}

// Returns sum of all root to leaf paths. The first parameter is root
// of current subtree, the second parameter is value of the number formed
// by nodes from root to this node
int treePathsSumUtil(struct node *root, int val) {
    // Base case
    if (root == NULL)  return 0;

    // Update val
    val = (val*10 + root->data);

    // if current node is leaf, return the current value of val
    if (root->left==NULL && root->right==NULL)
        return val;

    // recur sum of values for left and right subtree
    return treePathsSumUtil(root->left, val) +
           treePathsSumUtil(root->right, val);
}

// A wrapper function over treePathsSumUtil()
int treePathsSum(struct node *root) {
    // Pass the initial value as 0 as there is nothing above root
    return treePathsSumUtil(root, 0);
}

// Driver function to test the above functions
int main() {
    struct node *root = newNode(6);
    root->left        = newNode(3);
    root->right       = newNode(5);
    root->left->left  = newNode(2);
    root->left->right = newNode(5);
    root->right->right = newNode(4);
    root->left->right->left = newNode(7);
    root->left->right->right = newNode(4);
    printf("Sum of all paths is %d", treePathsSum(root));
    return 0;
}