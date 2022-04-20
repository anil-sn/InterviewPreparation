#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// returns max of two integers
int max(int a, int b) { return (a > b) ? a : b; }

// A binary tree node has data, pointer to left child
// and a pointer to right child
typedef struct node {
    int data;
    struct node *left;
    struct node *right;
} NODE;

// Helper function that allocates a new node with the
// given data and NULL left and right pointers.
NODE* newNode(int data)
{
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return (node);
}

//  The function Compute the "height" of a tree. Height is
//  the number f nodes along the longest path from the root
//   node down to the farthest leaf node.
int height(NODE* node)
{
    // base case tree is empty
    if (node == NULL)
        return 0;

    // If tree is not empty then height = 1 + max of left
    // height and right heights
    return 1 + max(height(node->left), height(node->right));
}

// Function to get diameter of a binary tree
int diameter(NODE* tree)
{
    int result = 0;
    int maxHeight = 0;
    int maxDiameter = 0;

    // base case where tree is empty
    if (tree == NULL)
        return 0;

    // get the height of left and right sub-trees
    int lheight = height(tree->left);
    if (tree->left) {
        printf("\tHeight of tree %d = %d\r\n", tree->left->data, lheight);
    }

    int rheight = height(tree->right);
    if (tree->right) {
        printf("\tHeight of tree %d = %d\r\n", tree->right->data, rheight);
    }

    // get the diameter of left and right sub-trees
    int ldiameter = diameter(tree->left);
    if (tree->left) {
        printf("\tdiameter of tree %d = %d\r\n", tree->left->data, ldiameter);
    }

    int rdiameter = diameter(tree->right);
    if (tree->right) {
        printf("\tdiameter of tree %d = %d\r\n", tree->right->data, rdiameter);
    }

    // Return max of following three
    // 1) Diameter of left subtree
    // 2) Diameter of right subtree
    // 3) Height of left subtree + height of right subtree + 1
    maxDiameter = max(ldiameter, rdiameter);
    maxHeight = lheight + rheight + 1;
    result = max(maxHeight, maxDiameter);

    printf("diameter of tree %d = %d\r\n", tree->data, maxDiameter);
    printf("Height of tree %d = %d\r\n", tree->data, maxHeight);
    return result;
}

// Driver Code
int main()
{

    /* Constructed binary tree is
              1
            /   \
          2      3
        /  \
      4     5

    */

    NODE* root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);

    // Function Call
    printf ("\r\n"
            "              1         \r\n"
            "            /  \\       \r\n"
            "          2      3      \r\n"
            "        /  \\            \r\n"
            "      4     5           \r\n"
            "                        \r\n");
    printf("Diameter of the given binary tree is %d\n", diameter(root));

    return 0;
}