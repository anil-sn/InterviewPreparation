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

typedef struct node {
    int key;
    struct node *left;
    struct node *right;
}NODE;

void inOrder(NODE *n) {
    if (NULL == n) {
        return;
    }
    inOrder(n->left);
    printf ("%d ", n->key);
    inOrder(n->right);

    return;
}

void postOrder(NODE *n) {
    if (NULL == n) {
        return;
    }
    postOrder(n->left);
    postOrder(n->right);
    printf ("%d ", n->key);

    return;
}

void preOrder(NODE *n) {
    if (NULL == n) {
        return;
    }

    printf ("%d ", n->key);
    preOrder(n->left);
    preOrder(n->right);

    return;
}

NODE * newNode(int key) {
    NODE *newNode = malloc(sizeof(NODE));
    assert(NULL != newNode);

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
}

NODE * insert(NODE *node, int key) {
    if (NULL == node) {
        return (newNode(key));
    }

    if (key > node->key ) {
        node->left = insert(node->left, key);
    } else if (key < node->key) {
        node->right = insert(node->left, key);
    } else {
        printf("Duplicate addition, Need to handle it \r\n");
    }

    return;
}

// Find the inorder successor
NODE *minValueNode(NODE *node) {
    NODE *current = node;

    // Find the leftmost leaf
    while (current && current->left != NULL) {
        current = current->left;
    }

    return current;
}

NODE * delete(NODE *root, int key) {
    // Return if the tree is empty
    if (root == NULL) {
        return root;
    }

    // Find the node to be deleted
    if (key < root->key) {
        root->left = delete(root->left, key);
    } else if (key > root->key) {
        root->right = delete(root->right, key);
    } else {
        // If the node is with only one child or no child
        if (root->left == NULL) {
            NODE *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            NODE *temp = root->left;
            free(root);
            return temp;
        }

        // If the node has two children
        NODE *temp = minValueNode(root->right);

        // Place the inorder successor in position of the node to be deleted
        root->key = temp->key;

        // Delete the inorder successor
        root->right = delete(root->right, temp->key);
    }

    return root;
}

int main () {
    NODE *root = NULL;
    root = insert(root, 8);
    root = insert(root, 3);
    root = insert(root, 1);
    root = insert(root, 6);
    root = insert(root, 7);
    root = insert(root, 10);
    root = insert(root, 14);
    root = insert(root, 4);

    printf("\r\nPreOrder traversal: ");
    preOrder(root);
    printf("\r\nInorder traversal: ");
    inOrder(root);
    printf("\r\nPostOrder traversal: ");
    postOrder(root);

    printf("\nAfter deleting 10\n");
    root = delete(root, 10);

    printf("\r\nPreOrder traversal: ");
    preOrder(root);
    printf("\r\nInorder traversal: ");
    inOrder(root);
    printf("\r\nPostOrder traversal: ");
    postOrder(root);

    return;
}