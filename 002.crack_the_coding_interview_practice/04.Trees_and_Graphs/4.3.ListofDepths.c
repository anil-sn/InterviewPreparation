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
    List of Depths: Given a binary tree, design an algorithm which creates a linked list of all the nodes
                    at each depth (e.g., if you have a tree with depth D, you'll have D linked lists).

    SOLUTION
        Though we might think at first glance that this problem requires a level-by-level traversal, this isn't actually
        necessary. We can traverse the graph any way that we'd like, provided we know which level we're on as we
        do so. We can implement a simple modification of the pre-order traversal algorithm, where we pass in
        level + 1 to the next recursive call. The code below provides an implementation using depth-first search.

        One might ask which of these solutions is more efficient. Both run in O ( N) time, but what about the space
        efficiency? At first, we might want to claim that the second solution is more space efficient.
        In a sense, that's correct. The first solution uses 0( log N) recursive calls (in a balanced tree), each of which
        adds a new level to the stack. The second solution, which is iterative, does not require this extra space.
        However, both solutions require returning O(N) data. The extra 0( log N) space usage from the recursive
        implementation is dwarfed by the O ( N) data that must be returned. So while the first solution may actually
        use more data, they are equally efficient when it comes to "big O:'

        Objective: Given a Binary tree create Linked Lists of all the nodes at each depth ,
                   say if the tree has height k then create k linked lists.

        NOTE : This problem is very similar “Print binary tree, each level in one line“

        Input: A binary tree

        Output: K linked lists if the height of tree is k. Each linked list will have all the nodes of each level.

        Input Tree
               A
              / \
             B   C
            / \   \
           D   E   F


        Output Tree
               A--->NULL
              / \
             B-->C-->NULL
            / \   \
           D-->E-->F-->NULL
       Also Similar to Level Order Binary Tree Traversal


*/

typedef struct node {
    int data;
    struct node* left;
    struct node* right;
    struct node* nextRight;
} NODE;

/* UTILITY FUNCTIONS */
/* Helper function that allocates a new node with the given data and NULL left and right pointers. */
NODE* newnode(int data) {
    NODE* node = (NODE*) malloc(sizeof(NODE));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->nextRight = NULL;

    return (node);
}

/* Set next right of all descendants of p, Assumption:  p is a complete binary tree */
void connectRecur(NODE* p) {
    // Base case
    if (!p) {
        return;
    }

    // Set the nextRight pointer for p's left child
    if (p->left) {
        p->left->nextRight = p->right;
    }

    // Set the nextRight pointer for p's right child p->nextRight will be NULL if p is the right most child at its level
    if (p->right) {
        p->right->nextRight = (p->nextRight) ? p->nextRight->left : NULL;
    }

    // Set nextRight for other nodes in pre order fashion
    connectRecur(p->left);
    connectRecur(p->right);
}

// Sets the nextRight of root and calls connectRecur()
// for other nodes
void connect(NODE* p) {
    // Set the nextRight for root
    p->nextRight = NULL;

    // Set the next right for rest of the nodes
    // (other than root)
    connectRecur(p);
}

void
printRightNodes(NODE* n) {

    while (NULL != n) {
        printf("-> %d", n->data);
        n = n->nextRight;
    }

    printf("-> %s", "NULL");
    return;
}

/* Driver program to test above functions*/
int main() {

    /* Constructed binary tree is
            10
          /   \
        8      2
      /  \    /
    3     5  9
  */
    NODE* root = newnode(10);
    root->left = newnode(8);
    root->left->left = newnode(3);
    root->left->right = newnode(5);
    root->right = newnode(2);
    root->right->left = newnode(9);

    // Populates nextRight pointer in all nodes
    connect(root);

    // Let us check the values of nextRight pointers
    printf("Following are populated nextRight pointers in the tree (NULL is printed if there is no nextRight) \n");
    printRightNodes(root);
    printf("\r\n");
    printRightNodes(root->left);
    printf("\r\n");
    printRightNodes(root->left->left);
    printf("\r\n");

    return 0;
}

