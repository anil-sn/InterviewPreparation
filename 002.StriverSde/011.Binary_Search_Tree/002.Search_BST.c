#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

typedef struct node {
    int data;
    struct node * left;
    struct node * right;
}NODE;

NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

void insertBST(NODE *root, int data)
{
    NODE * new = newNode(data);

    while (root != NULL && root->data != data) {

        if (data <= root->data) {
            if (NULL != root->left) {
                root = root->left;
            } else {
                root->left = new;
                break;
            }
        } else {
            if (NULL != root->right) {
                root = root->right;
            } else {
                root->right = new;
                break;
            }
        }
    }

    return;
}

NODE* findLastRight(NODE* root)
{
    if (root->right == NULL) {
        return root;
    }

    return findLastRight(root->right);
}

NODE* helper(NODE* root)
{
    if (root->left == NULL) {
        return root->right;
    } else if (root->right == NULL)
    {
        return root->left;
    }

    NODE* rightChild = root->right;
    NODE* lastRight = findLastRight(root->left);
    lastRight->right = rightChild;

    return root->left;
}

NODE* deleteBST(NODE* root, int key)
{
    if (root == NULL) {
        return NULL;
    }

    if (root->data == key) {
        return helper(root);
    }

    NODE *dummy = root;
    while (root != NULL) {

        if (root->data > key) {

            if (root->left != NULL && root->left->data == key) {

                root->left = helper(root->left);
                break;
            } else {
                root = root->left;
            }
        } else {
            if (root->right != NULL && root->right->data == key) {
                root->right = helper(root->right);
                break;
            } else {
                root = root->right;
            }
        }
    }

    return dummy;
}

NODE* searchBST (NODE* root, int val)
{
    while(root != NULL && root->data != val) {

        root = (val < root->data) ? (root->left) : (root->right);
    }

    return root;
}


void inOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    inOrderTrav(curr->left);
    printf("%5d ", curr->data);
    inOrderTrav(curr->right);
}

void preOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    printf("%5d ", curr->data);
    preOrderTrav(curr->left);
    preOrderTrav(curr->right);
}

void postOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    postOrderTrav(curr->left);
    postOrderTrav(curr->right);
    printf("%5d ", curr->data);
}


int height(NODE* node)
{
    if (node == NULL) {
        return 0;
    }

    int lheight = height(node->left);
    int rheight = height(node->right);

    if (lheight > rheight) {
        return (lheight + 1);
    } else {
        return (rheight + 1);
    }

    return 0;
}

void printGivenLevel(NODE* root, int level)
{
    if (root == NULL) {
        return;
    }

    if (level == 1) {
        printf("%3d ", root->data);
    } else if (level > 1) {
        printGivenLevel(root->left, level-1);
        printGivenLevel(root->right, level-1);
    }
}

void printLevelOrder(NODE* root)
{
    int h = height(root);
    for (int i=1; i<=h; i++) {
        printGivenLevel(root, i);
        printf("\n");
    }
}

int main()
{
    int time = 0;

    NODE * root = newNode(10);

    insertBST(root, 5);
    insertBST(root, 4);
    insertBST(root, 6);
    insertBST(root, 3);
    insertBST(root, 7);
    insertBST(root, 2);
    insertBST(root, 8);
    insertBST(root, 1);
    insertBST(root, 9);

    insertBST(root, 15);
    insertBST(root, 14);
    insertBST(root, 16);
    insertBST(root, 13);
    insertBST(root, 17);
    insertBST(root, 12);
    insertBST(root, 18);
    insertBST(root, 11);
    insertBST(root, 19);

    printf("\r\n");
    printLevelOrder(root);
    printf("\r\n\r\n");

    printf("Pre Order : \r\n");
    preOrderTrav(root);
    printf("\r\n");

    printf("In Order : \r\n");
    inOrderTrav(root);
    printf("\r\n");

    printf("Post Order : \r\n");
    postOrderTrav(root);
    printf("\r\n\r\n");

    printf("Search BST for data 20 result : %s \r\n", (searchBST(root, 20)) ? "Found" : "Not Found" );
    printf("Search BST for data 9 result : %s \r\n", (searchBST(root, 9)) ? "Found" : "Not Found" );
    printf("Search BST for data 19 result : %s \r\n", (searchBST(root, 19)) ? "Found" : "Not Found" );

    deleteBST(root, 20);
    deleteBST(root, 9);
    deleteBST(root, 19);


    printf("\r\n");
    printLevelOrder(root);
    printf("\r\n\r\n");

    printf("Pre Order : \r\n");
    preOrderTrav(root);
    printf("\r\n");

    printf("In Order : \r\n");
    inOrderTrav(root);
    printf("\r\n");

    printf("Post Order : \r\n");
    postOrderTrav(root);
    printf("\r\n\r\n");


    printf("Search BST for data 20 result : %s \r\n", (searchBST(root, 20)) ? "Found" : "Not Found" );
    printf("Search BST for data 9 result : %s \r\n", (searchBST(root, 9)) ? "Found" : "Not Found" );
    printf("Search BST for data 19 result : %s \r\n", (searchBST(root, 19)) ? "Found" : "Not Found" );

    return 0;
}