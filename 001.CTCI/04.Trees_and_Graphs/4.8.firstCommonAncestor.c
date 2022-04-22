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
    4.8 First Common Ancestor: Design an algorithm and write code to find the first common ancestor
            of two nodes in a binary tree. Avoid storing additional nodes in a data structure. NOTE: This is not
            necessarily a binary search tree.

    SOLUTION
        If this were a binary search tree, we could modify the find operation for the two nodes and see where the
        paths diverge. Unfortunately, this is not a binary search tree, so we must try other approaches.
        Let's assume we're looking for the common ancestor of nodes p and q. One question to ask here is if each
        node in our tree has a link to its parents.

    Solution #1: With Links to Parents
        If each node has a link to its parent, we could trace p and q's paths up until they intersect. This is essentially
        the same problem as question 2.7 which find the intersection of two linked lists. The "linked list" in this case
        is the path from each node up to the root.

    Solution #2: With Links to Parents (Better Worst-Case Runtime)
        Similar to the earlier approach, we could trace p's path upwards and check if any of the nodes cover q.
        The first node that covers q (we already know that every node on this path will cover p) must be the first
        common ancestor.

    Following is the definition of LCA from Wikipedia:

    Let T be a rooted tree. The lowest common ancestor between two nodes n1 and n2 is defined as the lowest node in T
    that has both n1 and n2 as descendants (where we allow a node to be a descendant of itself).
    The LCA of n1 and n2 in T is the shared ancestor of n1 and n2 that is located farthest from the root.
    Computation of lowest common ancestors may be useful, for instance, as part of a procedure for determining the
    distance between pairs of nodes in a tree: the distance from n1 to n2 can be computed as the distance from
    the root to n1, plus the distance from the root to n2, minus twice the distance from the root to their lowest common ancestor.

    Approach: For Binary search tree, while traversing the tree from top to bottom the first node which lies in between
    the two numbers n1 and n2 is the LCA of the nodes, i.e. the first node n with the lowest depth which lies in
    between n1 and n2 (n1<=n<=n2) n1 < n2. So just recursively traverse the BST in, if node’s value is greater
    than both n1 and n2 then our LCA lies in the left side of the node, if it’s is smaller than both n1 and n2,
    then LCA lies on the right side. Otherwise, the root is LCA (assuming that both n1 and n2 are present in BST).

    Algorithm:

    1. Create a recursive function that takes a node and the two values n1 and n2.
    2. If the value of the current node is less than both n1 and n2,
       then LCA lies in the right subtree. Call the recursive function for the right subtree.
    3. If the value of the current node is greater than both n1 and n2,
       then LCA lies in the left subtree. Call the recursive function for the left subtree.
    4. If both the above cases are false then return the current node as LCA.
*/

typedef struct node
{
    int data;
    struct node* left, *right;
} NODE;

/* Function to find LCA of n1 and n2. The function assumes that both n1 and n2 are present in BST */
NODE
*lca(NODE* root, int n1, int n2) {
    if (root == NULL) {
        return NULL;
    }

    // If both n1 and n2 are smaller than root, then LCA lies in left
    if (root->data > n1 && root->data > n2) {
        return lca(root->left, n1, n2);
    }

    // If both n1 and n2 are greater than root, then LCA lies in right
    if (root->data < n1 && root->data < n2) {
        return lca(root->right, n1, n2);
    }

    return root;
}

/* Helper function that allocates a new node with the given data.*/
NODE*
newNode(int data) {
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->data  = data;
    node->left  = NULL;
    node->right = NULL;
    return(node);
}

/* Driver program to test lca() */
int main()
{
    /* Let us construct the BST shown in the above figure */
    NODE *root        = newNode(20);
    root->left               = newNode(8);
    root->right              = newNode(22);
    root->left->left         = newNode(4);
    root->left->right        = newNode(12);
    root->left->right->left  = newNode(10);
    root->left->right->right = newNode(14);

    int n1 = 10, n2 = 14;
    NODE *t = lca(root, n1, n2);
    printf("LCA of %d and %d is %d \n", n1, n2, t->data);

    n1 = 14, n2 = 8;
    t = lca(root, n1, n2);
    printf("LCA of %d and %d is %d \n", n1, n2, t->data);

    n1 = 10, n2 = 22;
    t = lca(root, n1, n2);
    printf("LCA of %d and %d is %d \n", n1, n2, t->data);

    return 0;
}
