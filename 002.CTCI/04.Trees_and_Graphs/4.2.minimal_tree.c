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
    4.2 Minimal Tree: Given a sorted (increasing order) array with unique integer elements, write an
                      algorithm to create a binary search tree with minimal height.

    SOLUTION
        To create a tree of minimal height, we need to match the number of nodes in the left subtree to the number
        of nodes in the right subtree as much as possible. This means that we want the root to be the middle of the
        array, since this would mean that half the elements would be less than the root and half would be greater
        than it.
        We proceed with constructing our tree in a similar fashion. The middle of each subsection of the array
        becomes the root of the node. The left half of the array will become our left subtree, and the right half of
        the array will become the right subtree.
        One way to implement this is to use a simple root. insertNode(int v) method which inserts the
        value v through a recursive process that starts with the root node. This will indeed construct a tree with
        minimal height but it will not do so very efficiently. Each insertion will require traversing the tree, giving a
        total cost ofO ( N log N) to the tree.
        Alternatively, we can cut out the extra traversals by recursively using the createMinimalBST method.
        This method is passed just a subsection of the array and returns the root of a minimal tree for that array.
        The algorithm is as follows:
        1. Insert into the tree the middle element of the array.
        2. Insert (into the left subtree) the left subarray elements.
        3. Insert (into the right subtree) the right subarray elements.
        4. Recurse.

    https://algorithms.tutorialhorizon.com/sorted-array-to-binary-search-tree-of-minimal-height/
    https://www.geeksforgeeks.org/sorted-array-to-balanced-bst/
*/

/* A Binary Tree node */
typedef struct TreeNode
{
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
}TREE_NODE;

/* Helper function that allocates a new node with the
   given data and NULL left and right pointers. */
TREE_NODE* newNode(int data)
{
    TREE_NODE* node = (TREE_NODE*) malloc(sizeof(TREE_NODE));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}


/* A function that constructs Balanced Binary Search Tree from a sorted array */
TREE_NODE* sortedArrayToBST(int arr[], int start, int end)
{
    /* Base Case */
    if (start > end) {
        return NULL;
    }

    /* Get the middle element and make it root */
    int mid = (start + end)/2;
    TREE_NODE *root = newNode(arr[mid]);

    /* Recursively construct the left subtree and make it left child of root */
    root->left =  sortedArrayToBST(arr, start, mid-1);

    /* Recursively construct the right subtree and make it right child of root */
    root->right = sortedArrayToBST(arr, mid+1, end);

    return root;
}


/* A utility function to print preorder traversal of BST */
void preOrder(TREE_NODE* node)
{
    if (node == NULL)
        return;
    printf("%d ", node->data);
    preOrder(node->left);
    preOrder(node->right);
}

/* Driver program to test above functions */
int main()
{
    int i;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int n = sizeof(arr)/sizeof(arr[0]);

    /* Convert List to BST */
    TREE_NODE *root = sortedArrayToBST(arr, 0, n-1);
    printf("Original Array :");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\r\n");
    printf("PreOrder Traversal of constructed BST :");
    preOrder(root);
    printf("\r\n");

    return 0;
}
