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
    4.6 Successor: Write an algorithm to find the "next" node (i.e., in-order successor) of a given node in a
                   binary search tree. You may assume that each node has a link to its parent.
    SOLUTION
        Recall that an in-order traversal traverses the left subtree, then the current node, then the right subtree. To
        approach this problem, we need to think very, very carefully about what happens.
        Let's suppose we have a hypothetical node. We know that the order goes left subtree, then current side,
        then right subtree. So, the next node we visit should be on the right side.
        But which node on the right subtree? It should be the first node we'd visit if we were doing an in-order
        traversal of that subtree. This means that it should be the leftmost node on the right subtree. Easy enough!
        But what if the node doesn't have a right subtree? This is where it gets a bit trickier.
        If a node n doesn't have a right subtree, then we are done traversing n's subtree. We need to pick up where
        we left off with n's parent, which we'll call q.
        If n was to the left of q, then the next node we should traverse should be q
        (again, since left - > current -> right).
        If n were to the right of q, then we have fully traversed q's subtree as well. We need to traverse upwards from
        q until we find a node x that we have not fully traversed. How do we know that we have not fully traversed
        a node x? We know we have hit this case when we move from a left node to its parent. The left node is fully
        traversed, but its parent is not.
*/

#include <stdio.h>
#include <stdlib.h>

/* A binary tree node has data, the pointer to left child and a pointer to right child */
typedef struct node {
    int data;
    struct node* left;
    struct node* right;
    struct node* parent;
} NODE;

/*  Given a non-empty binary search tree, return the minimum data value found in that tree.
    Note that the entire tree does not need to be searched. */
NODE* minValue(NODE* node)
{
    NODE* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}


/*
    Method 1 (Uses Parent Pointer)
    In this method, we assume that every node has a parent pointer.
    The Algorithm is divided into two cases on the basis of the right subtree of the input node being empty or not.

    Input: node, root // node is the node whose Inorder successor is needed.
    Output: succ // succ is Inorder successor of node.

    1.  If right subtree of node is not NULL, then succ lies in right subtree. Do the following.
        Go to right subtree and return the node with minimum key value in the right subtree.
    2.  If right subtree of node is NULL, then succ is one of the ancestors. Do the following.
        Travel up using the parent pointer until you see a node which is left child of its parent. The parent of such a node is the succ.
*/

NODE*
inOrderSuccessor1(NODE* root, NODE* n) {

    /* step 1 of the above algorithm */
    if (n->right != NULL) {
        return minValue(n->right);
    }

    /* step 2 of the above algorithm */
    NODE* p = n->parent;
    while (p != NULL && n == p->right) {
        n = p;
        p = p->parent;
    }
    return p;
}

/*
    Method 2 (Search from root)
    Parent pointer is NOT needed in this algorithm. The Algorithm is divided into two cases on the basis of right subtree of the input node being empty or not.

    Input: node, root // node is the node whose Inorder successor is needed.

    Output: succ // succ is Inorder successor of node.

    1.  If right subtree of node is not NULL, then succ lies in right subtree. Do the following.
        Go to right subtree and return the node with minimum key value in the right subtree.
    2.  If right subtree of node is NULL, then start from the root and use search-like technique. Do the following.
        Travel down the tree, if a node’s data is greater than root’s data then go right side, otherwise, go to left side.
*/

NODE*
inOrderSuccessor2( NODE* root, NODE* n) {

    /* step 1 of the above algorithm */
    if (n->right != NULL)
        return minValue(n->right);

    NODE* succ = NULL;

    /* Start from root and search for successor down the tree */
    while (root != NULL) {
        if (n->data < root->data) {
            succ = root;
            root = root->left;
        }
        else if (n->data > root->data) {
            root = root->right;
        }
        else {
            break;
        }
    }

    return succ;
}

/* Helper function that allocates a new node with the given data and NULL left and right pointers. */
NODE*
newNode(int data) {
    NODE* node = (NODE*) malloc(sizeof(NODE));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return (node);
}

/* Give a binary search tree and a number, inserts a new node with the given number in the correct place in the tree.
   Returns the new root pointer which the caller should then use
   (the standard trick to avoid using reference parameters). */
NODE*
insert(NODE* node, int data) {
    NODE* temp;

    /* 1. If the tree is empty, return a new, single node */
    if (node == NULL)
        return (newNode(data));
    else { /* 2. Otherwise, recur down the tree */
        if (data <= node->data) {
            temp = insert(node->left, data);
            node->left = temp;
            temp->parent = node;
        }
        else {
            temp = insert(node->right, data);
            node->right = temp;
            temp->parent = node;
        }

        /* return the (unchanged) node pointer */
        return node;
    }
}

/* Driver program to test above functions*/
int main()
{
    NODE *root = NULL, *temp, *succ, *min;

    // creating the tree given in the above diagram
    root = insert(root, 20);
    root = insert(root, 8);
    root = insert(root, 22);
    root = insert(root, 4);
    root = insert(root, 12);
    root = insert(root, 10);
    root = insert(root, 14);
    temp = root->left->right->right;

    succ = inOrderSuccessor1(root, temp);
    if (succ != NULL) {
        printf( "\n[Method 1] Inorder Successor of %d is %d \r\n", temp->data, succ->data);
    } else {
        printf("\n[Method 1] Inorder Successor doesn't exit \r\n");
    }

    succ = inOrderSuccessor2(root, temp);
    if (succ != NULL) {
        printf( "\n[Method 1] Inorder Successor of %d is %d \r\n", temp->data, succ->data);
    } else {
        printf("\n[Method 1] Inorder Successor doesn't exit \r\n");
    }

    return 0;
}
