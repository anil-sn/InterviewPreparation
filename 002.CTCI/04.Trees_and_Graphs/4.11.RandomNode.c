#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "limits.h"
#include "math.h"
#include "sys/types.h"
#include "assert.h"
#include <time.h>


/*
    Chapter 04.11 Random Node
    Problem Statement:
        You are implementing a binary search tree class from scratch, which, in addition to insert, find, and delete,
        has a method getRandomNode() which returns a random node from the tree. All nodes should be equally likely to
        be chosen. Design and implement an algorithm for getRandomNode(), and explain how you would implement the rest
        of the methods.
    Solution:
        The obvious - but not necessarily optimal - solution is to create and maintain an array of pointers to each node in the
        tree. When a random node is desired, a random number between zero and the maximum index of the array is generated and
        the pointer at the random index is returned. This solution has a time complexity of O(1) for getRandomNode() calls and space
        complexity of O(N) due to the space required for the array of pointers. If nodes were never removed from the tree, this
        solution would be fine. However, if nodes are removed from the tree, modifying and resizing pointer array requires O(N) time.
        A solution that is more optimal in the case of possible node removal is to randomly traverse the tree and
        return the node reached after random traversal. In this case, adding nodes to the tree requires O(log(N)) time, removing
        nodes from the tree requires O(log(N)) time, and getting a random node requires O(log(N)) time. The random traversal is
        enabled by incrementing and decrementing each node's number of children during operations that insert or delete a node from
        the tree. At each instance of a recursive traversal, the value of a random number is used to decide whether to
        traverse left, traverse right, or return the current node. The probabilities of the choices will be proportional to the
        number of nodes that each choice represents i.e. if a node has 4 left children and 3 right children, the probability to
        return itself is 1/8, the probability to traverse left is 4/8, and the probability to traverse right is 3/8. Since random
        number generator calls can be expensive, we employ an optimization in which we only generate a random number at the head
        of the tree, then propagate the random value to the recursive traversals after accounting for the reduction in node children
        at every level. This solution also uses O(N) space because each node must store its total number of children.
        Time complexity: O(log(N)) for a balanced tree. O(D) for an unbalanced tree where D is the depth of the tree.
        Space complexity: O(N) because the number of children must be stored for each node.
*/

/* A binary tree node has data, left child and right child */
typedef struct node
{
    int data;
    int children;
    struct node* left;
    struct node* right;
} NODE;

/* Helper function that allocates a new node with the given data
   and NULL left and right pointers. */
NODE*
newNode(int data) {

    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->data  = data;
    node->left  = NULL;
    node->right = NULL;
    node->children = 0;
    return(node);
}


// This is used to fill children counts.
int
getElements(NODE* root) {
    if (NULL == root) {
        return 0;
    }

    return getElements(root->left) + getElements(root->right) + 1;
}

// Inserts Children count for each node
void
insertChildrenCount(NODE* root) {
    if (NULL == root) {
        return;
    }

    root->children = getElements(root) - 1;
    insertChildrenCount(root->left);
    insertChildrenCount(root->right);
}

// returns number of children for root
int
children(NODE* root) {
    if (!root) {
        return 0;
    }
    return root->children + 1;
}

// Helper Function to return a random node
int
randomNodeUtil(NODE* root, int count) {
    if (NULL == root) {
        return 0;
    }

    if (count == children(root->left)) {
        return root->data;
    }

    if (count < children(root->left)) {
        return randomNodeUtil(root->left, count);
    }

    return randomNodeUtil(root->right, count - children(root->left) - 1);
}

// Returns Random node
int
randomNode(NODE* root) {

    srand(time(0));

    int count = rand() % (root->children + 1);
    return randomNodeUtil(root, count);
}

int main()
{
    // Creating Above Tree
    NODE* root = newNode(10);
    root->left = newNode(20);
    root->right = newNode(30);
    root->left->right = newNode(40);
    root->left->right = newNode(50);
    root->right->left = newNode(60);
    root->right->right = newNode(70);

    insertChildrenCount(root);

    printf("A Random Node From Tree : %d\r\n", randomNode(root));

    return 0;
}
