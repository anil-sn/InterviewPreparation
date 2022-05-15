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

// {8,5,1,7,10,12}
NODE* build(int preOrder[], int len, int *i, int bound)
{
    if ((*i == len) || (preOrder[*i] > bound)) {
        return NULL;
    }

    NODE* root = newNode(preOrder[(*i)++]);
    root->left = build(preOrder,  len, i, root->data);
    root->right = build(preOrder, len, i, bound);
    return root;
}

NODE* bstFromPreorder(int preOrder[], int len)
{
    int i = 0;
    return build(preOrder, len, &i, INT_MAX);
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

bool isValid(NODE *root, int minVal, int maxVal)
{
    if (root == NULL) {
        return true;
    }

    if (root->data >= maxVal || root->data <= minVal) {
        return false;
    }

    return isValid(root->left, minVal, root->data) && isValid(root->right, root->data, maxVal);
}

bool isValidBST(NODE *root)
{
    return isValid(root, INT_MIN, INT_MAX);
}

int main ()
{
    int preOrder[] = {8,5,1,7,10,12};
    int len = sizeof(preOrder)/sizeof(preOrder[0]);

    NODE* root = bstFromPreorder(preOrder, len);

    printf("Pre Order : \r\n");
    preOrderTrav(root);
    printf("\r\n");

    printf("In Order : \r\n");
    inOrderTrav(root);
    printf("\r\n");

    printf("Post Order : \r\n");
    postOrderTrav(root);
    printf("\r\n\r\n");

    printf("Is the constructed BST is valid : %s \r\n", isValidBST(root) ? "TRUE" : "FALSE");
    return;
}