#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX_STACK 1000
#define MAX(a_, b_) ((a_) > (b_) ? (a_) : (b_))

typedef struct node {
    int data;
    int horizontalLevel;
    int verticalLevel;
    int index;
    struct node * left;
    struct node * right;
}NODE;

NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->horizontalLevel = 0;
    node->verticalLevel = 0;
    node->index = 0;

    return (node);
}

void changeTree(NODE *root)
{
    if(NULL == root) {
        return;
    }

    int child = 0;

    if(NULL != root->left) {
        child += root->left->data;
    }

    if(NULL != root->right) {
        child += root->right->data;
    }

    if(child >= root->data) {
        root->data = child;
    } else {
        if(NULL != root->left) {
            root->left->data = root->data;
        } else if (NULL != root->right) {
            root->right->data = root->data;
        }
    }

    changeTree(root->left);
    changeTree(root->right);

    int tot = 0;
    if(NULL != root->left ) {
        tot += root->left->data;
    }

    if(NULL != root->right) {
        tot+= root->right->data;
    }

    if(NULL != root->left  || NULL != root->right) {
        root->data = tot;
    }
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

int main()
{
    NODE * root = newNode(50);
    root->left = newNode(7);
    root->left->left = newNode(3);
    root->left->right = newNode(5);

    root->right = newNode(2);
    root->right->left = newNode(1);
    root->right->right = newNode(30);

    printf("Children Sum Of Binary Tree : \r\n");
    printf("Before Update: PreOrder \r\n");
    preOrderTrav(root);
    printf("\r\n");

    printf("Before Update: Inorder \r\n");
    inOrderTrav(root);
    printf("\r\n");

    printf("Before Update: PostOrder \r\n");
    postOrderTrav(root);
    printf("\r\n");

    printf("Update The Binary Tree \r\n");
    changeTree(root);

    printf("After Update: PreOrder \r\n");
    preOrderTrav(root);
    printf("\r\n");

    printf("After Update: Inorder \r\n");
    inOrderTrav(root);
    printf("\r\n");

    printf("After Update: PostOrder \r\n");
    postOrderTrav(root);
    printf("\r\n");


    return 0;
}